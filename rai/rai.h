//! @file
//! @brief Заголовок библиотеки маршрутного автоинформатора

#ifdef RAI_H
#endinput
#endif
#define RAI_H

//#define RAI_DEBUG // включить для отладки

#define RAI_FILE_PATH_LENGTH_MAX 255
#define RAI_FILE_PATH_LENGTH_MAX_W0 (RAI_FILE_PATH_LENGTH_MAX + 1)
#define RAI_STRING_LENGTH_MAX 255
#define RAI_STRING_LENGTH_MAX_W0 (RAI_STRING_LENGTH_MAX + 1)
stock const RAI_CURRENT_ROUTE_FILE_PATH{} = "route_name";
stock const RAI_FINAL_STATIONS_FILE_NAME{} = "stations.txt";
#define RAI_FINAL_STATIONS_FILE_NAME_LENGTH 12 // длина имени файла с конечными остановками
stock const RAI_BUSLINE_FILE_NAME{} = "BusLine.txt";
#define RAI_BUSLINE_FILE_NAME_LENGTH 11 // длина имени файла остановочной информации
stock const RAI_ADVERTISMENT_FILE_NAME{} = "advertisment.txt";
#define RAI_ADV_FILE_NAME_LENGTH 16 // длина имени файла вспомогательной информации
stock const RAI_AUDIO_FILE_NAME{} = "route.wav";
#define RAI_AUDIO_FILE_NAME_LENGTH 9 // длина имени аудиофайла маршрута

#define RAI_CR_SYMBOL 0x0D // символ возврата каретки
#define RAI_LF_SYMBOL 0x0A // символ конца строки

#define RAI_ROUTE_DATA [\
    .name{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .crc,\
    .busLineFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
	.finalStationsFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .advertismentFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .audioFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .startStation{RAI_STRING_LENGTH_MAX_W0},\
    .stopStation{RAI_STRING_LENGTH_MAX_W0},\
    .nextStationFilePos,\
    .advertismentFilePos,\
    .isShowAdvertisment,\
    .advertismentStartUptime,\
]

//! @brief Получить имя, crc и пути файлов текущего маршрута
//! @param[out] route структура маршрута
//! @return true - успешно, false - ошибка файловой системы
forward stock raiGetCurrentRoute(route[RAI_ROUTE_DATA]);

//! @brief Получить имя, crc и пути файлов следующего маршрута
//! @param[in] currentRoute структура текущего маршрута
//! @param[out] nextRoute структура следующего маршрута
//! @return true - успешно, false - нет папок с маршрутами или ошибка файловой системы
forward stock raiGetNewRoute(const currentRoute[RAI_ROUTE_DATA], nextRoute[RAI_ROUTE_DATA]);

//! @brief Получить начальную и конечную остановки маршрута из файла
//! @param[inout] route маршрут
//! @return true - успешно, false - ошибка: нет файла или он пустой
forward stock raiGetFinalStations(route[RAI_ROUTE_DATA]);

//! @brief Проверить на присутствие в геозоне остановки
//! @param[in] route маршрут
//! @param[out] station{} название остановки, если в ее геозоне
//! @param[out] &nextStationFilePos позиция следующей остановки в файле, если находимся в геозоне остановки
//! @param[in] diag признак диагностики
//! @return 0 - не в зоне, !=0 - в зоне
forward stock raiIsOnStation(route[RAI_ROUTE_DATA], station{}, &nextStationFilePos, diag);

//! Получить рекламное сообщение
//! \param[in] route маршрут
//! \param[in] &filePos позиция в файле, с которой считывать. Смещается после чтения
//! \param[out] adv{} сообщение
//! \return false - ошибка, true - успешно
forward stock raiGetAdvertisment(route[RAI_ROUTE_DATA], &filePos, adv{});

//! Получить текст следующей остановки "Следующая остановка: ..."
//! \param route маршрут
//! \param[in] filePos позиция в файле, с которой считывать
//! \param[out] station{} остановка
//! \return false - ошибка, true - успешно
forward stock raiGetNextStation(route[RAI_ROUTE_DATA], filePos, station{});

//! @brief Сохранить название маршрута в теге массива пользователя
//! @param[in] route маршрут
forward stock raiSaveRouteNameInTag(const route[RAI_ROUTE_DATA]);
