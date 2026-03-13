//! @file
//! @brief Заголовок библиотеки систем контроля давления в шинах

#if defined TPMS_H
#endinput
#endif
#define TPMS_H

#define TPMS_WHEELS_MAX 34
#define TPMS_WHEEL_INDEX_MIN 0
#define TPMS_WHEEL_INDEX_MAX (TPMS_WHEELS_MAX - 1)

//! @defgroup reasons Причины отправки данных
//! @{
#define TPMS_REASON_BITS 3

#define TPMS_REASON_PERIODIC 0
#define TPMS_REASON_PRES_UNDER_10 1 //!< PressurePro: 10%, TPMS: 12,5%
#define TPMS_REASON_PRES_UNDER_20 2 //!< PressurePro: 20%, TPMS: 25%
#define TPMS_REASON_PRES_UNDER_50 3
#define TPMS_REASON_OVERTEMP 4
#define TPMS_REASON_PRES_FAST_DEC 5
#define TPMS_REASON_NEWAIR_OVERPRES 6 //!< PressurePro: new air, TPMS: overpressure
#define TPMS_REASON_NEWMAGNET 7

#define TPMS_REASON_MIN TPMS_REASON_PERIODIC
#define TPMS_REASON_MAX TPMS_REASON_NEWMAGNET
//! @}

//! @defgroup pressure Давление
//! @{
#define TPMS_PRESSURE_MIN 0
#define TPMS_PRESSURE_MAX 255
//! @}

#define TPMS_WHEEL_DATA [ \
    .temperature,\
    .pressurePsi,\
    bool:.sensorOnline,\
    bool:.sensorOk,\
    .reason,\
]

//! @brief Инициализировать параметры шин
//! @param[out] wheels массив структур шин
forward stock tpmsInitWheels(wheels[TPMS_WHEELS_MAX][TPMS_WHEEL_DATA]);

//! @brief Записать параметры шин в общий тег
//! @param[in] wheels массив структур шин
forward stock tpmsWriteCommonTag(const wheels[TPMS_WHEELS_MAX][TPMS_WHEEL_DATA]);

//! @brief Очистить общий тег шин
forward stock tpmsClearCommonTag();

//! @brief Записать в тег колесного датчика состояние шины
//! @param[in] wheelIdx индекс шины TPMS_WHEEL_INDEX_MIN..TPMS_WHEEL_INDEX_MAX
//! @param[in] wheel структура данных шины
forward stock tpmsWriteWheelTag(wheelIdx, const wheel[TPMS_WHEEL_DATA]);
