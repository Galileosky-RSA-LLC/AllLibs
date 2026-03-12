//! @file
//! @brief Реализация библиотеки систем контроля давления в шинах

#if defined TPMS_LIB
#endinput
#endif
#define TPMS_LIB

#include "tpms.h"
#include "..\gdefines.h"
#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"

//! @defgroup wheelTag Структура тега колесного датчика
//! @{
#define TPMS_WHEELTAG_PRES_BYTE 0
#define TPMS_WHEELTAG_TEMP_BYTE 1
#define TPMS_WHEELTAG_TEMP_MIN -128
#define TPMS_WHEELTAG_TEMP_MAX 127
#define TPMS_WHEELTAG_TEMP_MASK 0xFF
#define TPMS_WHEELTAG_TEMP_LSHIFT (TPMS_WHEELTAG_TEMP_BYTE * 8)
#define TPMS_WHEELTAG_FLAGS_BYTE 2
#define TPMS_WHEELTAG_FLAGS_LSHIFT (TPMS_WHEELTAG_FLAGS_BYTE * 8)
#define TPMS_WHEELTAG_FLAGS_ONLINE_BIT 0
#define TPMS_WHEELTAG_FLAGS_ERROR_BIT 1
#define TPMS_WHEELTAG_FLAGS_REASON_BIT 2
//! @}

//! @defgroup commonTag Структура общего тега со всеми шинами
//! @{
#define TPMS_COMTAG_WHEEL_PRES_BYTE 0
#define TPMS_COMTAG_WHEEL_PRES_SIZE 1
#define TPMS_COMTAG_WHEEL_TEMPFLAGS_BYTE (TPMS_COMTAG_WHEEL_PRES_BYTE + TPMS_COMTAG_WHEEL_PRES_SIZE)
#define TPMS_COMTAG_WHEEL_TEMPFLAGS_SIZE 1
#define TPMS_COMTAG_WHEEL_TEMP_BIT 0
#define TPMS_COMTAG_WHEEL_TEMP_BITS 3
#define TPMS_COMTAG_WHEEL_TEMP_MASK 0xFF
#define TPMS_COMTAG_TEMP_MIN -40
#define TPMS_COMTAG_TEMP_MAX 100
#define TPMS_COMTAG_TEMP_STEP ((TPMS_COMTAG_TEMP_MAX - TPMS_COMTAG_TEMP_MIN) / ((1 << TPMS_COMTAG_WHEEL_TEMP_BITS) - 1))
#define TPMS_COMTAG_WHEEL_ONLINE_BIT (TPMS_COMTAG_WHEEL_TEMP_BIT + TPMS_COMTAG_WHEEL_TEMP_BITS)
#define TPMS_COMTAG_WHEEL_ERROR_BIT TPMS_COMTAG_WHEEL_ONLINE_BIT + 1
#define TPMS_COMTAG_WHEEL_REASON_BIT TPMS_COMTAG_WHEEL_ERROR_BIT + 1
#define TPMS_COMTAG_WHEEL_INIT_VALUE ((TPMS_REASON_PERIODIC << TPMS_COMTAG_WHEEL_REASON_BIT) \
                                    + (1 << TPMS_COMTAG_WHEEL_ERROR_BIT) \
                                    + (1 << TPMS_COMTAG_WHEEL_ONLINE_BIT))
#define TPMS_COMTAG_WHEEL_SIZE (TPMS_COMTAG_WHEEL_PRES_SIZE + TPMS_COMTAG_WHEEL_TEMPFLAGS_SIZE)
#define TPMS_COMTAG_WHEELS_SIZE (TPMS_WHEELS_MAX * TPMS_COMTAG_WHEEL_SIZE)
#define TPMS_COMTAG_SEND_FLAG_POS (TAG_TPMS_SIZE - 1)
//! @}

stock tpmsWriteWheelTag(wheelIdx, const wheel[TPMS_WHEEL_DATA])
{
    coerce(wheelIdx, TPMS_WHEEL_INDEX_MIN, TPMS_WHEEL_INDEX_MAX);
    new pressure = wheel.pressurePsi;
    coerce(pressure, TPMS_PRESSURE_MIN, TPMS_PRESSURE_MAX);
    new reason = wheel.reason;
    coerce(reason, TPMS_REASON_MIN, TPMS_REASON_MAX);
    new temperature = wheel.temperature;
    coerce(temperature, TPMS_WHEELTAG_TEMP_MIN, TPMS_WHEELTAG_TEMP_MAX);
    new const value = ((reason << TPMS_WHEELTAG_FLAGS_REASON_BIT)
                        + (((!wheel.sensorOk ? 1 : 0) << TPMS_WHEELTAG_FLAGS_ERROR_BIT) + (!wheel.sensorOnline ? 1 : 0)) << TPMS_WHEELTAG_FLAGS_LSHIFT)
                        + ((temperature & TPMS_WHEELTAG_TEMP_MASK) << TPMS_WHEELTAG_TEMP_LSHIFT)
                        + pressure;
    TagWriteValue(TAG_WHEEL_0 + wheelIdx, value);
}

stock tpmsWriteCommonTag(const wheels[TPMS_WHEELS_MAX][TPMS_WHEEL_DATA])
{
    new buf{TAG_TPMS_SIZE};
    for (new i = 0; i < TPMS_WHEELS_MAX; i++)
    {
        new pressure = wheels[i].pressurePsi;
        coerce(pressure, TPMS_PRESSURE_MIN, TPMS_PRESSURE_MAX);
        new const wheelOffset = i * TPMS_COMTAG_WHEEL_SIZE;
        buf{wheelOffset + TPMS_COMTAG_WHEEL_PRES_BYTE} = pressure;
        new temperature = wheels[i].temperature;
        coerce(temperature, TPMS_COMTAG_TEMP_MIN, TPMS_COMTAG_TEMP_MAX);
        temperature = (temperature - TPMS_COMTAG_TEMP_MIN) / TPMS_COMTAG_TEMP_STEP;
        new reason = wheels[i].reason;
        coerce(reason, TPMS_REASON_MIN, TPMS_REASON_MAX);
        buf{wheelOffset + TPMS_COMTAG_WHEEL_TEMPFLAGS_BYTE} = (reason << TPMS_COMTAG_WHEEL_REASON_BIT)
                                                                + ((!wheels[i].sensorOk ? 1 : 0) << TPMS_COMTAG_WHEEL_ERROR_BIT)
                                                                + ((!wheels[i].sensorOnline ? 1 : 0) << TPMS_COMTAG_WHEEL_ONLINE_BIT)
                                                                + temperature;
    }
    swapBuf(buf,TPMS_COMTAG_WHEELS_SIZE);
    buf{TPMS_COMTAG_SEND_FLAG_POS} = 1;
    TagWriteArray(TAG_TPMS, TAG_TPMS_SIZE, buf);
}

stock tpmsClearCommonTag()
{
    new buf{TAG_TPMS_SIZE};
    for (new i = 0; i < TPMS_WHEELS_MAX; i++)
    {
        new const wheelOffset = i * TPMS_COMTAG_WHEEL_SIZE;
        buf{wheelOffset + TPMS_COMTAG_WHEEL_PRES_BYTE} = 0;
        buf{wheelOffset + TPMS_COMTAG_WHEEL_TEMPFLAGS_BYTE} = TPMS_COMTAG_WHEEL_INIT_VALUE;
    }
    swapBuf(buf, TPMS_COMTAG_WHEELS_SIZE);
    TagWriteArray(TAG_TPMS, TAG_TPMS_SIZE, buf);
}

stock tpmsInitWheels(wheels[TPMS_WHEELS_MAX][TPMS_WHEEL_DATA])
{
    for (new i = 0; i < TPMS_WHEELS_MAX; i++)
    {
        wheels[i].temperature = TPMS_COMTAG_TEMP_MIN;
        wheels[i].pressurePsi = 0;
        wheels[i].sensorOnline = false;
        wheels[i].sensorOk = false;
        wheels[i].reason = TPMS_REASON_PERIODIC;
    }
}
