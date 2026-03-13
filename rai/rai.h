//! @file
//! @brief Заголовок библиотеки маршрутного автоинформатора

#if defined RAI_H
#endinput
#endif
#define RAI_H

#define RAI_FILE_PATH_LENGTH_MAX 255
#define RAI_FILE_PATH_LENGTH_MAX_W0 (RAI_FILE_PATH_LENGTH_MAX + 1)
#define RAI_STRING_LENGTH_MAX 255
#define RAI_STRING_LENGTH_MAX_W0 (RAI_STRING_LENGTH_MAX + 1)

stock const RAI_CURRENT_ROUTE_FILE_PATH{} = "route_name";
stock const RAI_FINAL_STATIONS_FILE_NAME{} = "stations.txt";
stock const RAI_BUSLINE_FILE_NAME{} = "BusLine.txt";
stock const RAI_ADVERTISEMENT_FILE_NAME{} = "advertisment.txt";
stock const RAI_AUDIO_FILE_NAME{} = "route.wav";

#define RAI_ROUTE_DATA [\
    .name{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .busLineFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .finalStationsFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .advertisementFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .audioFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .startStation{RAI_STRING_LENGTH_MAX_W0},\
    .endStation{RAI_STRING_LENGTH_MAX_W0},\
]

//! @brief Получить имя и пути файлов текущего маршрута
//! @param[out] route структура маршрута
//! @return true - успешно, false - ошибка файловой системы
forward bool:stock raiGetCurrentRoute(route[RAI_ROUTE_DATA]);

//! @brief Установить маршрут в автоинформаторе
//! @note Предварительно необходимо получить имя и пути маршрута raiGetCurrentRoute()
//! @param[in] route структура маршрута
//! @return true - успешно, false - ошибка (не получен маршрут, ошибка файловой системы)
forward bool:stock raiSetCurrentRoute(const route[RAI_ROUTE_DATA]);

//! @brief Получить имя и пути файлов следующего маршрута
//! @param[in] currentRoute структура текущего маршрута
//! @param[out] nextRoute структура следующего маршрута
//! @return true - успешно, false - нет маршрутов или ошибка файловой системы
forward bool:stock raiGetNewRoute(const currentRoute[RAI_ROUTE_DATA], nextRoute[RAI_ROUTE_DATA]);

//! @brief Получить начальную и конечную остановки маршрута из файла
//! @note Предварительно необходимо получить имя и пути маршрута raiGetCurrentRoute()
//! @param[inout] route структура маршрута
//! @return true - успешно, false - ошибка (нет маршрута или файла с конечными остановками)
forward bool:stock raiGetFinalStations(route[RAI_ROUTE_DATA]);

//! @brief Проверить на присутствие в геозоне остановки
//! @note Предварительно необходимо получить имя и пути маршрута raiGetCurrentRoute()
//! @param[in] route структура маршрута
//! @param[out] currentStation название текущей остановки, если в ее геозоне
//! @param[in] currentStationMaxSize предельная длина для названия текущей остановки
//! @param[out] nextStation название следующей остановки, если в геозоне текущей остановки
//! @param[in] nextStationMaxSize предельная длина для названия следующей остановки
//! @param[out] nextStationFilePos позиция в файле названия следующей остановки (-1 - название не найдено или не в геозоне текущей остановки)
//! @return true - в геозоне, false - не в геозоне
forward bool:stock raiIsAtStation(const route[RAI_ROUTE_DATA], currentStation{}, currentStationMaxSize, nextStation{}, nextStationMaxSize, &nextStationFilePos);

//! @brief Получить название следующей остановки
//! @note Предварительно необходимо получить имя и пути маршрута raiGetCurrentRoute()
//! @param[in] route структура маршрута
//! @param[in] filePos смещение в файле
//! @param[out] station название остановки при успешном возврате
//! @param[in] stationMaxSize предельная длина для названия остановки
//! @return true - успешно, false - ошибка
forward bool:stock raiGetNextStation(const route[RAI_ROUTE_DATA], filePos, station{}, stationMaxSize);

//! @brief Получить рекламное сообщение
//! @note Предварительно необходимо получить имя и пути маршрута raiGetCurrentRoute()
//! @param[in] route структура маршрута
//! @param[in] filePos смещение сообщения в файле
//! @param[out] advertisement сообщение
//! @param[in] advertisementMaxSize предельная длина сообщения
//! @param[out] nextPos позиция следующего сообщения, если найдено текущее
//! @return true - успешно, false - ошибка
forward bool:stock raiGetAdvertisement(const route[RAI_ROUTE_DATA], filePos, advertisement{}, advertisementMaxSize, &nextPos);

//! @brief Установить название маршрута в тег массива пользователя
//! @note Предварительно необходимо получить имя и пути маршрута raiGetCurrentRoute()
//! @param[in] route структура маршрута
forward stock raiSetRouteNameInUserArray(const route[RAI_ROUTE_DATA]);
