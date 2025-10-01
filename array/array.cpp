//! @file
//! @brief Функции библиотеки для работы с массивами

#ifdef ARRAY_LIB
#endinput
#endif
#define ARRAY_LIB

#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"

stock arrayXorStr(const ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new res;
    if (arSize > 0)
        res = ar{start};

    for (new i = start + 1; i < arSize; i++)
        res ^= ar{i};
    
    return res;
}

stock arrayPlusStr(const ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new res;
    for (new i = start; i < arSize; i++)
        res += ar{i};
    
    return res;
}

stock insertArrayStr(dest{}, destPos, destSize, const source{}, sourceSize, sourcePos = 0, fromBack = false)
{
    if (destPos < 0)
        destPos = 0;

    if (sourcePos < 0)
        sourcePos = 0;

    new sourceLen = sourceSize - sourcePos;
    new destLen = destSize - destPos;
    new i = 0;
    if (fromBack)
    {
        if (sourceLen > destLen)
            return 0;

        for (; i < sourceLen; i++)
            dest{destPos + sourceLen - 1 - i} = source{sourceSize - 1 - i};
    }
    else
    {
        for (; (i < destLen) && (i < sourceLen); i++)
            dest{destPos + i} = source{sourcePos + i};
    }
    return i;
}

stock isArraysEqual(const ar1[], ar1start, ar1size, const ar2[], ar2start, ar2size)
{
	new count1 = getSubSize(ar1start, ar1size);
    return (count1 == getSubSize(ar2start, ar2size)) && (countIdentical(ar1, ar1start, ar1size, ar2, ar2start, ar2size) == count1);
}

stock isArraysEqualStr(const ar1{}, ar1start, ar1size, const ar2{}, ar2start, ar2size)
{
	new count1 = getSubSize(ar1start, ar1size);
    return (count1 == getSubSize(ar2start, ar2size)) && (countIdenticalStr(ar1, ar1start, ar1size, ar2, ar2start, ar2size) == count1);
}

stock invertArray(ar[], arSize, start = 0)
{
    if (start < 0)
        start = 0;
    
    for (new i = 0; i < (arSize - start) / 2; i++)
    {
        new ch = ar[start + i];
        ar[start + i] = ar[arSize - 1 - i];
        ar[arSize - 1 - i] = ch;
    }
}

stock invertArrayStr(ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    for (new i = 0; i < (arSize - start) / 2; i++)
    {
        new ch = ar{start + i};
        ar{start + i} = ar{arSize - 1 - i};
        ar{arSize - 1 - i} = ch;
    }
}

stock replaceVal(ar[], arSize, valOld, valNew, start = 0)
{
    if (start < 0)
        start = 0;

    new count;
    for (new i = start; i < arSize; i++)
    {    
        if (ar[i] == valOld)
        {
            ar[i] = valNew;
            count++;
        }
    }
    return count;
}

stock replaceValStr(ar{}, arSize, valOld, valNew, start = 0)
{
    if (start < 0)
        start = 0;
    
    new count;
    for (new i = start; i < arSize; i++)
    {    
        if (ar{i} == valOld)
        {
            ar{i} = valNew;
            count++;
        }
    }
    return count;
}

stock arrayInvertBitsStr(ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;
    
    for (new i = start; i < arSize; i++)
        ar{i} = invertBitsInByte(ar{i});
}

stock array2num(const ar{}, start, arSize, bytes, littleEnd, sign, &num)
{
    if (bytes < 1)
        bytes = 1;
    
    if (bytes > 4)
        bytes = 4;

    if ((start + bytes > arSize) || (start < 0))
        return false;

    num = 0;
    for (new i = 0; i < bytes; i++)
        num += ar{start + i} << (littleEnd ? 8 * i : 8 * (bytes - 1 - i));

    if (sign)
    {
        if ((bytes == 1) && (num > 0x7F))
            num -= 0x100;
        
        if ((bytes == 2) && (num > 0x7FFF))
            num -= 0x10000;
        
        if ((bytes == 3) && (num > 0x7FFFFF))
            num -= 0x1000000;
    }
    return true;
}

stock array2num8sign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 1, true, true, num);
}

stock array2num8unSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 1, true, false, num);
}

stock array2num16leSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 2, true, true, num);
}

stock array2num16leUnSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 2, true, false, num);
}

stock array2num16beUnSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 2, false, false, num);
}

stock array2num16beSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 2, false, true, num);
}

stock array2num24leSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 3, true, true, num);
}

stock array2num24leUnSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 3, true, false, num);
}

stock array2num24beUnSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 3, false, false, num);
}

stock array2num24beSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 3, false, true, num);
}

stock array2num32beSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 4, false, true, num);
}

stock array2num32leSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 4, true, true, num);
}

stock num2array(num, bytes, littleEnd, ar{}, start, arSize)
{
    if (bytes < 1)
        bytes = 1;
    
    if (bytes > 4)
        bytes = 4;

    if ((start + bytes > arSize) || (start < 0))
        return false;

    for (new i = 0; i < bytes; i++)
        ar{start + i} = (num & (0xFF << 8 * (littleEnd ? i :( bytes - 1 - i)))) >> (8 * (littleEnd ? i : (bytes - 1 - i)));

    return true;
}

stock num32bit2arrayLe(num, ar{}, start, arSize)
{
    return num2array(num, 4, true, ar, start, arSize);
}

stock num32bit2arrayBe(num, ar{}, start, arSize)
{
    return num2array(num, 4, false, ar, start, arSize);
}

stock num16bit2arrayLe(num, ar{}, start, arSize)
{
    return num2array(num, 2, true, ar, start, arSize);
}

stock num16bit2arrayBe(num, ar{}, start, arSize)
{
    return num2array(num, 2, false, ar, start, arSize);
}

stock searchLinear(const ar[], arSize, element, start = 0)
{
    if (start < 0)
        start = 0;

    for (new i = start; i < arSize; i++)
    {    
        if (ar[i] == element)
            return i;
    }
    return -1;
}

stock searchLinearStr(const ar{}, arSize, element, start = 0)
{
    if (start < 0)
        start = 0;

    for (new i = start; i < arSize; i++)
    {    
        if (ar{i} == element)
            return i;
    }
    return -1;
}

stock searchBinary(const ar[], arSize, element, start = 0)
{
    if (arSize <= 0)
        return -1;

    if (start < 0)
        start = 0;

    new first;
    first = start;
    new last;
    last = arSize - 1;
    new mid;
    while ((first < last) && (ar[last] != element))
    {
        mid = first + (last - first) / 2;
        if (ar[mid] >= element)
            last = mid
        else
            first = mid + 1;
    }
    // After the end of the loop, lastIndex can point to the search item. 
    // Otherwise the element is missing in the array.
    return (ar[last] == element) ? last : -1;
}

stock searchBinaryStr(const ar{}, arSize, element, start = 0)
{
    if (arSize <= 0)
        return -1;

    if (start < 0)
        start = 0;

    new first;
    first = start;
    new last;
    last = arSize - 1;
    new mid;
    while ((first < last) && (ar{last} != element))
    {
        mid = first + (last - first) / 2;
        if (ar{mid} >= element)
            last = mid
        else
            first = mid + 1;
    }
    // After the end of the loop, lastIndex can point to the search item. 
    // Otherwise the element is missing in the array.
    return (ar{last} == element) ? last : -1;
}

stock searchSubArBruteForce(const ar[], arStart, arSize, const sub[], subSize, subStart = 0)
{
    if (arStart < 0)
        arStart = 0;

    if (subStart < 0)
        subStart = 0;

    if ((arSize <= 0) || (subSize <= 0) || (arStart >= arSize) || (subStart >= subSize))
        return -1;

    for (new i = 0; i <= (arSize - subSize); i++)
    {
        new j;
        for (j = 0; j < (subSize - subStart); j++)
        {
            if (ar[arStart + i + j] != sub[subStart + j])
                break;
        }
        if (j >= (subSize - subStart))
            return arStart + i;
    }
    return -1;
}

stock searchSubArBruteForceStr(const ar{}, arStart, arSize, const sub{}, subSize, subStart = 0)
{
    if (arStart < 0)
        arStart = 0;

    if (subStart < 0)
        subStart = 0;

    if ((arSize <= 0) || (subSize <= 0) || (arStart >= arSize) || (subStart >= subSize))
        return -1;

    for (new i = 0; i <= (arSize - subSize); i++)
    {
        new j;
        for (j = 0; j < (subSize - subStart); j++)
        {
            if (ar{arStart + i + j} != sub{subStart + j})
                break;
        }
        if (j >= (subSize - subStart))
            return arStart + i;
    }
    return -1;
}

stock shakerSort(ar[], arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new left; // левая граница сортируемой области
    left = start;
    new right; // правая граница сортируемой области
    right = arSize - 1;
    new move = true;  // признак перемещений
    new i; //счетчик
    new t; // для обмена
    while ((left < right) && move)
    {
        move = false;
        for (i = left; i < right; i++)  // слева направо
        {
            if (ar[i] > ar[i + 1])
            {
                // обмен
                t = ar[i];
                ar[i] = ar[i + 1];
                ar[i + 1] = t;
                move = true;
            }
        }
        right--;
        for (i = right; i > left; i--)  // справа налево
        {
            if (ar[i - 1] > ar[i])
            {
                // обмен
                t = ar[i];
                ar[i] = ar[i - 1];
                ar[i - 1] = t;
                move = true;
            }
        }
        left++; // сдвигаем левую границу на следующий элемент
    }
}

stock shakerSortStr(ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new left; // левая граница сортируемой области
    left = start;
    new right; // правая граница сортируемой области
    right = arSize - 1;
    new move = true;  // признак перемещений
    new i; //счетчик
    new t; // для обмена
    while ((left < right) && move)
    {
        move = false;
        for (i = left; i < right; i++)  // слева направо
        {
            if (ar{i} > ar{i + 1})
            {
                // обмен
                t = ar{i};
                ar{i} = ar{i + 1};
                ar{i + 1} = t;
                move = true;
            }
        }
        right--;
        for (i = right; i > left; i--)  // справа налево
        {
            if (ar{i - 1} > ar{i})
            {
                // обмен
                t = ar{i};
                ar{i} = ar{i - 1};
                ar{i - 1} = t;
                move = true;
            }
        }
        left++; // сдвигаем левую границу на следующий элемент
    }
}

stock getBitFromArray(const ar{}, start, arSize, bit, &value)
{
    new offset = start + bit / 8;
    if ((start < 0) || (bit < 0) || (offset >= arSize))
        return false;

    value = (ar{offset} >> bit % 8) & 1;
    return true;
}

stock setBitInArray(ar{}, start, arSize, bit, value)
{
    new offset = start + bit / 8;
    if ((start < 0) || (bit < 0) || (offset >= arSize))
        return false;

    if (value != 0)
        ar{offset} |= 1 << bit % 8;
    else
        ar{offset} &= ~(1 << bit % 8);
    
    return true;
}

stock searchMin(const ar[], arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new idx = -1;
    for (new i = start; i < arSize; i++)
    {
        if (ar[i] < ar[idx])
            idx = i;
    }
    return idx;
}

stock searchMinStr(const ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new idx = -1;
    for (new i = start; i < arSize; i++)
    {
        if (ar{i} < ar{idx})
            idx = i;
    }
    return idx;
}

stock searchMax(const ar[], arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new idx = -1;
    for (new i = start; i < arSize; i++)
    {
        if (ar[i] > ar[idx])
            idx = i;
    }
    return idx;
}

stock searchMaxStr(const ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new idx = -1;
    for (new i = start; i < arSize; i++)
    {
        if (ar{i} > ar{idx})
            idx = i;
    }
    return idx;
}

stock clearArrayStr(ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    for (new i = start; i < arSize; i++)
        ar{i} = 0;
}

stock countArrayUsedCells(currentArSizeInBytes)
{
    return countUsedCells(currentArSizeInBytes, 4);
}

stock num24bit2arrayLe(num, ar{}, start, arSize)
{
    return num2array(num, 3, true, ar, start, arSize);
}

stock num24bit2arrayBe(num, ar{}, start, arSize)
{
    return num2array(num, 3, false, ar, start, arSize);
}

stock countIdentical(const ar1[], ar1start, ar1size, const ar2[], ar2start, ar2size)
{
	if (ar1start < 0)
        ar1start = 0;
    
    if (ar2start < 0)
        ar2start = 0;
    
    new i;
    for (i = 0; ((ar1start + i) < ar1size) && ((ar2start + i) < ar2size) && (ar1[ar1start + i] == ar2[ar2start + i]); i++)
    {}
    return i;
}

stock countIdenticalStr(const ar1{}, ar1start, ar1size, const ar2{}, ar2start, ar2size)
{
	if (ar1start < 0)
        ar1start = 0;
    
    if (ar2start < 0)
        ar2start = 0;
    
    new i;
    for (i = 0; ((ar1start + i) < ar1size) && ((ar2start + i) < ar2size) && (ar1{ar1start + i} == ar2{ar2start + i}); i++)
    {}
    return i;
}

stock getSubSize(arStart, arSize)
{
    if (arStart < 0)
        arStart = 0;
    
    if (arSize < 0)
        arSize = 0;
    
    return arSize > arStart ? arSize - arStart : 0;
}

stock arrayRingShift(ar[], arSize, toRight, arStart = 0, count = 1)
{
    if (arStart < 0)
        arStart = 0;

    new subSize = getSubSize(arStart, arSize);
    if (subSize <= 1)
        return;

    count %= subSize;
    new arLast = arSize - 1;
    for (new i = 0; i < count; i++)
    {
        new t = ar[toRight ? arLast : arStart];
        for (new j = 0; j < (subSize - 1); j++)
            ar[toRight ? arLast - j : j] = ar[toRight ? arLast - j - 1 : j + 1];

        ar[toRight ? arStart : arLast] = t;
    }
}

stock arrayRingShiftStr(ar{}, arSize, toRight, arStart = 0, count = 1)
{
    if (arStart < 0)
        arStart = 0;

    new subSize = getSubSize(arStart, arSize);
    if (subSize <= 1)
        return;

    count %= subSize;
    new arLast = arSize - 1;
    for (new i = 0; i < count; i++)
    {
        new t = ar{toRight ? arLast : arStart};
        for (new j = 0; j < (subSize - 1); j++)
            ar{toRight ? arLast - j : j} = ar{toRight ? arLast - j - 1 : j + 1};

        ar{toRight ? arStart : arLast} = t;
    }
}

stock insertArray(dest[], destPos, destSize, const source[], sourceSize, sourcePos = 0, fromBack = false)
{
    if (destPos < 0)
        destPos = 0;

    if (sourcePos < 0)
        sourcePos = 0;

    new sourceLen = sourceSize - sourcePos;
    new destLen = destSize - destPos;
    new i = 0;
    if (fromBack)
    {
        if (sourceLen > destLen)
            return 0;

        for (; i < sourceLen; i++)
            dest[destPos + sourceLen - 1 - i] = source[sourceSize - 1 - i];
    }
    else
    {
        for (; (i < destLen) && (i < sourceLen); i++)
            dest[destPos + i] = source[sourcePos + i];
    }
    return i;
}
