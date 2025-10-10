#ifdef BLUETOOTH_LIB
#endinput
#endif
#define BLUETOOTH_LIB

//! @file
//! @brief Реализация библиотеки для работы с Bluetooth

#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"
#include "bluetooth.h"

//! @publicsection

stock bluetoothGetAdStruct(const msg[BTMSG], pos, adStruct[BLUETOOTH_ADSTRUCT])
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

stock bluetoothGetLocalName(const msg[BTMSG], nameMaxSize, name{})
{
    return bluetoothGetComplName(msg, nameMaxSize, name) || bluetoothGetShortName(msg, nameMaxSize, name);
}

stock bluetoothGetRssi(const msg[BTMSG])
{
    return msg.rssi | BLUETOOTH_RSSI_MASK;
}

stock bluetoothGetCompanyId(const adStruct[BLUETOOTH_ADSTRUCT], &result)
{
    return (adStruct.type == BLUETOOTH_ADTYPE_MFRSPECDATA) && array2num16leUnSign(adStruct.data, BLUETOOTH_MFRSPECDATA_COMP_ID_POS, adStruct.dataSize, result);
}

stock bluetoothGetComplName(const msg[BTMSG], nameMaxSize, name{})
{
    return bluetooth_getLocalName(msg, true, nameMaxSize, name);
}

stock bluetoothGetShortName(const msg[BTMSG], nameMaxSize, name{})
{
    return bluetooth_getLocalName(msg, false, nameMaxSize, name);
}

stock bluetoothGetAdStructType(const msg[BTMSG], type, adStruct[BLUETOOTH_ADSTRUCT])
{
    new size;
    for (new pos = 0; (pos < msg.dataSize) && ((size = bluetoothGetAdStruct(msg, pos, adStruct)) > 0); pos += size)
    {
        if (adStruct.type == type)
            return true;
    }
    return false;
}

//! @privatesection

stock bluetooth_getLocalName(const msg[BTMSG], isComplName, nameMaxSize, name{})
{
    new adStruct[BLUETOOTH_ADSTRUCT];
    if (!bluetoothGetAdStructType(msg, isComplName ? BLUETOOTH_ADTYPE_COMPL_LOCAL_NAME : BLUETOOTH_ADTYPE_SHORT_LOCAL_NAME, adStruct))
        return false;

    strncpy(name, 0, nameMaxSize, adStruct.data);
    return strLen(adStruct.data, adStruct.dataSize) == strLen(name, nameMaxSize);
}
