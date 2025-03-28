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

//! Получить имя
//! \param[in] msg полученное рекламное сообщение
//! \param[in] nameMaxSize максимальная длина для имени
//! \param[out] name полученное имя
//! \param[out] nameSize длина полученного имени
//! \return true - успешно, false - ошибка
bluetoothGetLocalName(const msg[BTMSG], nameMaxSize, name{}, &nameSize = 0)
{
    return bluetoothGetComplName(msg, nameMaxSize, name, nameSize) || bluetoothGetShortName(msg, nameMaxSize, name, nameSize);
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

bluetoothGetComplName(const msg[BTMSG], nameMaxSize, name{}, &nameSize = 0)
{
    return bluetooth_getLocalName(msg, true, nameMaxSize, name, nameSize);
}

bluetoothGetShortName(const msg[BTMSG], nameMaxSize, name{}, &nameSize = 0)
{
    return bluetooth_getLocalName(msg, false, nameMaxSize, name, nameSize);
}

//! Получить AD структуру заданного типа из рекламного сообщения
//! \param[in] msg полученное рекламное сообщение
//! \param[in] type искомый тип структуры
//! \param[out] adStruct найденная AD структура
//! \return true - найдена, false - не найдена
bluetoothGetAdStructType(const msg[BTMSG], type, adStruct[BLUETOOTH_ADSTRUCT])
{
    new size;
    for (new pos = 0; (pos < msg.dataSize) && ((size = bluetoothGetAdStruct(msg, pos, adStruct)) > 0); pos += size)
    {
        if (adStruct.type == type)
            return true;
    }
    return false;
}

// Приватные функции

bluetooth_getLocalName(const msg[BTMSG], isComplName, nameMaxSize, name{}, &nameSize = 0)
{
    new adStruct[BLUETOOTH_ADSTRUCT];
    if (!bluetoothGetAdStructType(msg, isComplName ? BLUETOOTH_ADTYPE_COMPL_LOCAL_NAME : BLUETOOTH_ADTYPE_SHORT_LOCAL_NAME, adStruct))
        return false;

    nameSize = insertArrayStr(name, 0, nameMaxSize, adStruct.data, adStruct.dataSize);
    return nameSize > 0;
}

#endif
