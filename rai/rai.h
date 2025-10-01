//! @file
//! @brief Заголовок библиотеки маршрутного автоинформатора

#ifdef RAI_H
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
stock const RAI_ADVERTISMENT_FILE_NAME{} = "advertisment.txt";
stock const RAI_AUDIO_FILE_NAME{} = "route.wav";

#define RAI_ROUTE_DATA [\
    .name{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .busLineFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .finalStationsFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .advertismentFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .audioFilePath{RAI_FILE_PATH_LENGTH_MAX_W0},\
    .startStation{RAI_STRING_LENGTH_MAX_W0},\
    .endStation{RAI_STRING_LENGTH_MAX_W0},\
]

//! @brief Получить имя и пути файлов текущего маршрута
//! @param[out] route структура маршрута
//! @return true - успешно, false - ошибка файловой системы
forward stock raiGetCurrentRoute(route[RAI_ROUTE_DATA]);

//! @brief Установить маршрут в автоинформаторе
//! @details Предварительно необходимо получить имя и пути маршрута
//! @param[in] route маршрут
forward stock raiSetCurrentRoute(const route[RAI_ROUTE_DATA]);

//! @brief Получить имя и пути файлов следующего маршрута
//! @param[in] currentRoute структура текущего маршрута
//! @param[out] nextRoute структура следующего маршрута
//! @return true - успешно, false - нет маршрутов или ошибка файловой системы
forward stock raiGetNewRoute(const currentRoute[RAI_ROUTE_DATA], nextRoute[RAI_ROUTE_DATA]);

//! @brief Получить начальную и конечную остановки маршрута из файла
//! @details Предварительно необходимо получить имя и пути маршрута
//! @param[inout] route маршрут
//! @return true - успешно, false - ошибка: нет маршрута или файла с конечными остановками
forward stock raiGetFinalStations(route[RAI_ROUTE_DATA]);

//! @brief Проверить на присутствие в геозоне остановки
//! @details Предварительно необходимо получить имя и пути маршрута
//! @param[in] route маршрут
//! @param[out] currentStation название текущей остановки, если в ее геозоне
//! @param[in] currentStationMaxSize предельная длина для названия текущей остановки
//! @param[out] nextStation название следующей остановки, если в геозоне текущей остановки
//! @param[in] nextStationMaxSize предельная длина для названия следующей остановки
//! @param[out] nextStationFilePos позиция в файле названия следующей остановки (-1 - название не найдено или не в геозоне текущей остановки)
//! @return true - в геозоне, false - не в геозоне
forward stock raiIsAtStation(const route[RAI_ROUTE_DATA], currentStation{}, currentStationMaxSize, nextStation{}, nextStationMaxSize, &nextStationFilePos);

//! @brief Получить рекламное сообщение
//! @details Предварительно необходимо получить имя и пути маршрута
//! @param[in] route маршрут
//! @param[in] filePos смещение сообщения в файле
//! @param[out] advertisment сообщение
//! @param[in] advertismentMaxSize предельная длина сообщения
//! @param[out] nextPos позиция следующего сообщения, если найдено текущее
//! @return true - успешно, false - ошибка
forward stock raiGetAdvertisment(const route[RAI_ROUTE_DATA], filePos, advertisment{}, advertismentMaxSize, &nextPos);

//! @brief Установить название маршрута в тег массива пользователя
//! @details Предварительно необходимо получить имя и пути маршрута
//! @param[in] route маршрут
forward stock raiSetRouteNameInUserArray(const route[RAI_ROUTE_DATA]);
