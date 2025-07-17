#ifndef CHECKCODE_LIB
#define CHECKCODE_LIB
// Функции вычисления проверочных кодов (CRC, CS, etc.)

// Публичные функции

//! CRC8
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
crc8(const data{}, length, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new res;
    for (; pos < length; pos++)
    {
        new t = data{pos};
        for (new j = 0; j < 8; j++)
        {
            res ^= t & 0x01;
            if (res & 0x01)
            {
                res ^= 0x18;
                res >>>= 1;
                res |= 0x80;
            }
            else
            {
                res >>>= 1;
            }
            t >>>= 1;
        }
    }
    return res;
}

//! LRC8
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
lrc8(const data{}, length, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new res;
    for (; pos < length; pos++)
        res += data{pos};

    res = 0xFF - res + 1;
    return res & 0xFF;
}

//! CRC-KERMIT
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение при инициализации
crc16Kermit(const data{}, length, pos = 0, init = 0)
{
    if (pos < 0)
        pos = 0;

    new crc = init;
    for (; pos < length; pos++)
    {
        crc ^= data{pos};
        for (new j = 0; j < 8; j++)
            crc = (crc & 1) == 1 ? (crc >>> 1) ^ 0x8408 : crc >>> 1;
    }
    return crc;
}

//! Контрольная сумма Флетчера 
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
fletcher16(const data{}, length, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new sum1 = 0;
    new sum2 = 0;
    for(; pos < length; pos++)
    {
        sum1 = (sum1 + data{pos}) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
   return (sum2 << 8) | sum1;
}

//! Контрольная сумма Флетчера 
//! оптимизированная
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
fletcher16opt(const data{}, length, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new c0;
    new c1;
    new len = length - pos;
    const blockSize = 5802;
    for (; len >= blockSize; len -= blockSize)
        fletcher16opt_calcBlock(data, pos, length, blockSize, c0, c1);
    
    fletcher16opt_calcBlock(data, pos, length, len, c0, c1);
    return c1 << 8 | c0;
}

//! Poly  : 0x1021    x^16 + x^12 + x^5 + 1
//!  Revert: false
//!  XorOut: 0x0000
//!  Check : 0x29B1 ("123456789")
//! MaxLen: 4095 байт (32767 бит) - обнаружение одинарных, двойных, тройных и всех нечетных ошибок
//! по окончании расчета взять младшие 2 байта
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение при инициализации
crc16ccitt(const data{}, length, pos = 0, init = 0xFFFF)
{
    if (pos < 0)
        pos = 0;

    new crc = init;
    for (; pos < length; pos++)
    {
        crc ^= data{pos} << 8;
        for (new i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

//! CRC-8-Dallas/Maxim
//! Poly: 0x31 (x^8 + x^5 + x^4 + 1)
//! init: 0xFF
//! Check: 0xF7 ("123456789")
//! \param[in] data массив с данными для расчета
//! \param[in] dataSize размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение инициализации
crc8dallasMaxim(const data{}, dataSize, pos = 0, init = 0xFF)
{
    if (pos < 0)
        pos = 0;

    new res = init;
    for (; pos < dataSize; pos++)
    {
        res ^= data{pos};
        for (new i = 0; i < 8; i++)
            res = res & 0x80 ? (res << 1) ^ 0x31 : res << 1;
    }
    return res;
}

//! Poly: 0x04C11DB7 (x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1)
//! Init  : 0xFFFFFFFF
//! Revert: true
//! XorOut: 0xFFFFFFFF
//! Check : 0xCBF43926 ("123456789")
//! MaxLen: 268 435 455 байт (2 147 483 647 бит) - обнаружение одинарных, двойных, пакетных и всех нечетных ошибок
//! \param[in] data массив с данными для расчета
//! \param[in] dataSize размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение инициализации
crc32(const data{}, dataSize, pos = 0, init = 0xFFFFFFFF)
{
    new res = init;
    for (; pos < dataSize; pos++)
    {
        res ^= data{pos};
        for (new j = 0; j < 8; j++)
            res = (res & 1) ? (res >>> 1) ^ 0xEDB88320 : res >>> 1;
    }
    return res ^ 0xFFFFFFFF;
}

//! Poly: 0x8005 (x^16 + x^15 + x^2 + 1)
//! Init  : 0x0000
//! RefIn: false
//! RefOut: false
//! XorOut: 0x0000
//! Check : 0xFEE8 ("123456789")
//! \param[in] data массив с данными для расчета
//! \param[in] dataSize размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение инициализации
stock crc16buypass(const data{}, dataSize, pos = 0, init = 0x0000)
{
    const poly = 0x8005;
    new res = init;
    for (; pos < dataSize; pos++)
    {
        res ^= data{pos} << 8;
        for(new i = 0; i < 8; i++)
            res = (res & (1 << 15)) ? ((res << 1) ^ poly) : (res << 1);
    }
    return res;
}

// Приватные функции

//! Расчет значений в блоке для fletcher16opt
//! \param[in] data массив для вычисления
//! \param[inout] pos начальная позиция
//! \param[in] dataLength размер массива
//! \param[in] blockSize размер блока для вычисления
//! \param[inout] c0 0 байт рассчитанного значения
//! \param[inout] c1 1 байт рассчитанного значения
fletcher16opt_calcBlock(const data{}, &pos, dataLength, blockSize, &c0, &c1)
{
    for (new i = 0; (i < blockSize) && (pos < dataLength); i++)
    {
        c0 += data{pos++};
        c1 += c0;
    }
    c0 %= 255;
    c1 %= 255;
}

#endif // CHECKCODE_LIB