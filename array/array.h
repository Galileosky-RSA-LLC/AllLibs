#ifndef ARRAY_H
#define ARRAY_H
// Функции работы с массивами
// не рассматривается работа с конкретно строками (т.е. с завершающим \0)

countArrayUsedCells(currentArSizeInBytes);
isArraysEqual(const ar1[], ar1start, ar1size, const ar2[], ar2start, ar2size);
isArraysEqualStr(const ar1{}, ar1start, ar1size, const ar2{}, ar2start, ar2size);
insertArrayStr(dest{}, destPos, destSize, const source{}, sourceSize, sourcePos = 0);
replaceVal(ar[], arSize, valOld, valNew, start = 0);
replaceValStr(ar{}, arSize, valOld, valNew, start = 0);
countIdentical(const ar1[], ar1start, ar1size, const ar2[], ar2start, ar2size);
countIdenticalStr(const ar1{}, ar1start, ar1size, const ar2{}, ar2start, ar2size);
getSubSize(arStart, arSize);

// Однотипные операции над элементами
arrayPlusStr(const ar{}, arSize, start = 0);
arrayXorStr(const ar{}, arSize, start = 0);
arrayInvertBitsStr(ar{}, arSize, start = 0);
clearArrayStr(ar{}, arSize, start = 0);
invertArray(ar[], arSize, start = 0);
invertArrayStr(ar{}, arSize, start = 0);
arrayRingShift(ar[], arSize, toRight, arStart = 0, count = 1);
arrayRingShiftStr(ar{}, arSize, toRight, arStart = 0, count = 1);

// Преобразование массив <-> число
array2num(const ar{}, start, arSize, bytes, littleEnd, sign, &num);
array2num8sign(const ar{}, start, arSize, &num);
array2num8unSign(const ar{}, start, arSize, &num);
array2num16leSign(const ar{}, start, arSize, &num);
array2num16leUnSign(const ar{}, start, arSize, &num);
array2num16beUnSign(const ar{}, start, arSize, &num);
array2num16beSign(const ar{}, start, arSize, &num);
array2num24leSign(const ar{}, start, arSize, &num);
array2num24leUnSign(const ar{}, start, arSize, &num);
array2num24beSign(const ar{}, start, arSize, &num);
array2num24beUnSign(const ar{}, start, arSize, &num);
array2num32beSign(const ar{}, start, arSize, &num);
array2num32leSign(const ar{}, start, arSize, &num);
num2array(num, bytes, littleEnd, ar{}, start, arSize);
num16bit2arrayLe(num, ar{}, start, arSize);
num16bit2arrayBe(num, ar{}, start, arSize);
num24bit2arrayLe(num, ar{}, start, arSize);
num24bit2arrayBe(num, ar{}, start, arSize);
num32bit2arrayLe(num, ar{}, start, arSize);
num32bit2arrayBe(num, ar{}, start, arSize);

// Поиск элемента
searchLinear(const ar[], arSize, element, start = 0);
searchLinearStr(const ar{}, arSize, element, start = 0);
searchBinary(const ar[], arSize, element, start = 0);
searchBinaryStr(const ar{}, arSize, element, start = 0);

// Поиск подмассива
searchSubArBruteForce(const ar[], arStart, arSize, const sub[], subSize, subStart = 0);
searchSubArBruteForceStr(const ar{}, arStart, arSize, const sub{}, subSize, subStart = 0);

// Поиск экстремумов
searchMin(const ar[], arSize, start = 0);
searchMinStr(const ar{}, arSize, start = 0);
searchMax(const ar[], arSize, start = 0);
searchMaxStr(const ar{}, arSize, start = 0);

// Сортировки
shakerSort(ar[], arSize, start = 0);
shakerSortStr(ar{}, arSize, start = 0);

// Битовые операции
getBitFromArray(const ar{}, start, arSize, bit, &value);
setBitInArray(ar{}, start, arSize, bit, value);

#endif // ARRAY_H