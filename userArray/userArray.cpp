#ifndef USERARRAY_LIB
#define USERARRAY_LIB
// Библиотека массива пользователя

#include "userArray.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\checkcode\checkcode.h"
#include "..\checkcode\checkcode.cpp"
#include "..\gdefines.h"

//! Сформировать описатель для группы параметров массива пользователя универсальной структуры
//! \param[in] isNumbers признак числового типа данных (!=0 - числа, =0 - строка)
//! \param[in] amount количество элементов: для строки - длина в байтах (от 0 до 127), для чисел - от 1 до 8
//! \param[in] size размер памяти числа: 0, 1, 2, 4 или 8 (для строкового типа данных не учитывается)
ustructMakeDescriptor(isNumbers, amount, size)
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

//! Отправить файл с помощью массива пользователя
//! \param[in] fileName имя файла, должно оканчиваться \0
//! \param[inout] lastFileId идентификатор последнего отправленного файла
//! \return true - успешно, false - ошибка файловой системы: файл пустой или не прочитался
sendFileInUserArray(const fileName{}, &lastFileId)
{
    new fsize = FileSize(fileName);
    if (fsize <= 0)
        return false;

    new chunks = fsize / USERARRAY_FILE_CHUNK_LEN_MAX + 1;// последняя часть должна быть менее USERARRAY_FILE_CHUNK_LEN_MAX
    new data{USERARRAY_FILE_CHUNK_LEN_MAX};
    new sentFileNum = lastFileId + 1;
    new time = GetVar(UNIX_TIME);
    new uid = GetVar(ARE_COORDINATES_VALID) ? time : sentFileNum;
    for (new i = 0; i < chunks; i++)
    {
        new bytesRead = FileRead(fileName, data, USERARRAY_FILE_CHUNK_LEN_MAX, i * USERARRAY_FILE_CHUNK_LEN_MAX);
        if ((bytesRead != USERARRAY_FILE_CHUNK_LEN_MAX) && (bytesRead != (fsize - i * USERARRAY_FILE_CHUNK_LEN_MAX)))
            return false;

        new userAr{USERARRAY_MAX_SIZE} = {USERARRAY_FILE_TYPE};
        num2array(uid, USERARRAY_FILE_UID_LEN, true, userAr, USERARRAY_FILE_UID_POS, USERARRAY_MAX_SIZE);
        num2array(i, USERARRAY_FILE_NCHUNK_LEN, true, userAr, USERARRAY_FILE_NCHUNK_POS, USERARRAY_MAX_SIZE);
        userAr{USERARRAY_FILE_SIZE_POS} = bytesRead;
        insertArrayStr(userAr, USERARRAY_FILE_DATA_POS, USERARRAY_MAX_SIZE, data, bytesRead);
        new userArSize = USERARRAY_FILE_FRAME_LEN_MIN + bytesRead;
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

//! Сформировать описатель группы чисел в массиве пользователя универсальной структуры
//! \param[in] amount количество чисел, 1..8
//! \param[in] size размер памяти числа: 1, 2, 4 или 8 байт
ustructMakeDescriptorNum(amount, size)
{
    return ustructMakeDescriptor(true, amount, size < 1 ? 1 : size);
}

//! Сформировать описатель строки в массиве пользователя универсальной структуры
//! \param[in] strSize длина строки в байтах, 0..127
ustructMakeDescriptorStr(strSize)
{
    return ustructMakeDescriptor(false, strSize, 0);
}

//! Пропустить неиспользуемые параметры в массиве пользователя универсальной структуры
//! \param[inout] userArray массив пользователя
//! \param[in] userArrayMaxSize предельный размер массива пользователя
//! \param[inout] pos позиция в массиве пользователя
//! \param[in] emptyCount количество пропускаемых параметров
//! \return количество вставленных байт (0 - ошибка)
ustructInsertEmpty(userArray{}, userArrayMaxSize, &pos, emptyCount)
{
    new groups = (emptyCount / USERARRAY_USTRUCT_DESCR_NUMBERS_MAX) + ((emptyCount % USERARRAY_USTRUCT_DESCR_NUMBERS_MAX) != 0);
    if ((pos < 0) || ((pos + groups) > userArrayMaxSize))
        return 0;
    
    for (new i = 0; i < groups; i++)
        userArray{pos++} = ustructMakeDescriptor(true, min(emptyCount - i * USERARRAY_USTRUCT_DESCR_NUMBERS_MAX, USERARRAY_USTRUCT_DESCR_NUMBERS_MAX), 0);

    return groups;
}

//! Вставить данные датчика пассажиропотока в массив пользователя
//! \param[inout] userArray массив пользователя
//! \param[in] userArrayMaxSize предельный размер массива пользователя
//! \param[in] sensor данные датчика
//! \param[inout] pos позиция в массиве пользователя
//! \return количество вставленных байт (0 - ошибка входных данных)
userArrayAddPasCounting(userArray{}, userArrayMaxSize, const sensor[USERARRAY_PASCOUNT_SENSOR_DATA], &pos)
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

#endif // USERARRAY_LIB
