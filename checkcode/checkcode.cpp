//! @file
//! @brief Функции библиотеки проверочных кодов (CRC, CS, etc.)

#ifdef CHECKCODE_LIB
#endinput
#endif
#define CHECKCODE_LIB

#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"

//! @publicsection

stock crc8(const data{}, dataSize, pos = 0, init = 0x00, bool:isFinish = true)
{
    return crc(data, dataSize, .polyWidth = 8, .poly = 0x31, .pos = pos, .init = init, .isRevert = true, .xorOut = 0, .isFinish = isFinish);
}

stock lrc8(const data{}, dataSize, pos = 0, init = 0x00)
{
    if (pos < 0)
        pos = 0;

    new res = init;
    for (; pos < dataSize; pos++)
        res -= data{pos};

    return res;
}

stock crc16Kermit(const data{}, dataSize, pos = 0, init = 0, bool:isFinish = true)
{
    return crc(data, dataSize, .polyWidth = 16, .poly = 0x1021, .pos = pos, .init = init, .isRevert = true, .xorOut = 0, .isFinish = isFinish);
}

stock fletcher16(const data{}, dataSize, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new sum1 = 0;
    new sum2 = 0;
    for(; pos < dataSize; pos++)
    {
        sum1 = (sum1 + data{pos}) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
   return (sum2 << 8) | sum1;
}

stock fletcher16opt(const data{}, dataSize, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new c0;
    new c1;
    new len = dataSize - pos;
    const blockSize = 5802;
    for (; len >= blockSize; len -= blockSize)
        fletcher16opt_calcBlock(data, pos, dataSize, blockSize, c0, c1);
    
    fletcher16opt_calcBlock(data, pos, dataSize, len, c0, c1);
    return c1 << 8 | c0;
}

stock crc16ccitt(const data{}, dataSize, pos = 0, init = 0xFFFF, bool:isFinish = true)
{
    return crc(data, dataSize, .polyWidth = 16, .poly = 0x1021, .pos = pos, .init = init, .isRevert = false, .xorOut = 0, .isFinish = isFinish);
}

stock crc8dallasMaxim(const data{}, dataSize, pos = 0, init = 0xFF, bool:isFinish = true)
{
    return crc(data, dataSize, .polyWidth = 8, .poly = 0x31, .pos = pos, .init = init, .isRevert = false, .xorOut = 0, .isFinish = isFinish);
}

stock crc32(const data{}, dataSize, pos = 0, init = 0xFFFFFFFF, bool:isFinish = true)
{
    return crc(data, dataSize, .polyWidth = 32, .poly = 0x04C11DB7, .pos = pos, .init = init, .isRevert = true, .xorOut = 0xFFFFFFFF, .isFinish = isFinish);
}

stock crc16buypass(const data{}, dataSize, pos = 0, init = 0x0000, bool:isFinish = true)
{
    return crc(data, dataSize, .polyWidth = 16, .poly = 0x8005, .pos = pos, .init = init, .isRevert = false, .xorOut = 0, .isFinish = isFinish);
}

stock crc(const data{}, dataSize, polyWidth, poly, pos = 0, init = 0, bool:isRevert = false, xorOut = 0, bool:isFinish = true)
{
    if (pos < 0)
        pos = 0;

    new result = init;
    new topBit = (1 << (polyWidth - 1));
    for (; pos < dataSize; pos++)
    {
        new byte = data{pos};
        result ^= (isRevert ? reverseBits(byte, 8) : byte) << (polyWidth - 8);
        for (new i = 0; i < 8; i++)
            result = result & topBit ? (result << 1) ^ poly : (result << 1);
    }
    if (isFinish)
    {
        if (isRevert)
        {
            result = reverseBits(result, polyWidth);
        }
        else
        {
            new mask = (topBit << 1) - 1;
            result &= mask;
        }
        result ^= xorOut;
    }
    return result;
}

//! @privatesection

//! Расчет значений в блоке для fletcher16opt
//! @param[in] data массив для вычисления
//! @param[inout] pos начальная позиция
//! @param[in] dataLength размер массива
//! @param[in] blockSize размер блока для вычисления
//! @param[inout] c0 0 байт рассчитанного значения
//! @param[inout] c1 1 байт рассчитанного значения
stock fletcher16opt_calcBlock(const data{}, &pos, dataLength, blockSize, &c0, &c1)
{
    for (new i = 0; (i < blockSize) && (pos < dataLength); i++)
    {
        c0 += data{pos++};
        c1 += c0;
    }
    c0 %= 255;
    c1 %= 255;
}
