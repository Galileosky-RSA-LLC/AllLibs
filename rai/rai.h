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

//!!! в библиотеку строк
#define SYMBOL_CR 0x0D
#define SYMBOL_LF 0x0A

//!!! в библиотеку файлов
#define SYMBOL_PATH_SEPARATOR_PRIME '/'
#define SYMBOL_PATH_SEPARATOR_SECOND '\\'
stock const PATH_SEPARATOR_PRIME{} = {SYMBOL_PATH_SEPARATOR_PRIME};
stock const PATH_SEPARATOR_SECOND{} = {SYMBOL_PATH_SEPARATOR_SECOND};

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
//! @param[in] route маршрут
//! @param[out] currentStation название текущей остановки, если в ее геозоне
//! @param[in] currentStationMaxSize предельная длина для названия текущей остановки
//! @param[out] nextStation название следующей остановки, если в геозоне текущей остановки
//! @param[in] nextStationMaxSize предельная длина для названия следующей остановки
//! @param[out] nextStationFilePos позиция в файле названия следующей остановки (-1 - название не найдено или не в геозоне текущей остановки)
//! @return !=0 - в геозоне, 0 - не в геозоне
forward stock raiIsAtStation(const route[RAI_ROUTE_DATA], currentStation{}, currentStationMaxSize, nextStation{}, nextStationMaxSize, &nextStationFilePos);

//! Получить рекламное сообщение
//! \param[in] route маршрут
//! \param[in] filePos смещение текущего сообщения в файле
//! \param[out] advertisment сообщение
//! \param[in] advertismentMaxSize предельная длина сообщения
//! \param[out] nextPos позиция следующего сообщения
//! \return true - успешно, false - ошибка
forward stock raiGetAdvertisment(const route[RAI_ROUTE_DATA], filePos, advertisment{}, advertismentMaxSize, &nextPos);

//! @brief Сохранить название маршрута в теге массива пользователя
//! @param[in] route маршрут
forward stock raiSaveRouteNameInTag(const route[RAI_ROUTE_DATA]);

