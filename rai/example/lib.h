//! @file
//! @brief Заголовок библиотеки общих функций алгоритма

#ifdef ALG_H
#endinput
#endif
#define ALG_H

#define FRONT_DISPLAY_INDEX 0
#define REAR_DISPLAY_INDEX 1
#define SIDE_DISPLAY_INDEX 2
#define SALON_DISPLAY_INDEX 3

#include "..\rai.h"

// что отображалось на салонном табло
#define SHOW_UNKNOWN 0
#define SHOW_CURRENT_STATION 1
#define SHOW_NEXT_STATION 2
#define SHOW_ADVERTISMENT 3

#define ROUTE_CURRENT_DATA [\
    .isOnStation,\
    .nextStationFilePos,\
    .advertismentFilePos,\
    .show,\
    .showStartUptime,\
]

//! Получить статус инициализации настроек
//! @return true - настройки инициализированы, false - не инициализированы
forward stock isSettingsInited();

//! Установить статус инициализации настроек
forward stock setSettingsInited();

//! Определить необходимость смены маршрута
//! @return true - нужно сменить маршрут, false - не нужно
forward stock isNeedSwithRoute();

//! Установить необходимость смены маршрута
//! @param[in] value устанавливаемое значение: true - нужно сменить, false - не нужно
forward stock setNeedSwithRoute(value);

//! Получить crc маршрута
forward stock getRouteCrc();

//! Установить crc маршрута
//! @param[in] value устанавливаемое значение
forward stock setRouteCrc(value);

//! Восстановить сохраненные текущие данные маршрута: позиции в файлах, таймеры, флаги
//! @param[out] route структура маршрута
forward stock restoreRouteCurrentData(route[RAI_ROUTE_DATA]);

//! Проверить инициализацию табло
//! @param[in] displayIndex индекс табло
//! @return true - табло инициализировано, false - не инициализировано
forward stock isDisplayInited(displayIndex);

//! Установить статус инициализации табло
//! @param[in] displayIndex индекс табло
//! @param[in] value устанавливаемый статус: 0 - не инициализировано, !=0: инициализировано
forward stock setDisplayInit(displayIndex, value);

//! Сбросить статус инициализации всех табло
forward stock resetAlldisplaysInit();