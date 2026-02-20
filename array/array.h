//! @file
//! @brief Заголовок библиотеки для работы с массивами

#ifdef ARRAY_H
#endinput
#endif
#define ARRAY_H

//! @defgroup general Общие функции
//! @{

//! @brief Подсчет занятых ячеек массива по его актуальной длине в байтах
forward stock countArrayUsedCells(currentArSizeInBytes);

//! @brief Проверить подмассивы на равенство соответствующих элементов
//! @param[in] ar1 первый сравниваемый массив
//! @param[in] ar1start индекс начала подмассива в первом массиве
//! @param[in] ar1size длина первого сравниваемого массива
//! @param[in] ar2 второй сравниваемый массив
//! @param[in] ar2start индекс начала подмассива во втором массиве
//! @param[in] ar2size длина второго сравниваемого массива
forward stock isArraysEqual(const ar1[], ar1start, ar1size, const ar2[], ar2start, ar2size);

//! @brief Проверить подмассивы однобайтных элементов на равенство соответствующих элементов
//! @param[in] ar1 первый сравниваемый массив
//! @param[in] ar1start индекс начала подмассива в первом массиве
//! @param[in] ar1size длина первого сравниваемого массива
//! @param[in] ar2 второй сравниваемый массив
//! @param[in] ar2start индекс начала подмассива во втором массиве
//! @param[in] ar2size длина второго сравниваемого массива
forward stock isArraysEqualStr(const ar1{}, ar1start, ar1size, const ar2{}, ar2start, ar2size);

//! @brief Вставить подмассив
//! @param[out] dest целевой массив, в который вставляется
//! @param[in] destPos смещение в целевом
//! @param[in] destSize размер целевого массива
//! @param[in] source вставляемый массив
//! @param[in] sourceSize размер вставляемого массива
//! @param[in] sourcePos смещение в вставляемом, с которого начинать вставку
//! @param[in] fromBack признак необходимости вставки с конца вставляемого (например, при совпадении вставляемого с целевым)
//! @return количество вставленных элементов
forward stock insertArray(dest[], destPos, destSize, const source[], sourceSize, sourcePos = 0, fromBack = false);

//! @brief Вставить подмассив однобайтных элементов
//! @param[out] dest целевой массив, в который вставляется
//! @param[in] destPos смещение в целевом
//! @param[in] destSize размер целевого массива
//! @param[in] source вставляемый массив
//! @param[in] sourceSize размер вставляемого массива
//! @param[in] sourcePos смещение в вставляемом, с которого начинать вставку
//! @param[in] fromBack признак необходимости вставки с конца вставляемого (при совпадении вставляемого с целевым)
//! @return количество вставленных элементов
forward stock insertArrayStr(dest{}, destPos, destSize, const source{}, sourceSize, sourcePos = 0, fromBack = false);

//! @brief Заменить определенные значения элементов подмассива на заданное
//! @param[inout] ar массив
//! @param[in] arSize длина массива
//! @param[in] valOld заменяемое значение
//! @param[in] valNew новое значение
//! @param[in] start индекс начала подмассива
//! @return кол-во выполненных замен
forward stock replaceVal(ar[], arSize, valOld, valNew, start = 0);

//! @brief Заменить определенные значения в подмассиве однобайтных элементов на заданное
//! @param[inout] ar массив
//! @param[in] arSize длина массива
//! @param[in] valOld заменяемое значение
//! @param[in] valNew новое значение
//! @param[in] start индекс начала подмассива
//! @return кол-во выполненных замен
forward stock replaceValStr(ar{}, arSize, valOld, valNew, start = 0);

//! @brief Посчитать количество одинаковых соответствующих элементов
//! @param[in] ar1 первый сравниваемый массив
//! @param[in] ar1start индекс начала подмассива в первом массиве
//! @param[in] ar1size длина первого сравниваемого массива
//! @param[in] ar2 второй сравниваемый массив
//! @param[in] ar2start индекс начала подмассива во втором массиве
//! @param[in] ar2size длина второго сравниваемого массива
forward stock countIdentical(const ar1[], ar1start, ar1size, const ar2[], ar2start, ar2size);

//! @brief Посчитать количество одинаковых соответствующих однобайтных элементов
//! @param[in] ar1 первый сравниваемый массив
//! @param[in] ar1start индекс начала подмассива в первом массиве
//! @param[in] ar1size длина первого сравниваемого массива
//! @param[in] ar2 второй сравниваемый массив
//! @param[in] ar2start индекс начала подмассива во втором массиве
//! @param[in] ar2size длина второго сравниваемого массива
forward stock countIdenticalStr(const ar1{}, ar1start, ar1size, const ar2{}, ar2start, ar2size);

//! @brief Посчитать длину подмассива
//! @param[in] arStart индекс начала подмассива
//! @param[in] arSize длина массива
forward stock getSubSize(arStart, arSize);

//! @brief Удалить последовательные дубликаты заданного значения в массиве однобайтных элементов
//! @param[inout] ar массив
//! @param[inout] arSize длина массива
//! @param[in] value искомое значение
//! @param[in] start стартовая позиция начала поиска
forward stock removeConsecutiveDuplicatesStr(ar{}, &arSize, value, start = 0);
//! @}

//! @defgroup similarElementsOperations Однотипные операции над элементами
//! @{

//! @brief Суммирование элементов подмассива 1-байтных элементов
//! @param[in] ar массив
//! @param[in] arSize размер массива
//! @param[in] start стартовый индекс
forward stock arrayPlusStr(const ar{}, arSize, start = 0);

//! @brief XOR элементов подмассива 1-байтных элементов
//! @param[in] ar массив
//! @param[in] arSize размер массива
//! @param[in] start стартовый индекс
forward stock arrayXorStr(const ar{}, arSize, start = 0);

//! @brief Инвертировать биты в байтах подмассива однобайтных элементов
//! @param[inout] ar массив
//! @param[in] arSize размер массива
//! @param[in] start стартовый индекс подмассива
forward stock arrayInvertBitsStr(ar{}, arSize, start = 0);

//! @brief Очистить подмассив
//! @details заменить элементы на \0
//! @param[in] ar массив
//! @param[in] start позиция начала подмассива
//! @param[in] arSize размер массива
forward stock clearArrayStr(ar{}, arSize, start = 0);

//! @brief Инвертировать подмассив
//! @param[inout] ar исходный массив
//! @param[in] arSize длина массива
//! @param[in] start индекс начала подмассива
forward stock invertArray(ar[], arSize, start = 0);

//! @brief Инвертировать подмассив однобайтных элементов
//! @param[inout] ar исходный массив
//! @param[in] arSize длина массива
//! @param[in] start индекс начала подмассива
forward stock invertArrayStr(ar{}, arSize, start = 0);

//! @brief Кольцевой сдвиг элементов подмассива
//! @param[in] ar изменяемый массив
//! @param[in] arSize длина массива
//! @param[in] toRight признак необходимости сдвига вправо: true - вправо, false - влево
//! @param[in] arStart индекс начала подмассива
//! @param[in] count величина сдвига
forward stock arrayRingShift(ar[], arSize, toRight, arStart = 0, count = 1);

//! @brief Кольцевой сдвиг однобайтных элементов подмассива
//! @param[in] ar изменяемый массив
//! @param[in] arSize длина массива
//! @param[in] toRight признак необходимости сдвига вправо: true - вправо, false - влево
//! @param[in] arStart индекс начала подмассива
//! @param[in] count величина сдвига
forward stock arrayRingShiftStr(ar{}, arSize, toRight, arStart = 0, count = 1);
//! @}

//! @defgroup ar2num Преобразование массив <-> число
//! @{

//! @brief Получить число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[in] bytes разрядность в байтах, 1..4
//! @param[in] littleEnd порядок хранения многобайтных чисел: true - littleEndian, false - BigEndian
//! @param[in] sign наличие знака для 8-, 16- и 24-битного числа: true - знаковое, false - беззнаковое (32-битное число всегда знаковое)
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num(const ar{}, start, arSize, bytes, littleEnd, sign, &num);

//! @brief Получить знаковое 8-битное число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num8sign(const ar{}, start, arSize, &num);

//! @brief Получить беззнаковое 8-битное число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num8unSign(const ar{}, start, arSize, &num);

//! @brief Получить знаковое 16-битное littleEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num16leSign(const ar{}, start, arSize, &num);

//! @brief Получить беззнаковое 16-битное littleEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num16leUnSign(const ar{}, start, arSize, &num);

//! @brief Получить беззнаковое 16-битное BigEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num16beUnSign(const ar{}, start, arSize, &num);

//! @brief Получить знаковое 16-битное BigEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num16beSign(const ar{}, start, arSize, &num);

//! @brief Получить знаковое 24-битное littleEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num24leSign(const ar{}, start, arSize, &num);

//! @brief Получить беззнаковое 24-битное littleEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num24leUnSign(const ar{}, start, arSize, &num);

//! @brief Получить знаковое 24-битное BigEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num24beSign(const ar{}, start, arSize, &num);

//! @brief Получить беззнаковое 24-битное BigEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num24beUnSign(const ar{}, start, arSize, &num);

//! @brief Получить знаковое 32-битное BigEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num32beSign(const ar{}, start, arSize, &num);

//! @brief Получить знаковое 32-битное littleEndian число из массива
//! @param[in] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @param[out] num полученное число
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock array2num32leSign(const ar{}, start, arSize, &num);

//! @brief Поместить число в массив
//! @param[in] num число для конвертации
//! @param[in] bytes кол-во байт в числе, 1..4
//! @param[in] littleEnd порядок хранения в массиве: true - littleEndian, false - BigEndian
//! @param[out] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock num2array(num, bytes, littleEnd, ar{}, start, arSize);

//! @brief Поместить 16-битное число в массив littleEndian
//! @param[in] num число для конвертации
//! @param[out] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock num16bit2arrayLe(num, ar{}, start, arSize);

//! @brief Поместить 16-битное число в массив BigEndian
//! @param[in] num число для конвертации
//! @param[out] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock num16bit2arrayBe(num, ar{}, start, arSize);

//! @brief Поместить 24-битное число в массив littleEndian
//! @param[in] num число для конвертации
//! @param[out] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock num24bit2arrayLe(num, ar{}, start, arSize);

//! @brief Поместить 24-битное число в массив BigEndian
//! @param[in] num число для конвертации
//! @param[out] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock num24bit2arrayBe(num, ar{}, start, arSize);

//! @brief Поместить 32-битное число в массив littleEndian
//! @param[in] num число для конвертации
//! @param[out] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock num32bit2arrayLe(num, ar{}, start, arSize);

//! @brief Поместить 32-битное число в массив BigEndian
//! @param[in] num число для конвертации
//! @param[out] ar массив
//! @param[in] start стартовый индекс массива
//! @param[in] arSize длина массива
//! @return false - неуспешно (некорректные входные данные), true - успешно
forward stock num32bit2arrayBe(num, ar{}, start, arSize);
//! @}

//! @defgroup ar2num Поиск элемента
//! @{

//! @brief Линейный поиск элемента в массиве
//! @param[in] ar массив
//! @param[in] arSize длина массива
//! @param[in] element искомый элемент
//! @param[in] start индекс начала подмассива
//! @param[in] fromEnd признак начала поиска с конца: true - с конца подмассива, false - с начала
//! @return >=0: индекс первого найденного элемента, <0 - не найден
forward stock searchLinear(const ar[], arSize, element, start = 0, fromEnd = false);

//! @brief Линейный поиск в массиве однобайтных элементов
//! @param[in] ar массив
//! @param[in] arSize длина массива
//! @param[in] element искомый элемент
//! @param[in] start индекс начала подмассива
//! @param[in] fromEnd признак начала поиска с конца: true - с конца подмассива, false - с начала
//! @return >=0: индекс первого найденного элемента, <0 - не найден
forward stock searchLinearStr(const ar{}, arSize, element, start = 0, fromEnd = false);

//! @brief Двоичный поиск элемента в массиве
//! @param[in] ar массив
//! @param[in] arSize длина массива
//! @param[in] element искомый элемент
//! @param[in] start стартовый индекс для поиска
//! @return >=0: меньший индекс искомого элемента, <0 - не найден
forward stock searchBinary(const ar[], arSize, element, start = 0);

//! @brief Двоичный поиск в массиве однобайтных элементов
//! @param[in] ar массив
//! @param[in] arSize длина массива
//! @param[in] element искомый элемент
//! @param[in] start стартовый индекс для поиска
//! @return >=0: меньший индекс искомого элемента, <0 - не найден
forward stock searchBinaryStr(const ar{}, arSize, element, start = 0);
//! @}

//! @defgroup searchSubAr Поиск подмассива
//! @{

//! @brief Простой поиск подмассива в массиве
//! @details brute force algorithm, naive algorithm
//! @param[in] ar массив
//! @param[in] arStart стартовый индекс массива для поиска
//! @param[in] arSize длина массива
//! @param[in] sub массив с искомым подмассивом
//! @param[in] subSize длина массива с искомым подмассивом
//! @param[in] subStart стартовый индекс искомого подмассива
//! @return >=0: индекс стартового элемента искомого подмассива в массиве для поиска, ближний к началу массива (т.е. первое вхождение), <0 - не найден
forward stock searchSubArBruteForce(const ar[], arStart, arSize, const sub[], subSize, subStart = 0);

//! @brief Простой поиск подмассива в массиве однобайтных элементов
//! @details brute force algorithm, naive algorithm
//! @param[in] ar массив
//! @param[in] arStart стартовый индекс массива для поиска
//! @param[in] arSize длина массива
//! @param[in] sub массив с искомым подмассивом
//! @param[in] subSize длина массива с искомым подмассивом
//! @param[in] subStart стартовый индекс искомого подмассива
//! @return >=0: индекс стартового элемента искомого подмассива в массиве для поиска, ближний к началу массива (т.е. первое вхождение), <0 - не найден
forward stock searchSubArBruteForceStr(const ar{}, arStart, arSize, const sub{}, subSize, subStart = 0);
//! @}

//! @defgroup minMax Поиск экстремумов
//! @{

//! @brief Поиск минимума в массиве
//! @param[in] ar массив
//! @param[in] arSize размер массива
//! @param[in] start позиция начала подмассива
//! @return -1 - не найден (ошибка входных данных), >=0 - успешно (индекс элемента)
forward stock searchMin(const ar[], arSize, start = 0);

//! @brief Поиск минимума в массиве однобайтных элементов
//! @param[in] ar массив
//! @param[in] arSize размер массива
//! @param[in] start позиция начала подмассива
//! @return -1 - не найден (ошибка входных данных), >=0 - успешно (индекс элемента)
forward stock searchMinStr(const ar{}, arSize, start = 0);

//! @brief Поиск максимума в массиве
//! @param[in] ar массив
//! @param[in] arSize размер массива
//! @param[in] start позиция начала подмассива
//! @return -1 - не найден (ошибка входных данных), >=0 - успешно (индекс элемента)
forward stock searchMax(const ar[], arSize, start = 0);

//! @brief Поиск максимума в массиве однобайтных элементов
//! @param[in] ar массив
//! @param[in] arSize размер массива
//! @param[in] start позиция начала подмассива
//! @return -1 - не найден (ошибка входных данных), >=0 - успешно (индекс элемента)
forward stock searchMaxStr(const ar{}, arSize, start = 0);
//! @}

//! @defgroup sort Сортировки
//! @{

//! @brief Шейкер-сортировка подмассива по возрастанию
//! @details сортировка перемешиванием / двунаправленная / Cocktail sort
//! @param[inout] ar массив для сортировки
//! @param[in] arSize длина массива
//! @param[in] start индекс начала сортируемого подмассива
forward stock shakerSort(ar[], arSize, start = 0);

//! @brief Шейкер-сортировка подмассива однобайтных элементов по возрастанию
//! @details сортировка перемешиванием / двунаправленная / Cocktail sort
//! @param[inout] ar массив для сортировки
//! @param[in] arSize длина массива
//! @param[in] start индекс начала сортируемого подмассива
forward stock shakerSortStr(ar{}, arSize, start = 0);
//! @}

//! @defgroup bitOperations Битовые операции
//! @{

//! @brief Получить значение бита в подмассиве
//! @param[in] ar массив
//! @param[in] start позиция начала подмассива
//! @param[in] arSize размер массива
//! @param[in] bit позиция бита относительно начала подмассива
//! @param[out] value значение бита при успешном возврате
//! @return false - неуспешно (ошибка входных данных), true - успешно
forward stock getBitFromArray(const ar{}, start, arSize, bit, &value);

//! @brief Установить значение бита в подмассиве
//! @param[inout] ar массив
//! @param[in] start позиция начала подмассива
//! @param[in] arSize размер массива
//! @param[in] bit позиция бита относительно начала подмассива
//! @param[in] value устанавливаемое значение бита
//! @return false - неуспешно (ошибка входных данных), true - успешно
forward stock setBitInArray(ar{}, start, arSize, bit, value);
//! @}

//! @defgroup globVarsStore Хранение в глобальных переменных
//! @{

//! Получить массив из глобальных переменных
//! \param[in] dataInVarAddresses адреса глобальных переменных массива исходных данных
//! \param[in] dataInVarAddressesSize количество адресов глобальных переменных массива исходных данных
//! \param[out] dataOut массив выходных данных
//! \param[in] dataOutMaxSize предельный размер массива выходных данных
//! \param[in] useDataInSize признак использования глобальной переменной с текущим размером исходных данных: true - используется, false - нет
//! (для массивов с \0 окончанием - например, для строк)
//! \param[in] dataInSizeVarAddr адрес глобальной переменной с текущим размером исходных данных
//! \param[in] dataOutPos позиция вставки в массив выходных данных
//! \return количество вставленных байт
forward stock getArrayFromGlobalVars(const dataInVarAddresses[], dataInVarAddressesSize, dataOut{}, dataOutMaxSize, useDataInSize = false, dataInSizeVarAddr = 0,
                            dataOutPos = 0);

//! Записать массив в глобальные переменные
//! \param[in] dataOutVarAddresses адреса глобальных переменных для хранения массива
//! \param[in] dataOutVarAddressesSize количество адресов глобальных переменных для хранения массива
//! \param[in] dataIn записываемые данные
//! \param[in] dataInSize размер массива записываемых данных
//! \param[in] useDataOutSize признак использования глобальной переменной с текущим размером выходных данных: true - используется, false - нет
//! (для массивов с \0 окончанием - например, для строк)
//! \param[in] dataOutActualSizeVarAddr адрес глобальной переменной с актуальной длиной массива данных
//! \param[in] dataInPos позиция начала записываемых данных
//! \return количество записанных байт
forward stock setArrayToGlobalVars(const dataOutVarAddresses[], dataOutVarAddressesSize, const dataIn{}, dataInSize, useDataOutSize = false,
                            dataOutActualSizeVarAddr = 0, dataInPos = 0);
//! @}
