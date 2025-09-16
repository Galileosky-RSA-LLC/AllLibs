//! @file
//! @brief Функции библиотеки маршрутного автоинформатора

#ifdef RAI_LIB
#endinput
#endif
#define RAI_LIB

#include "rai.h"
#include "..\string\string.h"
#include "..\string\string.cpp"

//! @publicsection

stock raiGetCurrentRoute(route[RAI_ROUTE_DATA])
{
    return rai_restoreRouteName(route) && rai_getFilePaths(route);
}

stock raiGetNewRoute(const currentRoute[RAI_ROUTE_DATA], nextRoute[RAI_ROUTE_DATA])
{
	strncpy(nextRoute.name, 0, RAI_FILE_PATH_LENGTH_MAX, currentRoute.name);
    new isReScan = false;
	while (true)
	{
		if (NextDir("", nextRoute.name, nextRoute.name, RAI_FILE_PATH_LENGTH_MAX))
		{
			Diagnostics("found dir=%s", nextRoute.name);//!!!
            if (rai_getFilePaths(nextRoute))
                return true;

            continue;
		}
		else if (isReScan)
		{
			break;
		}
        isReScan = true;
        nextRoute.name{0} = 0;
	}
    nextRoute.name{0} = 0;
    return false;
}

stock raiGetFinalStations(route[RAI_ROUTE_DATA])
{
	route.startStation{0} = 0;
	route.endStation{0} = 0;
	if (!route.name{0} || (FileSize(route.finalStationsFilePath) < 0))
		return false;

	new pos = fileReadLine(route.finalStationsFilePath, route.startStation, RAI_STRING_LENGTH_MAX, 0);
    if (pos)
        fileReadLine(route.finalStationsFilePath, route.endStation, RAI_STRING_LENGTH_MAX, pos);

	return true;
}

stock raiIsOnStation(const route[RAI_ROUTE_DATA], &currentStationFilePos, &nextStationFilePos)
{
    new filePos = 0;
    new fileSize = FileSize(route.busLineFilePath);
	while (filePos < fileSize)
	{
		new str{RAI_STRING_LENGTH_MAX};
		new readSize = fileReadLine(route.busLineFilePath, str, RAI_STRING_LENGTH_MAX, filePos);
		if (!readSize)
			return false;

		// В строке файла идет:
		// Широта;долгота;угол направления;угол разброса;внешний радиус R0;внутренний радиус R1;имя аудио файла;название остановки.
        // типа:
        // 51.540487;46.004783;111.00;45.00;60.00;20.00;Universitatska_.wav;Остановка ул. Университетская
		new strPos = 0;

		new latitude       = rai_readParam(str, strPos, 6);
		new longitude      = rai_readParam(str, strPos, 6);
		new directionAngle = rai_readParam(str, strPos, 2);
		new spreadAngle    = rai_readParam(str, strPos, 2);
		new extRadiusR0             = rai_readParam(str, strPos, 2);
		new intRadiusR1             = rai_readParam(str, strPos, 2);
		
		Diagnostics("str latitude=%d,longitude=%d,directionAngle=%d,spreadAngle=%d,R0=%d,R1=%d", latitude, longitude, directionAngle, spreadAngle, extRadiusR0,
                    intRadiusR1);//!!!
		if (!InZone(latitude, longitude, directionAngle, spreadAngle, extRadiusR0, intRadiusR1))
        {
            filePos += readSize;
            continue;
        }
        new p = rai_getStationNameStart(str, strPos, rowLen);

        if (diag)
            Diagnostics("InZone, stationNamePos=%d", p);

        if (p > 0)
        {
            new i = 0;
            for (; i + p < rowLen; ++i)
                station{i} = str{p + i};

            station{i} = 0;
            Diagnostics("Station: %s", station);
            new nextStation{RAI_STRING_LENGTH_MAX};
            nextStationFilePos = rai_getNextStationPos(route, filePos, nextStation) + filePos;
        }

        return true;
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
		if(adv{p} == SYMBOL_CR)
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

//!!! в библиотеку файлов
//! @brief Прочитать строку из текстового файла
//! @details Завершение строк в файле может быть в форматах LF и CRLF
//! @param[in] fileFullPath имя файла, должно оканчиваться \0
//! @param[in] fileOffset смещение начала чтения
//! @param[out] buf буфер для прочитанной строки
//! @param[in] bufMaxSize предельный размер буфера для прочитанной строки, с учетом спец. символов завершения строки
//! @return прочитанный размер, включая спец. символы завершения и все промежуточные \0
stock fileReadLine(const fileFullPath{}, buf{}, bufMaxSize, fileOffset)
{
	new size = FileRead(fileFullPath, buf, bufMaxSize, fileOffset);
    if (size <= 0)
		return 0;

	new i;
    new hasCrSymbol = false;
    new hasLfSymbol = false;
    for (i = 0; (i < size) && !(hasLfSymbol = buf{i} == SYMBOL_LF); i++)
        hasCrSymbol = buf{i} == SYMBOL_CR;
    
    if (i < bufMaxSize)
        buf{hasCrSymbol && hasLfSymbol ? i - 1 : i} = 0;

    return hasLfSymbol ? i + 1 : i;
}

//! @privatesection

stock rai_generateFilePath(const route[RAI_ROUTE_DATA], const fileName{}, filePath{})
{
	new pos = 0;
    filePath{0} = 0;
    new routeNameLength = strLen(route.name);
    new fileNameLength = strLen(fileName);
    if ((routeNameLength <= 0) || (fileNameLength <= 0) || ((routeNameLength + fileNameLength + 1) > RAI_FILE_PATH_LENGTH_MAX))
        return false;

    pos += insertArrayStr(filePath, pos, RAI_FILE_PATH_LENGTH_MAX, route.name, routeNameLength);
    pos += insertArrayStr(filePath, pos, RAI_FILE_PATH_LENGTH_MAX, PATH_SEPARATOR_PRIME, strLen(PATH_SEPARATOR_PRIME));
    strncpy(filePath, pos, RAI_FILE_PATH_LENGTH_MAX, fileName);
	return true;
}

//! Прочитать название текущего маршрута из файла
//! @param[out] route структура маршрута
//! @return true - успешно, false - ошибка
stock rai_restoreRouteName(route[RAI_ROUTE_DATA])
{
	new len = FileRead(RAI_CURRENT_ROUTE_FILE_PATH, route.name, RAI_FILE_PATH_LENGTH_MAX);
	route.name{len <= 0 ? 0 : ((len < RAI_FILE_PATH_LENGTH_MAX) ? len : RAI_FILE_PATH_LENGTH_MAX)} = 0;
	return len > 0;
}

//! Сохранить имя маршрута в файл
//! @param[in] route структура маршрута
//! @return true - успешно, false - ошибка
stock rai_storeRouteName(const route[RAI_ROUTE_DATA])
{
	return !FileDelete(RAI_CURRENT_ROUTE_FILE_PATH) && FileWrite(RAI_CURRENT_ROUTE_FILE_PATH, route.name, strLen(route.name));
}

//! Получить путь файла с конечными остановками
//! @param[inout] route структура маршрута
//! @return true - успешно, false - ошибка
stock rai_getFinalStationsFilePath(route[RAI_ROUTE_DATA])
{
	return route.name{0}
            && rai_generateFilePath(route, RAI_FINAL_STATIONS_FILE_NAME, route.finalStationsFilePath)
            && (FileSize(route.finalStationsFilePath) >= 0);
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

stock rai_getFilePaths(route[RAI_ROUTE_DATA])
{
	if (!rai_getFinalStationsFilePath(route))
		return false;
	
	rai_generateFilePath(route, RAI_AUDIO_FILE_NAME, route.audioFilePath);
    rai_generateFilePath(route, RAI_BUSLINE_FILE_NAME, route.busLineFilePath);
    rai_generateFilePath(route, RAI_ADVERTISMENT_FILE_NAME, route.advertismentFilePath);
    return true;
}