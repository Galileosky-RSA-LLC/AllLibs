//! @file
//! @brief Функции библиотеки маршрутного автоинформатора

#ifdef RAI_LIB
#endinput
#endif
#define RAI_LIB

#include "rai.h"

//! @publicsection

stock raiGetCurrentRoute(route[RAI_ROUTE_DATA])
{
    new res = rai_readRouteName(route);
	// признак, что папка - именно маршрут - наличие файла с конечными остановками, хоть и пустого
	res *= rai_getFinalStationsFile(route);
	if (!res)
		return 0;
	route.crc = CRC16(route.name, GS_strlen(route.name));
	return res + rai_generateFilePath(route, RAI_BUSLINE_FILE_NAME, RAI_BUSLINE_FILE_NAME_LENGTH, route.busLineFilePath);
}

stock raiGetNewRoute(route[RAI_ROUTE_DATA], diag)
{
	new res = true;
	new scan = 0;
	while ((res) && (scan <= 1))
	{
		if (diag)
			Diagnostics("GetNewRoute. last dir=%s", route.name);
		res = NextDir("/", route.name, route.name, RAI_FILE_PATH_LENGTH_MAX_W0);
		if (res)
		{
			if (rai_getFinalStationsFile(route))
			{
				if (diag)
					Diagnostics("GetNewRoute. found route dir=%s", route.name);
				if (!rai_writeRouteName(route))
				{
					Diagnostics("GetNewRoute. ERROR write route");
					return 0;
				}
				route.crc = CRC16(route.name, GS_strlen(route.name));
				return 1 + rai_generateFilePath(route, RAI_BUSLINE_FILE_NAME, RAI_BUSLINE_FILE_NAME_LENGTH, route.busLineFilePath);
			}
		}
		else
		{
			if (diag)
				Diagnostics("GetNewRoute. dir not found, rescan");
			scan++;
			res = true;
			route.name{0} = 0;
		}
	}
	route.name{0} = 0;
	return 0;
}

stock raiPlayRouteAudio(route[RAI_ROUTE_DATA])
{
	new res = rai_generateFilePath(route, RAI_AUDIO_FILE_NAME, RAI_AUDIO_FILE_NAME_LENGTH, route.audioFilePath);
	res = res && (FileSize(route.audioFilePath) > 0);
	if (res)
		PlayAudio(route.audioFilePath);
	else
		Diagnostics("Route audio file %s not found", route.audioFilePath);
	return res;
}

stock raiGetFinalStations(route[RAI_ROUTE_DATA])
{
	route.startStation{0} = 0;
	route.stopStation{0} = 0;
	if ((FileSize(route.finalStations) < 0) || (route.name{0} == 0))
		return 0;

	new len = FileRead(route.finalStations, route.startStation, RAI_STRING_LENGTH_MAX - 1, 0);
	new pos = 0;
	for (; pos < len; ++pos)
	{
		if (route.startStation{pos} == RAI_CR_SYMBOL)
			break;
	}
	route.startStation{pos} = 0;
	pos = pos + 2;
	len = FileRead(route.finalStations, route.stopStation, RAI_STRING_LENGTH_MAX - 1, pos);
	pos = 0;
	for (; pos < len; ++pos)
	{
		if (route.stopStation{pos} == RAI_CR_SYMBOL)
			break;
	}
	route.stopStation{pos} = 0;
	Diagnostics("RouteAutoinformer. First and last stations:");
	Diagnostics(route.startStation);
	Diagnostics(route.stopStation);
	return 1;
}

stock raiInStationGeoPos(route[RAI_ROUTE_DATA], station{}, &nextStationFilePos, diag)
{
	new currentFilePos = 0;
	new latitude       = 0;
	new longitude      = 0;
	new directionAngle = 0;
	new spreadAngle    = 0;
	new R0             = 0;
	new R1             = 0;

	while (true)
	{
		new rowData{RAI_STRING_LENGTH_MAX};
		new rowLen = GS_readLine(route.busLineFilePath, currentFilePos, rowData, RAI_STRING_LENGTH_MAX - 1);
		
		if (diag)
			Diagnostics("busline str=%s", rowData);
		
		if (!rowLen)
			return false;

		// В файле идет:
		// Широта, долгота, угол направления, угол разброса, внешний радиус R0, внутренний радиус R1, имя аудио файла, название остановки.
		new pos = 0;

		latitude       = rai_readParam(rowData, pos, 6);
		longitude      = rai_readParam(rowData, pos, 6);
		directionAngle = rai_readParam(rowData, pos, 2);
		spreadAngle    = rai_readParam(rowData, pos, 2);
		R0             = rai_readParam(rowData, pos, 2);
		R1             = rai_readParam(rowData, pos, 2);
		
		if (diag)
		{
			Diagnostics("str latitude=%d, longitude=%d", latitude, longitude);
			Diagnostics("str directionAngle=%d, spreadAngle=%d", directionAngle, spreadAngle);
			Diagnostics("str R0=%d, R1=%d", R0, R1);
		}

		currentFilePos = currentFilePos + rowLen + 2; // 2 символа - CR LF

		if (InZone(latitude, longitude, directionAngle, spreadAngle, R0, R1) == true)
		{
			new p = rai_getStationNameStart(rowData, pos, rowLen);

			if (diag)
				Diagnostics("InZone, stationNamePos=%d", p);

			if (p > 0)
			{
				new i = 0;
				for (; i + p < rowLen; ++i)
					station{i} = rowData{p + i};

				station{i} = 0;
				Diagnostics("Station: %s", station);
				new nextStation{RAI_STRING_LENGTH_MAX};
				nextStationFilePos = rai_getNextStationPos(route, currentFilePos, nextStation) + currentFilePos;
			}

			return true;
		}
		else
		{
			if (diag)
				Diagnostics("Not in Zone");
		}
	}

	return false;
}

stock raiGetAdvertisment(route[RAI_ROUTE_DATA], &filePos, adv{})
{
	adv{0} = 0;
	rai_generateFilePath(route, RAI_ADVERTISMENT_FILE_NAME, RAI_ADV_FILE_NAME_LENGTH, route.advertismentFilePath);
	if (FileSize(route.advertismentFilePath) <= 0)
		return false;

	new pos = filePos;
	new len = FileRead(route.advertismentFilePath, adv, RAI_STRING_LENGTH_MAX - 1, pos);

	if (len == 0)
	{
		pos = 0;
		len = FileRead(route.advertismentFilePath, adv, RAI_STRING_LENGTH_MAX - 1, pos);
	}

	new p = 0;
	for (; p < len; ++p)
	{
		if(adv{p} == RAI_CR_SYMBOL)
			break;
	}

	adv{p} = 0;
	filePos = pos + p + 2;
	return (adv{0} != 0);
}

stock raiGetNextStation(route[RAI_ROUTE_DATA], filePos, station{})
{
	station{0} = 0;
	if (filePos == 0)
		return false;

	new pos = 0;
	GS_append(station, pos, RAI_STRING_LENGTH_MAX, "Следующая остановка: ", 21);
	new buf{RAI_STRING_LENGTH_MAX};
	new len = GS_readLine(route.busLineFilePath, filePos, buf, RAI_STRING_LENGTH_MAX - 1);
	GS_append(station, pos, RAI_STRING_LENGTH_MAX - 1, buf, len);
	station{pos} = 0;
	return true;
}

//! @privatesection

//! Получить полный путь файла
//! Приватная функция
//! \param[in] route маршрут
//! \param[in] fileName имя файла
//! \param[in] fileNameLength длина имени файла
//! \param[out] path путь
//! \return 0 - ошибка (превышена длина / нет маршрута), 1 - успешно
stock rai_generateFilePath(route[RAI_ROUTE_DATA], fileName{}, fileNameLength, path{})
{
	if (route.name{0} == 0)
	{
		path{0} = 0;
		return 0;
	}
	new pos = 0;
	path{pos++} = '/';
	GS_append(path, pos, RAI_FILE_PATH_LENGTH_MAX_W0, route.name, GS_strlen(route.name));
    path{pos++} = '/';
	GS_append(path, pos, RAI_FILE_PATH_LENGTH_MAX_W0, fileName, fileNameLength);
	if (pos >= RAI_FILE_PATH_LENGTH_MAX_W0)
	{
		path{RAI_FILE_PATH_LENGTH_MAX_W0 - 1} = 0;
		return 0;
	}

	path{pos} = 0;
	return 1;
}

//! Прочитать название текущего маршрута из файла в корне SD
//! Приватная функция
//! \param[out] route маршрут
//! \return длина прочитанных данных, если 0, то нет файла 
stock rai_readRouteName(route[RAI_ROUTE_DATA])
{
	new len = FileRead(RAI_CURRENT_ROUTE_FILE_PATH, route.name, RAI_FILE_PATH_LENGTH_MAX);

	if (len == 0)
	{
		Diagnostics("RouteAutoinformer ERROR: Current route unknown");
		route.name{0} = 0;
		return 0;
	}

	if (len >= (RAI_FILE_PATH_LENGTH_MAX))
	{
		route.name{RAI_FILE_PATH_LENGTH_MAX} = 0;
		return RAI_FILE_PATH_LENGTH_MAX;
	}

	route.name{len} = 0;
	return len;
}

//! Сохранить имя маршрута в файл
//! Приватная функция
//! \param[in] route маршрут
stock rai_writeRouteName(route[RAI_ROUTE_DATA])
{
	FileDelete(RAI_CURRENT_ROUTE_FILE_PATH);
	return FileWrite(RAI_CURRENT_ROUTE_FILE_PATH, route.name, GS_strlen(route.name));
}

//! Проверить наличие файла с конечными остановками
//! Приватная функция
//! \param[in] route маршрут
//! \return 0 - нет файла, !=0 - файл существует
stock rai_getFinalStationsFile(route[RAI_ROUTE_DATA])
{
	rai_generateFilePath(route, RAI_FINAL_STATIONS_FILE_NAME, RAI_FINAL_STATIONS_FILE_NAME_LENGTH, route.finalStations);
	return (FileSize(route.finalStations) >= 0) && (route.name{0} != 0);
}

//! Получить значение из строки и пропустить 1 символ
//! Приватная функция
//! \param[in] buf{} строка
//! \param[inout] &pos с какой позиции искать
//! \param[in] precision требуемая точность после запятой
stock rai_readParam(buf{}, &pos, precision)
{
	new res = GS_atof(buf, pos, precision, '.');

	if (buf{pos} != 0)
	{
		++pos;
	}

	return res;
}

//! Получить позицию начала названия в строке
//! Приватная функция
//! \param rowData{} строка
//! \param pos с какой позиции искать
//! \param len длина строки
stock rai_getStationNameStart(rowData{}, pos, len)
{
	for (; pos < len; pos++)
	{
		if (rowData{pos} == ';')
		{
			if(pos < len)
			{
				return (pos + 1);
			}

			return 0;
		}
	}

	return 0;
}

//! Получить позицию начала следующей строки с остановкой
//! \param[in] route маршрут
//! \param[in] buf{} строка
//! \param[in] pos с какой позиции искать
stock rai_getNextStationPos(route[RAI_ROUTE_DATA], pos, buf{})
{
	new len = GS_readLine(route.busLineFilePath, pos, buf, RAI_STRING_LENGTH_MAX);

	if (!len)
	{
		len = GS_readLine(route.busLineFilePath, 0, buf, RAI_STRING_LENGTH_MAX);
		if (!len)
			return 0;
	}

	pos = 0;

	for (new i = 0; i < 6; ++i)
	{
		rai_readParam(buf, pos, 2);
	}

	return rai_getStationNameStart(buf, pos, len);
}
