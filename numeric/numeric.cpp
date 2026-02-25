//! @file
//! @brief Реализация библиотеки работы с числами

#if defined NUMERIC_LIB
#endinput
#endif
#define NUMERIC_LIB

#include "numeric.h"

//! @publicsection

stock invertBitsInByte(byte)
{
    new tmp = 0;
    for (new i = 0; i < BYTE_BITS; i++)
    {
        tmp <<= 1;
        tmp |= byte & 1;
        byte >>= 1;
    }
    return tmp;
}

stock getBit(num, bit)
{
    coerce(bit, 0, CELL_LAST_BIT_INDEX);
    return (num >> bit) & 1;
}

stock setBit(num, bit, bitValue)
{
    coerce(bit, 0, CELL_LAST_BIT_INDEX);
    new tmp = 1 << bit;
    if (bitValue)
        num |= tmp;
    else
        num &= ~tmp;

    return num;
}

stock bool:coerce(&num, rangeMin, rangeMax)
{
    if (rangeMin > rangeMax)
    {
        new tmp = rangeMin;
        rangeMin = rangeMax;
        rangeMax = tmp;
    }
    if (num < rangeMin)
    {
        num = rangeMin;
        return true;
    }
    if (num > rangeMax)
    {
        num = rangeMax;
        return true;
    }
    return false;
}

stock digits(val)
{
    if (val == 0)
        return 1;
    
    if (val == cellmin)
        return 10;
    
    if (val < 0)
        val *= -1;

    new res;
    while (val > 0)
    {
        res++;
        val /= 10;
    }
    return res;
}

stock pow(num, p)
{
    if (p <= 0)
        return 1;

    new res = num;
    for (new i = 0; i < (p - 1); i++)
        res *= num;

    return res;
}

stock abs(num)
{
    return (num >= 0) ? num : -num;
}

stock rnd(atLeast, noGreater)
{
    if (atLeast < 0)
        atLeast = 0;
    
    if (noGreater < 0)
        noGreater = 0;

    if (atLeast > noGreater)
    {    
        new tmp = atLeast;
        atLeast = noGreater;
        noGreater = tmp;
    }
    if (atLeast == noGreater)
        return atLeast;

    static uptimeOld;
    static calls;
    new uptimeNew;
    uptimeNew = GetVar(UPTIME);
    if (uptimeNew == uptimeOld)
    {
        calls++;
        uptimeNew = (uptimeNew >= calls) ? uptimeNew - calls : cellmax - calls + uptimeNew;
    }
    else
    {
        uptimeOld = uptimeNew;
        calls = 0;
    }
    if (uptimeNew == 0)
        return atLeast;

    new range = noGreater - atLeast + 1;
    return atLeast + (uptimeNew > range ? uptimeNew % range : range % uptimeNew);
}

stock dec2bcd(dec)
{
    new shift;
    new result;
    while (dec)
    {
        result += (dec % 10) << shift;
        dec /= 10;
        shift += 4;
    }
    return result;
}

stock countUsedCells(size, cellSize)
{
    if ((size <= 0) || (cellSize <= 0))
        return 0;

    return (size / cellSize) + ((size % cellSize) ? 1 : 0);
}

stock rol(value, bits)
{
    new restBits;
    rolr_normalizeBits(bits, restBits);
    return (value << bits) | (value >>> restBits);
}

stock ror(value, bits)
{
    new restBits;
    rolr_normalizeBits(bits, restBits);
    return (value >>> bits) | (value << restBits);
}

stock reverse(value)
{
    return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >>> 8) | ((value & 0xFF000000) >>> 24);
}

stock reverseBits(value, bits)
{
    coerce(bits, 0, cellbits);
    new res = 0;
    for (new i = 0; i < bits; i++)
        res = (res << 1) | ((value >>> i) & 1);

    return res;
}

stock setByte(number, byteId, newByteValue)
{
    coerce(byteId, 0, CELL_LAST_BYTE_INDEX);
    new shift = byteId * BYTE_BITS;
    return (number & (~(0xFF << shift))) | ((newByteValue & 0xFF) << shift);
}

stock getByte(number, byteId)
{
    coerce(byteId, 0, CELL_LAST_BYTE_INDEX);
    return (number >> (byteId * BYTE_BITS)) & 0xFF;
}

//! @privatesection

//! @brief Нормализовать биты для циклических сдвигов
stock rolr_normalizeBits(&bits, &restBits)
{
    const mask = cellbits - 1;
    bits &= mask;
    restBits = cellbits - bits;
}
