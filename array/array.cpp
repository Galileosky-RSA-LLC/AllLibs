#ifndef ARRAY_LIB
#define ARRAY_LIB
// Функции работы с массивами
// не рассматривается работа с конкретно строками (т.е. с завершающим \0)


#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"

//! XOR элементов подмассива 1-байтных элементов
//! \param[in] ar массив
//! \param[in] arSize размер массива
//! \param[in] start стартовый индекс
arrayXorStr(const ar{}, arSize, start = 0)
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

//! Суммирование элементов подмассива 1-байтных элементов
//! \param[in] ar массив
//! \param[in] arSize размер массива
//! \param[in] start стартовый индекс
arrayPlusStr(const ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    new res;
    for (new i = start; i < arSize; i++)
        res += ar{i};
    
    return res;
}

//! Вставить один подмассив однобайтных элементов в другой массив
//! \param[out] dest целевой массив, куда копируется
//! \param[in] destPos смещение в целевом
//! \param[in] destSize длина целевого массива
//! \param[in] source копируемый массив
//! \param[in] sourceSize длина копируемого массива
//! \param[in] sourcePos смещение в копируемом, с которого начинать копирование
//! \return количество вставленных элементов
insertArrayStr(dest{}, destPos, destSize, const source{}, sourceSize, sourcePos = 0)
{
    if (destPos < 0)
        destPos = 0;

    if (sourcePos < 0)
        sourcePos = 0;

    new i;
	for (i = 0; ((destPos + i) < destSize) && ((sourcePos + i) < sourceSize); i++)
		dest{destPos + i} = source{sourcePos + i};

	return i;
}

//! Проверить подмассивы на равенство соответствующих элементов
//! \param[in] ar1 первый сравниваемый массив
//! \param[in] ar1start индекс начала подмассива в первом массиве
//! \param[in] ar1size длина первого сравниваемого массива
//! \param[in] ar2 второй сравниваемый массив
//! \param[in] ar2start индекс начала подмассива во втором массиве
//! \param[in] ar2size длина второго сравниваемого массива
isArraysEqual(const ar1[], ar1start, ar1size, const ar2[], ar2start, ar2size)
{
	new count1 = getSubSize(ar1start, ar1size);
    return (count1 == getSubSize(ar2start, ar2size)) && (countIdentical(ar1, ar1start, ar1size, ar2, ar2start, ar2size) == count1);
}

//! Проверить подмассивы однобайтных элементов на равенство соответствующих элементов
//! \param[in] ar1 первый сравниваемый массив
//! \param[in] ar1start индекс начала подмассива в первом массиве
//! \param[in] ar1size длина первого сравниваемого массива
//! \param[in] ar2 второй сравниваемый массив
//! \param[in] ar2start индекс начала подмассива во втором массиве
//! \param[in] ar2size длина второго сравниваемого массива
isArraysEqualStr(const ar1{}, ar1start, ar1size, const ar2{}, ar2start, ar2size)
{
	new count1 = getSubSize(ar1start, ar1size);
    return (count1 == getSubSize(ar2start, ar2size)) && (countIdenticalStr(ar1, ar1start, ar1size, ar2, ar2start, ar2size) == count1);
}

//! Инвертировать подмассив
//! \param[inout] ar исходный массив
//! \param[in] arSize длина массива
//! \param[in] start индекс начала подмассива
invertArray(ar[], arSize, start = 0)
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

//! Инвертировать подмассив однобайтных элементов
//! \param[inout] ar исходный массив
//! \param[in] arSize длина массива
//! \param[in] start индекс начала подмассива
invertArrayStr(ar{}, arSize, start = 0)
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

//! Заменить определенные значения элементов подмассива на заданное
//! \param[inout] ar массив
//! \param[in] arSize длина массива
//! \param[in] valOld заменяемое значение
//! \param[in] valNew новое значение
//! \param[in] start индекс начала подмассива
//! \return кол-во выполненных замен
replaceVal(ar[], arSize, valOld, valNew, start = 0)
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

//! Заменить определенные значения в подмассиве однобайтных элементов на заданное
//! \param[inout] ar массив
//! \param[in] arSize длина массива
//! \param[in] valOld заменяемое значение
//! \param[in] valNew новое значение
//! \param[in] start индекс начала подмассива
//! \return кол-во выполненных замен
replaceValStr(ar{}, arSize, valOld, valNew, start = 0)
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

//! Инвертировать биты в байтах подмассива однобайтных элементов
//! \param[inout] ar массив
//! \param[in] arSize размер массива
//! \param[in] start стартовый индекс подмассива
arrayInvertBitsStr(ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;
    
    for (new i = start; i < arSize; i++)
        ar{i} = invertBitsInByte(ar{i});
}

//! Получить число 32, 24, 16 или 8 бит из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[in] bytes разрядность в байтах, 1..4
//! \param[in] littleEnd порядок хранения многобайтных чисел: true - littleEndian, false - BigEndian
//! \param[in] sign наличие знака для 8-, 16- и 24-битного числа: true - знаковое, false - беззнаковое (32-битное число всегда знаковое)
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num(const ar{}, start, arSize, bytes, littleEnd, sign, &num)
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

//! Получить знаковое 8-битное число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num8sign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 1, true, true, num);
}

//! Получить беззнаковое 8-битное число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num8unSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 1, true, false, num);
}

//! Получить знаковое 16-битное littleEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num16leSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 2, true, true, num);
}

//! Получить беззнаковое 16-битное littleEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num16leUnSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 2, true, false, num);
}

//! Получить беззнаковое 16-битное BigEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num16beUnSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 2, false, false, num);
}

//! Получить знаковое 16-битное BigEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num16beSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 2, false, true, num);
}

//! Получить знаковое 24-битное littleEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num24leSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 3, true, true, num);
}

//! Получить беззнаковое 24-битное littleEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num24leUnSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 3, true, false, num);
}

//! Получить беззнаковое 24-битное BigEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num24beUnSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 3, false, false, num);
}

//! Получить знаковое 24-битное BigEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num24beSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 3, false, true, num);
}

//! Получить знаковое 32-битное BigEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num32beSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 4, false, true, num);
}

//! Получить знаковое 32-битное littleEndian число из массива
//! \param[in] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \param[out] num полученное число
//! \return false - неуспешно (некорректные входные данные), true - успешно
array2num32leSign(const ar{}, start, arSize, &num)
{
    return array2num(ar, start, arSize, 4, true, true, num);
}

//! Поместить число 8, 16, 24 или 32 бит в массив
//! \param[in] num число для конвертации
//! \param[in] bytes кол-во байт в числе, 1..4
//! \param[in] littleEnd порядок хранения в массиве: true - littleEndian, false - BigEndian
//! \param[out] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \return false - неуспешно (некорректные входные данные), true - успешно
num2array(num, bytes, littleEnd, ar{}, start, arSize)
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

//! Поместить 32-битное число в массив littleEndian
//! \param[in] num число для конвертации
//! \param[out] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \return false - неуспешно (некорректные входные данные), true - успешно
num32bit2arrayLe(num, ar{}, start, arSize)
{
    return num2array(num, 4, true, ar, start, arSize);
}

//! Поместить 32-битное число в массив BigEndian
//! \param[in] num число для конвертации
//! \param[out] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \return false - неуспешно (некорректные входные данные), true - успешно
num32bit2arrayBe(num, ar{}, start, arSize)
{
    return num2array(num, 4, false, ar, start, arSize);
}

//! Поместить 16-битное число в массив littleEndian
//! \param[in] num число для конвертации
//! \param[out] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \return false - неуспешно (некорректные входные данные), true - успешно
num16bit2arrayLe(num, ar{}, start, arSize)
{
    return num2array(num, 2, true, ar, start, arSize);
}

//! Поместить 16-битное число в массив BigEndian
//! \param[in] num число для конвертации
//! \param[out] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \return false - неуспешно (некорректные входные данные), true - успешно
num16bit2arrayBe(num, ar{}, start, arSize)
{
    return num2array(num, 2, false, ar, start, arSize);
}

//! Линейный поиск элемента в массиве
//! \param[in] ar массив
//! \param[in] arSize длина массива
//! \param[in] element искомый элемент
//! \param[in] start стартовый индекс для поиска
//! \return >=0: меньший индекс искомого элемента, <0 - не найден
searchLinear(const ar[], arSize, element, start = 0)
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

//! Линейный поиск в массиве однобайтных элементов
//! \param[in] ar массив
//! \param[in] arSize длина массива
//! \param[in] element искомый элемент
//! \param[in] start стартовый индекс для поиска
//! \return >=0: меньший индекс искомого элемента, <0 - не найден
searchLinearStr(const ar{}, arSize, element, start = 0)
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

//! Двоичный поиск элемента в массиве
//! \param[in] ar массив
//! \param[in] arSize длина массива
//! \param[in] element искомый элемент
//! \param[in] start стартовый индекс для поиска
//! \return >=0: меньший индекс искомого элемента, <0 - не найден
searchBinary(const ar[], arSize, element, start = 0)
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

//! Двоичный поиск в массиве однобайтных элементов
//! \param[in] ar массив
//! \param[in] arSize длина массива
//! \param[in] element искомый элемент
//! \param[in] start стартовый индекс для поиска
//! \return >=0: меньший индекс искомого элемента, <0 - не найден
searchBinaryStr(const ar{}, arSize, element, start = 0)
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

//! Простой поиск подмассива в массиве (brute force algorithm, naive algorithm)
//! \param[in] ar массив
//! \param[in] arStart стартовый индекс массива для поиска
//! \param[in] arSize длина массива
//! \param[in] sub массив с искомым подмассивом
//! \param[in] subSize длина массива с искомым подмассивом
//! \param[in] subStart стартовый индекс искомого подмассива
//! \return >=0: индекс стартового элемента искомого подмассива в массиве для поиска, ближний к началу массива (т.е. первое вхождение), <0 - не найден
searchSubArBruteForce(const ar[], arStart, arSize, const sub[], subSize, subStart = 0)
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

//! Простой поиск подмассива в массиве однобайтных элементов (brute force algorithm, naive algorithm)
//! \param[in] ar массив
//! \param[in] arStart стартовый индекс массива для поиска
//! \param[in] arSize длина массива
//! \param[in] sub массив с искомым подмассивом
//! \param[in] subSize длина массива с искомым подмассивом
//! \param[in] subStart стартовый индекс искомого подмассива
//! \return >=0: индекс стартового элемента искомого подмассива в массиве для поиска, ближний к началу массива (т.е. первое вхождение), <0 - не найден
searchSubArBruteForceStr(const ar{}, arStart, arSize, const sub{}, subSize, subStart = 0)
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

//! Шейкер-сортировка подмассива по возрастанию (сортировка перемешиванием или двунаправленная, Cocktail sort)
//! \param[inout] ar массив для сортировки
//! \param[in] arSize длина массива
//! \param[in] start индекс начала сортируемого подмассива
shakerSort(ar[], arSize, start = 0)
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

//! Шейкер-сортировка подмассива однобайтных элементов по возрастанию (сортировка перемешиванием или двунаправленная, Cocktail sort)
//! \param[inout] ar массив для сортировки
//! \param[in] arSize длина массива
//! \param[in] start индекс начала сортируемого подмассива
shakerSortStr(ar{}, arSize, start = 0)
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

//! Получить значение бита в подмассиве
//! \param[in] ar массив
//! \param[in] start позиция начала подмассива
//! \param[in] arSize размер массива
//! \param[in] bit позиция бита относительно начала подмассива
//! \param[out] value значение бита при успешном возврате
//! \return false - неуспешно (ошибка входных данных), true - успешно
getBitFromArray(const ar{}, start, arSize, bit, &value)
{
    new offset = start + bit / 8;
    if ((start < 0) || (bit < 0) || (offset >= arSize))
        return false;

    value = (ar{offset} >> bit % 8) & 1;
    return true;
}

//! Установить значение бита в подмассиве
//! \param[inout] ar массив
//! \param[in] start позиция начала подмассива
//! \param[in] arSize размер массива
//! \param[in] bit позиция бита относительно начала подмассива
//! \param[in] value устанавливаемое значение бита
//! \return false - неуспешно (ошибка входных данных), true - успешно
setBitInArray(ar{}, start, arSize, bit, value)
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

//! Поиск минимума в массиве
//! \param[in] ar массив
//! \param[in] arSize размер массива
//! \param[in] start позиция начала подмассива
//! \return -1 - не найден (ошибка входных данных), >=0 - успешно (индекс элемента)
searchMin(const ar[], arSize, start = 0)
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

//! Поиск минимума в массиве однобайтных элементов
//! \param[in] ar массив
//! \param[in] arSize размер массива
//! \param[in] start позиция начала подмассива
//! \return -1 - не найден (ошибка входных данных), >=0 - успешно (индекс элемента)
searchMinStr(const ar{}, arSize, start = 0)
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

//! Поиск максимума в массиве
//! \param[in] ar массив
//! \param[in] arSize размер массива
//! \param[in] start позиция начала подмассива
//! \return -1 - не найден (ошибка входных данных), >=0 - успешно (индекс элемента)
searchMax(const ar[], arSize, start = 0)
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

//! Поиск максимума в массиве однобайтных элементов
//! \param[in] ar массив
//! \param[in] arSize размер массива
//! \param[in] start позиция начала подмассива
//! \return -1 - не найден (ошибка входных данных), >=0 - успешно (индекс элемента)
searchMaxStr(const ar{}, arSize, start = 0)
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

//! Очистить подмассив (заменить элементы на \0)
//! \param[in] ar массив
//! \param[in] start позиция начала подмассива
//! \param[in] arSize размер массива
clearArrayStr(ar{}, arSize, start = 0)
{
    if (start < 0)
        start = 0;

    for (new i = start; i < arSize; i++)
        ar{i} = 0;
}

//! Подсчет занятых 32-битных ячеек массива по его актуальной длине в байтах
countArrayUsedCells(currentArSizeInBytes)
{
    return countUsedCells(currentArSizeInBytes, 4);
}

//! Поместить 24-битное число в массив littleEndian
//! \param[in] num число для конвертации
//! \param[out] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \return false - неуспешно (некорректные входные данные), true - успешно
num24bit2arrayLe(num, ar{}, start, arSize)
{
    return num2array(num, 3, true, ar, start, arSize);
}

//! Поместить 24-битное число в массив BigEndian
//! \param[in] num число для конвертации
//! \param[out] ar массив
//! \param[in] start стартовый индекс массива
//! \param[in] arSize длина массива
//! \return false - неуспешно (некорректные входные данные), true - успешно
num24bit2arrayBe(num, ar{}, start, arSize)
{
    return num2array(num, 3, false, ar, start, arSize);
}

//! Посчитать количество одинаковых соответствующих элементов
//! \param[in] ar1 первый сравниваемый массив
//! \param[in] ar1start индекс начала подмассива в первом массиве
//! \param[in] ar1size длина первого сравниваемого массива
//! \param[in] ar2 второй сравниваемый массив
//! \param[in] ar2start индекс начала подмассива во втором массиве
//! \param[in] ar2size длина второго сравниваемого массива
countIdentical(const ar1[], ar1start, ar1size, const ar2[], ar2start, ar2size)
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

//! Посчитать количество одинаковых соответствующих однобайтных элементов
//! \param[in] ar1 первый сравниваемый массив
//! \param[in] ar1start индекс начала подмассива в первом массиве
//! \param[in] ar1size длина первого сравниваемого массива
//! \param[in] ar2 второй сравниваемый массив
//! \param[in] ar2start индекс начала подмассива во втором массиве
//! \param[in] ar2size длина второго сравниваемого массива
countIdenticalStr(const ar1{}, ar1start, ar1size, const ar2{}, ar2start, ar2size)
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

//! Посчитать длину подмассива
//! \param[in] arStart индекс начала подмассива
//! \param[in] arSize длина массива
getSubSize(arStart, arSize)
{
    if (arStart < 0)
        arStart = 0;
    
    if (arSize < 0)
        arSize = 0;
    
    return arSize > arStart ? arSize - arStart : 0;
}

//! Кольцевой сдвиг элементов подмассива
//! \param[in] ar изменяемый массив
//! \param[in] arSize длина массива
//! \param[in] toRight признак необходимости сдвига вправо: true - вправо, false - влево
//! \param[in] arStart индекс начала подмассива
//! \param[in] count величина сдвига
arrayRingShift(ar[], arSize, toRight, arStart = 0, count = 1)
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

//! Кольцевой сдвиг однобайтных элементов подмассива
//! \param[in] ar изменяемый массив
//! \param[in] arSize длина массива
//! \param[in] toRight признак необходимости сдвига вправо: true - вправо, false - влево
//! \param[in] arStart индекс начала подмассива
//! \param[in] count величина сдвига
arrayRingShiftStr(ar{}, arSize, toRight, arStart = 0, count = 1)
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

//! Вставить один подмассив в другой массив
//! \param[out] dest целевой массив, куда копируется
//! \param[in] destPos смещение в целевом
//! \param[in] destSize длина целевого массива
//! \param[in] source копируемый массив
//! \param[in] sourceSize длина копируемого массива
//! \param[in] sourcePos смещение в копируемом, с которого начинать копирование
//! \return количество вставленных элементов
insertArray(dest[], destPos, destSize, const source[], sourceSize, sourcePos = 0)
{
    if (destPos < 0)
        destPos = 0;

    if (sourcePos < 0)
        sourcePos = 0;

    new i;
	for (i = 0; ((destPos + i) < destSize) && ((sourcePos + i) < sourceSize); i++)
		dest[destPos + i] = source[sourcePos + i];

	return i;
}

#endif // ARRAY_LIB
