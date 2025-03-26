#ifndef BLUETOOTH_LIB
#define BLUETOOTH_LIB

#include "bluetooth.h"
#include "..\array\array.h"
#include "..\array\array.cpp"

//! Получить AD Structure из рекламного сообщения
//! \param[in] msg полученное рекламное сообщение
//! \param[in] pos позиция начала структуры в данных рекламного сообщения
//! \param[out] adStruct AD структура
//! \return общий размер AD struct (0 - ошибка)
bluetoothGetAdStruct(const msg[BTMSG], pos, adStruct[BLUETOOTH_ADSTRUCT])
{
    if (pos < 0)
        pos = 0;

    new msgDataSize = min(msg.dataSize, BLUETOOTH_ADV_DATA_SIZE_MAX);
    if ((pos + BLUETOOTH_ADSTRUCT_SIZE_MIN) > msgDataSize)
        return 0;
    
    adStruct.dataSize = msg.data{pos + BLUETOOTH_ADSTRUCT_LEN_POS} - BLUETOOTH_ADSTRUCT_LEN_SIZE;
    if ((adStruct.dataSize < BLUETOOTH_ADSTRUCT_SIZE_MIN)
        || (insertArrayStr(adStruct.data, 0, BLUETOOTH_ADSTRUCT_DATA_SIZE_MAX, msg.data, msgDataSize, pos + BLUETOOTH_ADSTRUCT_PAYLOAD_POS)
            != adStruct.dataSize))
        return 0;

    adStruct.type = msg.data{pos + BLUETOOTH_ADSTRUCT_TYPE_POS};
    return BLUETOOTH_ADSTRUCT_SIZE_MIN + adStruct.dataSize;
}

//! Получить имя из AD Struct
//! \param[in] adStruct AD Struct с Local Name
//! \param[in] nameMaxSize максимальная длина для имени
//! \param[out] name полученное имя
//! \param[out] nameSize длина полученного имени
//! \return true - успешно, false - ошибка
bluetoothGetLocalName(const adStruct[BLUETOOTH_ADSTRUCT], nameMaxSize, name{}, &nameSize)
{
    if ((adStruct.type != BLUETOOTH_ADTYPE_SHORT_LOCAL_NAME) || (adStruct.type != BLUETOOTH_ADTYPE_COMPL_LOCAL_NAME))
        return false;

    nameSize = insertArrayStr(name, 0, nameMaxSize, adStruct.data, adStruct.dataSize);
    return nameSize > 0;
}

//! Получить RSSI устройства, передавшего сообщение
bluetoothGetRssi(const msg[BTMSG])
{
    return msg.rssi | BLUETOOTH_RSSI_MASK;
}

//! Получить Company Id из AD Struct
//! \param[in] adStruct AD Struct c Manufacturer Specific Data
//! \param[out] result полученный идентификатор
//! \return true - успешно, false - ошибка
bluetoothGetCompanyId(const adStruct[BLUETOOTH_ADSTRUCT], &result)
{
    return (adStruct.type == BLUETOOTH_ADTYPE_MFRSPECDATA) && array2num16leUnSign(adStruct.data, BLUETOOTH_MFRSPECDATA_COMP_ID_POS, adStruct.dataSize, result);
}

#endif
