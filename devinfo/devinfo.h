#ifdef DEVINFO_H
#endinput
#endif
#define DEVINFO_H

//! @file
//! @brief Заголовок библиотеки определения информации о приборе

//! @defgroup models Модели приборов
//! @{
#define DEVINFO_MODEL_UNKNOWN 0
#define DEVINFO_MODEL_7X 1
#define DEVINFO_MODEL_70 2
#define DEVINFO_MODEL_BB 3
#define DEVINFO_MODEL_10 4
//! @}

//! @defgroup debugLevels Уровни отладки
//! @{
#define DEVINFO_DEBUGLEVEL_UNKNOWN -1
#define DEVINFO_DEBUGLEVEL_OFF 0
#define DEVINFO_DEBUGLEVEL_MIN 1
#define DEVINFO_DEBUGLEVEL_STD 2
#define DEVINFO_DEBUGLEVEL_MAX 3
//! @}

//! @defgroup statusBits Биты статуса устройства
//! @{
#define DEVINFO_STATUS_BIT_MOTION 0
#define DEVINFO_STATUS_BIT_INCLINE 1
#define DEVINFO_STATUS_BIT_IBUTTON 2
#define DEVINFO_STATUS_BIT_NOSIM 3
#define DEVINFO_STATUS_BIT_GEOFENCE 4
#define DEVINFO_STATUS_BIT_UINTLOW 5
#define DEVINFO_STATUS_BIT_GPSANTOFF 6
#define DEVINFO_STATUS_BIT_UINTBUSFAIL 7
#define DEVINFO_STATUS_BIT_EXTPOWERFAIL 8
#define DEVINFO_STATUS_BIT_ENGINEON 9
#define DEVINFO_STATUS_BIT_SHOCK 10
#define DEVINFO_STATUS_BIT_COORDS 11
#define DEVINFO_STATUS_BIT_SIGNALING 14
#define DEVINFO_STATUS_BIT_ALARM 15
#define DEVINFO_STATUS_BIT_MAINSRV 16
#define DEVINFO_STATUS_BIT_GPRS 17
#define DEVINFO_STATUS_BIT_GSMJAMMING 18
#define DEVINFO_STATUS_BIT_SECONDSRV 19
#define DEVINFO_STATUS_BIT_GNSSJAMMING 20
#define DEVINFO_STATUS_BIT_USB 21
#define DEVINFO_STATUS_BIT_SD 22
//! @}

//! @defgroup userspecBits Биты спецификации пользователя
//! @{
#define DEVINFO_USERSPEC_BIT_GNSS 0
#define DEVINFO_USERSPEC_BIT_OUTS 1
#define DEVINFO_USERSPEC_BIT_BLUETOOTH 2
#define DEVINFO_USERSPEC_BIT_WIFI 3
#define DEVINFO_USERSPEC_BIT_RS232 4
#define DEVINFO_USERSPEC_BIT_RS485 5
#define DEVINFO_USERSPEC_BIT_CAN 6
#define DEVINFO_USERSPEC_BIT_1WIRE 7
#define DEVINFO_USERSPEC_BIT_ARCHIVE 8
#define DEVINFO_USERSPEC_BIT_WEBDAV 9
#define DEVINFO_USERSPEC_BIT_SIGNALING 10
#define DEVINFO_USERSPEC_BIT_GEOFENCES 11
#define DEVINFO_USERSPEC_BIT_AUTOINFORMER 12
#define DEVINFO_USERSPEC_BIT_TRANSMESSYS 13
#define DEVINFO_USERSPEC_BIT_BLECONFIG 14
//! @}

//! @brief Получить модель прибора
//! @return @ref models
forward stock getModel();

//! @brief Получить версию прошивки прибора
//! @param[out] softMaj мажорная версия
//! @param[out] softMin минорная версия
//! @return true - успешно, false - ошибка
forward stock getSoftVersion(&softMaj, &softMin);

//! @brief Получить уровень диагностики
//! @return @ref debugLevels
forward stock getDebugLevel();

//! @brief Проверить наличие возврата для функции PortInit()
//! @param[in] devModel модель прибора DEVINFO_MODEL_
//! @param[in] softMaj мажорная версия прошивки
//! @param[in] softMin минорная версия прошивки
forward stock isPortInitHasResult(devModel, softMaj, softMin);

//! @brief Проверить доступность функций ROM (сохранение параметров в ПЗУ)
//! @param[in] devModel модель прибора DEVINFO_MODEL_
//! @param[in] softMaj мажорная версия прошивки
//! @param[in] softMin минорная версия прошивки
forward stock isRomAvailable(devModel, softMaj, softMin);

//! @brief Проверить доступность функций отложенной записи тегов
//! @param[in] devModel модель прибора DEVINFO_MODEL_
//! @param[in] softMaj мажорная версия прошивки
//! @param[in] softMin минорная версия прошивки
forward stock isTagWriteBeginAvailable(devModel, softMaj, softMin);

//! @brief Проверить доступность тегов колесных датчиков
//! @param[in] devModel модель прибора DEVINFO_MODEL_
//! @param[in] softMaj мажорная версия прошивки
//! @param[in] softMin минорная версия прошивки
forward stock isWheelTagsAvailable(devModel, softMaj, softMin);

//! @brief Получить размеры свободной оперативной памяти в байтах
//! @param[out] firmware для основной прошивки и алгоритмов Easy Logic (в Galileosky 10 Pro - только для прошивки)
//! @param[out] zip для распаковки файлов и прошивок
//! @param[out] easyLogic для алгоритмов Easy Logic в Galileosky 10 Pro
//! @return true - количество полученных значений, false - ошибка (команда не поддерживается, изменился формат и пр.)
forward stock getFreeRam(&firmware, &zip = 0, &easyLogic = 0);

//! @brief Получить максимальный размер данных тега
forward stock getTagMaxSize(tagId);

//! @brief Получить значение числового тега по его идентификатору
//! @param[in] tagId идентификатор тега
//! @param[out] value полученное значение при успешном возврате
//! @return true - успешно, false - ошибка идентификатора тега
forward stock getTagValue(tagId, &value);

//! @brief Проверить наличие внешнего питания прибора
//! @param[in] devStatus значение статуса из глобальной переменной STATUS
forward stock hasExtPower(devStatus);

//! @brief Проверить, что двигатель заведен (== зажигание включено)
//! @param[in] devStatus значение статуса из глобальной переменной STATUS
forward stock isEngineOn(devStatus);

//! @brief Получить дискретный статус дискретно-аналогового входа
//! @param[in] index индекс входа
forward stock getInStatus(index);

//! @brief Проверить поддержку спецификации пользователя
//! @param[in] devModel модель прибора DEVINFO_MODEL_
//! @param[in] softMaj мажорная версия прошивки
//! @param[in] softMin минорная версия прошивки
forward stock hasUserSpec(devModel, softMaj, softMin);

//! @brief Получить спецификацию пользователя по ответу на команду
//! @note Предварительно нужно проверить доступность спецификации пользователя с помощью hasUserSpec()
//! @param[out] userSpec Полученная спецификация при успешном возврате
//! @return true - успешно, false - ошибка
forward stock getUserSpec(&userSpec);

//! @brief Проверить включение модуля прошивки по спецификации пользователя
//! @param[in] userSpec спецификация пользователя, можно получить с помощью getUserSpec()
//! @param[in] userSpecBit бит модуля @ref userspecBits
forward stock isModuleOn(userSpec, userSpecBit);
