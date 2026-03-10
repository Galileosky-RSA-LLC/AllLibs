//! @file
//! @brief Функции библиотеки массива пользователя

#if defined USERARRAY_LIB
#endinput
#endif
#define USERARRAY_LIB

#include "userArray.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\checkcode\checkcode.h"
#include "..\checkcode\checkcode.cpp"
#include "..\gdefines.h"
#include "..\string\string.h"
#include "..\string\string.cpp"

stock ustructMakeDescriptor(isNumbers, amount, size)
{
    if (isNumbers)
    {    
        if (amount > USERARRAY_USTRUCT_DESCR_NUMBERS_MAX)
            amount = USERARRAY_USTRUCT_DESCR_NUMBERS_MAX;

        if (amount < 1)
            amount = 1;

        if (size < 0)
            size = 0;
        else if (size == 3)
            size = 4;
        else if ((size > 4) && (size != 8))
            size = 8;
    }
    else
    {
        if (amount > USERARRAY_USTRUCT_DESCR_SYMBOLS_MAX)
            amount = USERARRAY_USTRUCT_DESCR_SYMBOLS_MAX;

        if (amount < 0)
            amount = 0;
    }
    return ((isNumbers == 0) << 7) + (isNumbers ? ((amount - 1) << 4) + size : amount);
}

stock bool:sendFileInUserArray(const fileName{}, &lastFileId)
{
    new const fsize = FileSize(fileName);
    if (fsize < 0)
        return false;

    new const chunks = (fsize / USERARRAY_FILE_CHUNK_LEN_MAX) + 1;// последняя часть должна быть менее USERARRAY_FILE_CHUNK_LEN_MAX
    new data{USERARRAY_FILE_CHUNK_LEN_MAX};
    new const uid = GetVar(ARE_COORDINATES_VALID) ? GetVar(UNIX_TIME) : lastFileId + 1;
    for (new i = 0; i < chunks; i++)
    {
        new const savedSize = i * USERARRAY_FILE_CHUNK_LEN_MAX;
        new const bytesRead = FileRead(fileName, data, USERARRAY_FILE_CHUNK_LEN_MAX, savedSize);
        if ((bytesRead != USERARRAY_FILE_CHUNK_LEN_MAX) && (bytesRead != (fsize - savedSize)))
            return false;

        new userAr{USERARRAY_MAX_SIZE} = {USERARRAY_FILE_TYPE};
        num2array(uid, USERARRAY_FILE_UID_LEN, true, userAr, USERARRAY_FILE_UID_POS, USERARRAY_MAX_SIZE);
        num2array(i, USERARRAY_FILE_NCHUNK_LEN, true, userAr, USERARRAY_FILE_NCHUNK_POS, USERARRAY_MAX_SIZE);
        userAr{USERARRAY_FILE_SIZE_POS} = bytesRead;
        insertArrayStr(userAr, USERARRAY_FILE_DATA_POS, USERARRAY_MAX_SIZE, data, bytesRead);
        new const userArSize = USERARRAY_FILE_FRAME_LEN_MIN + bytesRead;
        userAr{USERARRAY_FILE_DATA_POS + bytesRead} = crc8dallasMaxim(userAr, userArSize - 1) & 0xFF;
        TagWriteArray(TAG_USER_ARRAY, userArSize, userAr);
        SavePoint();
        TagWriteArray(TAG_USER_ARRAY, 0, userAr);
        const interChunksWaitMs = 10;
        Delay(interChunksWaitMs);
    }
    lastFileId = uid;
    return true;
}

stock ustructMakeDescriptorNum(amount, size)
{
    return ustructMakeDescriptor(true, amount, size < 1 ? 1 : size);
}

stock ustructMakeDescriptorStr(strSize)
{
    return ustructMakeDescriptor(false, strSize, 0);
}

stock ustructInsertEmpty(userArray{}, userArrayMaxSize, &pos, emptyCount)
{
    new groups = (emptyCount / USERARRAY_USTRUCT_DESCR_NUMBERS_MAX) + ((emptyCount % USERARRAY_USTRUCT_DESCR_NUMBERS_MAX) != 0);
    if ((pos < 0) || ((pos + groups) > userArrayMaxSize))
        return 0;
    
    for (new i = 0; i < groups; i++)
        userArray{pos++} = ustructMakeDescriptor(true, min(emptyCount - i * USERARRAY_USTRUCT_DESCR_NUMBERS_MAX, USERARRAY_USTRUCT_DESCR_NUMBERS_MAX), 0);

    return groups;
}

stock userArrayAddPasCounting(userArray{}, userArrayMaxSize, const sensor[USERARRAY_PASCOUNT_SENSOR_DATA], &pos)
{
    if ((pos < USERARRAY_PASCOUNT_DATA_POS_MIN)
        || ((pos + USERARRAY_PASCOUNT_SENSOR_SIZE) > USERARRAY_PASCOUNT_SIZE_MAX)
        || ((pos - USERARRAY_PASCOUNT_DATA_POS_MIN) % USERARRAY_PASCOUNT_SENSOR_SIZE)
        || (sensor.addr < USERARRAY_PASCOUNT_SENSOR_ADDR_MIN) || (sensor.addr > USERARRAY_PASCOUNT_SENSOR_ADDR_MAX)
        || (sensor.incoming < USERARRAY_PASCOUNT_SENSOR_DATA_MIN) || (sensor.incoming > USERARRAY_PASCOUNT_SENSOR_DATA_MAX)
        || (sensor.outgoing < USERARRAY_PASCOUNT_SENSOR_DATA_MIN) || (sensor.outgoing > USERARRAY_PASCOUNT_SENSOR_DATA_MAX))
        return 0;

    num24bit2arrayLe(sensor.addr, userArray, pos, userArrayMaxSize);
    pos += USERARRAY_PASCOUNT_SENSOR_ADDR_SIZE;
    userArray{pos++} = sensor.state;
    num16bit2arrayLe(sensor.incoming, userArray, pos, userArrayMaxSize);
    pos += USERARRAY_PASCOUNT_SENSOR_DATA_SIZE;
    num16bit2arrayLe(sensor.outgoing, userArray, pos, userArrayMaxSize);
    pos += USERARRAY_PASCOUNT_SENSOR_DATA_SIZE;
    return USERARRAY_PASCOUNT_SENSOR_SIZE;
}

stock setTextInUserArray(const text{})
{
    new userArray{USERARRAY_MAX_SIZE} = {USERARRAY_TEXT_TYPE};
    new size = USERARRAY_TEXT_TYPE_LEN + strncpy(userArray, USERARRAY_MAX_SIZE, text, USERARRAY_TEXT_PAYLOAD_POS);
    TagWriteArray(TAG_USER_ARRAY, size, userArray);
}

stock ustructInsertNumbers(userArray{}, userArrayMaxSize, &pos, dimension, const numbers[], numbersSize = 1)
{
    if ((pos < USERARRAY_USTRUCT_TYPE_SIZE) || (numbersSize < 1) || (dimension < 1) || (dimension == 3) || (dimension > 4)
        || ((pos + countUsedCells(numbersSize, USERARRAY_USTRUCT_DESCR_NUMBERS_MAX) + (dimension * numbersSize)) > userArrayMaxSize))
        return 0;

    new startPos = pos;
    new numbersPos = 0;
    while ((numbersPos < numbersSize) && (pos < userArrayMaxSize))
    {
        new amount = min(numbersSize - numbersPos, USERARRAY_USTRUCT_DESCR_NUMBERS_MAX);
        userArray{pos++} = ustructMakeDescriptorNum(amount, dimension);
        for (new i = 0; i < amount; i++)
        {
            num2array(numbers[numbersPos], dimension, true, userArray, pos, userArrayMaxSize);
            numbersPos++;
            pos += dimension;
        }
    }
    return pos - startPos;
}
