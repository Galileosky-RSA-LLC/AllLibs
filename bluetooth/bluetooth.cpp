#ifndef BLUETOOTH_LIB
#define BLUETOOTH_LIB

#include "bluetooth.h"
#include "..\array\array.h"
#include "..\array\array.cpp"

//! Получить AD Structure из рекламного сообщения
//! \param[in] msg принятое рекламное сообщение
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
    
    new len = msg.data{pos + BLUETOOTH_ADSTRUCT_LEN_POS};
    new adStructLen = len + BLUETOOTH_ADSTRUCT_LEN_SIZE;
    if ((len < BLUETOOTH_ADSTRUCT_LEN_MIN) || ((pos + adStructLen) > msgDataSize))
        return 0;

    adStruct.dataSize = len - BLUETOOTH_ADSTRUCT_LEN_SIZE;
    if (insertArrayStr(adStruct.data, 0, adStruct.dataSize, msg.data, msgDataSize, pos + BLUETOOTH_ADSTRUCT_PAYLOAD_POS) != adStruct.dataSize)
        return 0;

    adStruct.type = msg.data{pos + BLUETOOTH_ADSTRUCT_TYPE_POS};
    return adStructLen;
}

//! Получить имя
//! \param[in] msg принятое рекламное сообщение
//! \param[in] nameMaxSize максимальная длина для имени
//! \param[out] name полученное имя
//! \return true - успешно, false - ошибка
bluetoothGetLocalName(const msg[BTMSG], nameMaxSize, name{})
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

//! Получить полное имя
//! \param[in] msg принятое рекламное сообщение
//! \param[in] nameMaxSize максимальная длина для имени
//! \param[out] name полученное имя
//! \return true - успешно, false - ошибка
bluetoothGetComplName(const msg[BTMSG], nameMaxSize, name{})
{
    return bluetooth_getLocalName(msg, true, nameMaxSize, name);
}

//! Получить короткое имя
//! \param[in] msg принятое рекламное сообщение
//! \param[in] nameMaxSize максимальная длина для имени
//! \param[out] name полученное имя
//! \return true - успешно, false - ошибка
bluetoothGetShortName(const msg[BTMSG], nameMaxSize, name{})
{
    return bluetooth_getLocalName(msg, false, nameMaxSize, name);
}

//! Получить AD структуру заданного типа из рекламного сообщения
//! \param[in] msg принятое рекламное сообщение
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

bluetooth_getLocalName(const msg[BTMSG], isComplName, nameMaxSize, name{})
{
    new adStruct[BLUETOOTH_ADSTRUCT];
    if (!bluetoothGetAdStructType(msg, isComplName ? BLUETOOTH_ADTYPE_COMPL_LOCAL_NAME : BLUETOOTH_ADTYPE_SHORT_LOCAL_NAME, adStruct))
        return false;

    new nameSize = insertArrayStr(name, 0, nameMaxSize, adStruct.data, adStruct.dataSize);
    if ((nameSize >= 0) && (nameSize < nameMaxSize))
        name{nameSize} = 0;
        
    return nameSize > 0;
}

#endif
