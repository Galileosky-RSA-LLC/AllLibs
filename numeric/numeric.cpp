#ifndef NUMERIC_LIB
#define NUMERIC_LIB
// Библиотека работы с числами

#include "numeric.h"

//! Инвертировать биты в байте
invertBitsInByte(byte)
{
    new tmp = 0;
    for (new j = 0; j < 8; j++)
    {
        tmp <<= 1;
        tmp |= byte & 1;
        byte >>= 1;
    }
    return tmp;
}

//! Получить значение бита из байта
//! \param[in] byteValue исходный байт
//! \param[in] bit номер бита, 0..7
//! \return значение бита false/true
getBit(byteValue, bit)
{
    if (bit < 0)
        bit = 0;

    if (bit > 7)
        bit = 7;

    return ((byteValue & 0xFF) >> bit) & 1;
}

//! Установить значение бита в байте
//! \param[in] byteValue исходный байт
//! \param[in] bit номер изменяемого бита, 0..7
//! \param[in] val устанавливаемое значение бита false/true
//! \return измененный байт
setBit(byteValue, bit, val)
{
    if (bit < 0)
        bit = 0;

    if (bit > 7)
        bit = 7;
    
    if (val != 0)
        byteValue |= 1 << bit;
    else
        byteValue &= ~(1 << bit);

    return byteValue;
}

//! Корректировать число так, чтобы оно оказалось внутри заданного диапазона
//! \param[inout] num преобразуемое число
//! \param[in] rangeMin минимум диапазона
//! \param[in] rangeMax максимум диапазона
//! \return false - число не корректировалось, true - число корректировалось
coerce(&num, rangeMin, rangeMax)
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

//! Подсчёт числа разрядов в числе
digits(val)
{
    if (val == 0)
        return 1;
    
    if (val == NUM_VALUE_MIN)
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

//! Возведение числа в неотрицательную степень,
//! без проверок границ диапазона
//! \param[in] num исходное число
//! \param[in] p степень
pow(num, p)
{
    if (p <= 0)
        return 1;

    new res = num;
    for (new i = 0; i < (p - 1); i++)
        res *= num;

    return res;
}

//! Возвращает модуль числа
abs(num)
{
    return (num >= 0) ? num : -num;
}

//! Возвращает псевдослучайное число в заданном диапазоне
//! \param[in] atLeast нижняя граница (от), 0..2147483647
//! \param[in] noGreater верхняя граница (до), 0..2147483647
rnd(atLeast, noGreater)
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
        uptimeNew = (uptimeNew >= calls) ? uptimeNew - calls : NUM_VALUE_MAX - calls + uptimeNew;
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

//! Конвертировать в BCD
dec2bcd(dec)
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

//! Подсчет занятых ячеек по актуальной длине
countUsedCells(size, cellSize)
{
    if ((size <= 0) || (cellSize <= 0))
        return 0;

    return (size / cellSize) + ((size % cellSize) ? 1 : 0);
}

#endif // NUMERIC_LIB
