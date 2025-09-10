//! @file
//! @brief Заголовок библиотеки общих функций алгоритма

#ifdef ALG_H
#endinput
#endif
#define ALG_H

#include "..\rai.h"

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