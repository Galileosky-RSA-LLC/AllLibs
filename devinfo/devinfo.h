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
#define DEVINFO_STATUS_MOTION_BIT 0
#define DEVINFO_STATUS_INCLINE_BIT 1
#define DEVINFO_STATUS_IBUTTON_BIT 2
#define DEVINFO_STATUS_NOSIM_BIT 3
#define DEVINFO_STATUS_GEOFENCE_BIT 4
#define DEVINFO_STATUS_UINTLOW_BIT 5
#define DEVINFO_STATUS_GPSANTOFF_BIT 6
#define DEVINFO_STATUS_UINTBUSFAIL_BIT 7
#define DEVINFO_STATUS_EXTPOWERFAIL_BIT 8
#define DEVINFO_STATUS_ENGINEON_BIT 9
#define DEVINFO_STATUS_SHOCK_BIT 10
#define DEVINFO_STATUS_COORDS_BIT 11
#define DEVINFO_STATUS_SIGNALING_BIT 14
#define DEVINFO_STATUS_ALARM_BIT 15
#define DEVINFO_STATUS_MAINSRV_BIT 16
#define DEVINFO_STATUS_GPRS_BIT 17
#define DEVINFO_STATUS_GSMJAMMING_BIT 18
#define DEVINFO_STATUS_SECONDSRV_BIT 19
#define DEVINFO_STATUS_GNSSJAMMING_BIT 20
#define DEVINFO_STATUS_USB_BIT 21
#define DEVINFO_STATUS_SD_BIT 22
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
