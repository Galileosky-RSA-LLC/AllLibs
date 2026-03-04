#if !defined TPMS_LIB
#define TPMS_LIB
// библиотека систем контроля давления в шинах

#include "tpms.h"
#include "..\gdefines.h"
#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"

//! Записать в тег колесного датчика состояние шины
//! \param[in] wheelIdx индекс шины TPMS_WHEEL_INDEX_MIN..TPMS_WHEEL_INDEX_MAX
//! \param[in] wheel структура данных шины
tpmsWriteWheelTag(wheelIdx, const wheel[TPMS_WHEEL_DATA])
{
    coerce(wheelIdx, TPMS_WHEEL_INDEX_MIN, TPMS_WHEEL_INDEX_MAX);
    new pressure = wheel.pressurePsi;
    coerce(pressure, TPMS_PRESSURE_MIN, TPMS_PRESSURE_MAX);
    new reason = wheel.reason;
    coerce(reason, TPMS_REASON_MIN, TPMS_REASON_MAX);
    new temperature = wheel.temperature;
    coerce(temperature, TPMS_WHEELTAG_TEMP_MIN, TPMS_WHEELTAG_TEMP_MAX);
    new value = ((reason << TPMS_WHEELTAG_FLAGS_REASON_BIT) + ((!wheel.sensorOk << TPMS_WHEELTAG_FLAGS_ERROR_BIT) + !wheel.sensorOnline)
                << TPMS_WHEELTAG_FLAGS_LSHIFT)
                + ((temperature & TPMS_WHEELTAG_TEMP_MASK) << TPMS_WHEELTAG_TEMP_LSHIFT)
                + pressure;
    TagWriteValue(TAG_WHEEL_0 + wheelIdx, value);
}

//! Записать параметры шин в общий тег
//! \param[in] wheels параметры шин
tpmsWriteCommonTag(const wheels[TPMS_WHEELS_MAX][TPMS_WHEEL_DATA])
{
    new buf{TAG_TPMS_SIZE};
    for (new i = 0; i < TPMS_WHEELS_MAX; i++)
    {
        new pressure = wheels[i].pressurePsi;
        coerce(pressure, TPMS_PRESSURE_MIN, TPMS_PRESSURE_MAX);
        buf{i * TPMS_COMTAG_WHEEL_SIZE + TPMS_COMTAG_WHEEL_PRES_BYTE} = pressure;
        new temperature = wheels[i].temperature;
        coerce(temperature, TPMS_COMTAG_TEMP_MIN, TPMS_COMTAG_TEMP_MAX);
        temperature = (temperature - TPMS_COMTAG_TEMP_MIN) / TPMS_COMTAG_TEMP_STEP;
        new reason = wheels[i].reason;
        coerce(reason, TPMS_REASON_MIN, TPMS_REASON_MAX);
        buf{i * TPMS_COMTAG_WHEEL_SIZE + TPMS_COMTAG_WHEEL_TEMPFLAGS_BYTE} = (reason << TPMS_COMTAG_WHEEL_REASON_BIT)
                                                                            + (!wheels[i].sensorOk << TPMS_COMTAG_WHEEL_ERROR_BIT)
                                                                            + (!wheels[i].sensorOnline << TPMS_COMTAG_WHEEL_ONLINE_BIT)
                                                                            + temperature;
    }
    swapBuf(buf,TPMS_COMTAG_WHEELS_SIZE);
    buf{TPMS_COMTAG_SEND_FLAG_POS} = 1;
    TagWriteArray(TAG_TPMS, TAG_TPMS_SIZE, buf);
}

//! Очистить общий тег шин
tpmsClearCommonTag()
{
    new buf{TAG_TPMS_SIZE};
    for (new i = 0; i < TPMS_WHEELS_MAX; i++)
    {
        buf{i * TPMS_COMTAG_WHEEL_SIZE + TPMS_COMTAG_WHEEL_PRES_BYTE} = 0;
        buf{i * TPMS_COMTAG_WHEEL_SIZE + TPMS_COMTAG_WHEEL_TEMPFLAGS_BYTE} = TPMS_COMTAG_WHEEL_INIT_VALUE;
    }
    swapBuf(buf, TPMS_COMTAG_WHEELS_SIZE);
    TagWriteArray(TAG_TPMS, TAG_TPMS_SIZE, buf);
}

//! Инициализировать параметры шин
//! \param[inout] wheels параметры шин
tpmsInitWheels(wheels[TPMS_WHEELS_MAX][TPMS_WHEEL_DATA])
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

#endif
