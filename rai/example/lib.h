//! @file
//! @brief Заголовок библиотеки общих функций алгоритма

#if defined ALG_H
#endinput
#endif
#define ALG_H

//! @defgroup displayIndexes Индексы табло
//! @{
#define FRONT_DISPLAY_INDEX 0
#define REAR_DISPLAY_INDEX 1
#define SIDE_DISPLAY_INDEX 2
#define SALON_DISPLAY_INDEX 3
//! @}

//! @defgroup showValues Отображаемые сообщения
//! @{
#define SHOW_UNKNOWN 0
#define SHOW_CURRENT_STATION 1
#define SHOW_NEXT_STATION 2
#define SHOW_ADVERTISMENT 3
//! @}

#define ROUTE_CURRENT_DATA [\
    .crc,\
    .isAtStation,\
    .nextStationFilePos,\
    .currentAdvertismentFilePos,\
    .nextAdvertismentFilePos,\
    .show,\
    .showStartUptime,\
]

//! @brief Получить статус инициализации настроек
//! @return true - настройки инициализированы, false - не инициализированы
forward bool:stock isSettingsInited();

//! @brief Установить статус инициализации настроек
forward stock setSettingsInited();

//! @brief Определить необходимость смены маршрута
//! @return true - нужно сменить маршрут, false - не нужно
forward bool:stock isNeedSwithRoute();

//! @brief Установить необходимость смены маршрута
//! @param[in] value устанавливаемое значение: true - нужно сменить, false - не нужно
forward stock setNeedSwithRoute(bool:value);

//! @brief Инициализировать текущие данные маршрута
//! @param[in] routeCrc crc маршрута
//! @param[out] routeCurrentData структура текущих данных маршрута
forward stock initRouteCurrentData(routeCrc, routeCurrentData[ROUTE_CURRENT_DATA]);

//! @brief Восстановить сохраненные текущие данные маршрута
//! @param[out] routeCurrentData структура текущих данных маршрута
forward stock restoreRouteCurrentData(routeCurrentData[ROUTE_CURRENT_DATA]);

//! @brief Сохранить текущие данные маршрута
//! @param[out] routeCurrentData структура текущих данных маршрута
forward stock storeRouteCurrentData(const routeCurrentData[ROUTE_CURRENT_DATA]);

//! @brief Проверить инициализацию табло
//! @param[in] displayIndex @ref displayIndexes
//! @return true - табло инициализировано, false - не инициализировано
forward bool:stock isDisplayInited(displayIndex);

//! @brief Установить статус инициализации табло
//! @param[in] displayIndex индекс табло
//! @param[in] value устанавливаемый статус: false - не инициализировано, true: инициализировано
forward stock setDisplayInit(displayIndex, bool:value);

//! @brief Сбросить статус инициализации всех табло
forward stock resetAlldisplaysInit();

//! @brief Получить длительность показа сообщения
forward stock getMessageShowTimeS();

//! @brief Установить длительность показа сообщения
forward stock setMessageShowTimeS(value);

//! @brief Сбросить таймер показа сообщений
forward stock resetShowTimer(routeCurrentData[ROUTE_CURRENT_DATA]);

//! @brief Изменить отображаемое сообщение на следующее
forward stock changeShow(routeCurrentData[ROUTE_CURRENT_DATA]);

//! @brief Получить часовой пояс
forward stock getTimeZone();

//! @brief Установить часовой пояс
forward stock setTimeZone(value);

//! @brief Получить индекс входа переключения маршрута
forward stock getRouteSwitchInputIdx();

//! @brief Установить индекс входа переключения маршрута
forward stock setRouteSwitchInputIdx(value);

//! @brief Рассчитать crc имени маршрута
forward stock calcRouteCrc(const routeName{});

//! @brief Проверить необходимость изменения отображаемой информации
forward bool:stock isNeedChangeShow(const routeCurrentData[ROUTE_CURRENT_DATA]);
