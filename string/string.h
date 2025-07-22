#ifndef STRING_H
#define STRING_H
// Заголовок библиотеки для работы со строками

#define NUM_VALUE_MIN_STR "-2147483648"
#define NUM_VALUE_MIN_STR_LENGTH 11
#define NUM_VALUE_MAX_STR "2147483647"
#define NUM_VALUE_MAX_STR_LENGTH 10
#define BASE64_BLOCK_SIZE 4
#define BASE64_PADDING '='
#define BASE64_ALPHABET_SIZE 64
stock const BASE64_ALPHABET{} ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//! Вернуть цифру из ее кода символа ASCII
forward stock getDigit(byte)

//! Проверяет, является ли символ цифрой в кодировке ASCII
//! \param[in] byte предполагаемый символ цифры в ASCII
forward stock isDigit(byte)

//! Посчитать длину символов числа
//! Например -1 -> 2
//! \param[in] num исходное число
//! \return кол-во символов в строковом представлении числа
forward stock numLength(num)

//! Пропустить последовательные пустоты в строке (пробел, табуляция)
//! \param[in] str массив со строкой
//! \param[in] length длина массива со строкой
//! \param[in] pos позиция начала
//! \return кол-во пропущенных символов
forward stock skipSpaces(const str{}, length, pos = 0)

//! Вычисление длины подстроки символов до \0 или до конца массива
//! \param[in] str массив со строкой
//! \param[in] strLength длина массива, если <=0, то игнорируется
//! \param[in] start индекс начала подстроки
forward stock strLen(const str{}, strLength = 0, start = 0)

//! Копирование одной подстроки в другую с ее завершением (если позволяет длина)
//! \param[out] dest массив строки-приемника
//! \param[in] destPos позиция начала вставки в строку-приемник
//! \param[in] destLength длина массива строки-приемника
//! \param[in] source массив строки-источника
//! \param[in] sourcePos начальная позиция в строке-источнике
//! \param[in] sourceLength длина массива строки-источника (если <=0, то будет вычислена)
forward stock strncpy(dest{}, destPos, destLength, const source{}, sourcePos = 0, sourceLength = 0)


//// Преобразование символ <-> другой символ

//! Переводит подстроку к нижнему регистру символов, в т.ч. кириллических
//! \param[inout] str массив со строкой
//! \param[in] strLength длина массива
//! \param[in] start индекс начала подстроки
//! \param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
forward stock toLowerCase(str{}, strLength, start = 0, ignoreNull = false)

//! Переводит подстроку к верхнему регистру символов, в т.ч. кириллических
//! \param[inout] str массив со строкой
//! \param[in] strLength длина массива
//! \param[in] start индекс начала подстроки
//! \param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
forward stock toUpperCase(str{}, strLength, start = 0, ignoreNull = false)

//! Преобразовать нечитаемые символы в пробелы
//! \param[inout] str массив со строкой для преобразования
//! \param[in] strLength длина массива со строкой
//! \param[in] start стартовый индекс начала преобразования
//! \param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
//! \return кол-во преобразований
forward stock unread2space(str{}, strLength, start = 0, ignoreNull = false)


//// Конвертация строка <-> число

//! Преобразование подстроки ASCII дробного числа в целое число, 
//! пробельные символы в начале строки пропускаются
//! \param[in] str массив со строкой
//! \param[in] pos позиция начала числа
//! \param[in] length длина массива со строкой
//! \param[in] separator символ-разделитель целой и дробной частей числа
//! \param[in] precision требуемая точность - кол-во цифр после разделителя целой и дробной частей
//! \param[out] value преобразованное число
//! \return кол-во преобразованных символов подстроки, т.е. если 0 - ошибка
forward stock atofi(const str{}, pos, length, separator, precision, &value)

//! Преобразование подстроки в целое число. 
//! Если встречается нечисловой символ (после начальных "-" или "+", если они есть), преобразование прекращается
//! \param[in] str массив со строкой
//! \param[in] pos позиция начала числа
//! \param[in] length длина массива со строкой
//! \param[out] value преобразованное число
//! \return кол-во преобразованных символов подстроки, т.е. если 0 - ошибка (неверные входные данные, нет числа или неверный формат)
forward stock atoi(const str{}, pos, length, &value)

//! Конвертация десятичной дроби в строку,
//! незначащие нули справа также вставляются
//! \param[in] num исходное целое число
//! \param[in] exp степень числа 10, на которое нужно разделить исходное число (= кол-во знаков, на которое нужно переместить дес. точку влево), 0..9
//! \param[out] str массив со строкой
//! \param[in] pos позиция в строке для вставки
//! \param[in] length длина массива со строкой
//! \param[in] separator символ-разделитель целой и дробной частей
//! \return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки, недопустимая позиция вставки или большая степень)
forward stock decfractoa(num, exp, str{}, pos, length, separator)

//! Конвертация числа в строку символов, 
//! завершающий \0 не добавляется
//! \param[in] num число для конвертации
//! \param[out] str массив со строкой
//! \param[in] pos позиция в строке для вставки
//! \param[in] length длина массива со строкой
//! \return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки или неверная позиция вставки)
forward stock itoa(num, str{}, pos, length)

//! Преобразовать число в строку и дополнить слева незначащими нулями
//! \param[in] num число для конвертации
//! \param[out] str массив со строкой
//! \param[in] pos позиция в строке для вставки
//! \param[in] length длина массива со строкой
//! \param[in] width ширина числа, если больше строкового представления, то будет дополнена справа нулями; если меньше, чем символов в числе, то игнорируется
//! \return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки или неверная позиция вставки)
forward stock itoaw(num, str{}, pos, length, width)

//! Разобрать целые числа из строки с заданными разделителями вида [<разделитель>]<число>[<разделитель><число>... ]
//! несколько разделителей подряд не допускаются, преобразование прекращается на нечисловом символе (+, -, пробельные символы перед числом допускаются)
//! \param[in] str строка
//! \param[in] strSize длина строки
//! \param[in] pos позиция начала в строке
//! \param[in] separator символ-разделитель
//! \param[out] values найденные числа
//! \param[in] valuesMaxSize предельное кол-во для найденных значений
//! \param[out] valuesActualSize найденное кол-во чисел
//! \return кол-во обработанных символов в строке
forward stock strSplitNums(const str{}, strSize, pos, separator, values[], valuesMaxSize, &valuesActualSize)


//// ASCII-hex

//! Преобразовать ASCII-hex символ в полубайт числа
forward stock asciiHex2num(byte)

//! Получить значения из их ASCII-hex строкового представления
//! \return кол-во вставленных байт
forward stock asciiHexStr2array(const asciiStr{}, asciiStrSize, asciiStrPos, hex{}, hexMaxSize)

//! Получить hex строковое представление подмассива 
//! (например, {0, 1, 171, 255} -> "0001ABFF")
//! \param[in] ar исходный массив
//! \param[in] arStart индекс начала конвертируемой части в исходном массиве
//! \param[in] arSize длина исходного массива
//! \param[out] str строка для вставки конвертации
//! \param[in] strSize длина конвертированной строки, должно соблюдаться (strSize - strPos) >= 2 * (arSize - arStart)
//! \param[in] strPos позиция для вставки
//! \return кол-во вставленных байт
forward stock getHexString(const ar{}, arStart, arSize, str{}, strSize, strPos = 0)

//! Проверить символ на принадлежность к ASCII-hex (0..9,A..F,a..f)
forward stock isAsciiHex(byte)

//! Преобразует последний полубайт числа в его ASCII-hex символ
forward stock num2asciiHexHalfByte(num)

//! Преобразовывает каждый элемент подмассива в его ASCII-hex 2-символьное представление
//! \param[inout] ar преобразуемый массив
//! \param[in] arLength длина массива
//! \param[in] subLength длина подмассива
//! \param[in] start стартовый индекс подмассива
//! \return true - успешно, false - ошибка (недостаточная длина массива)
forward stock toAsciiHex(ar{}, arLength, subLength, start = 0)


//// Base64

//! Закодировать в Base64 (RFC 4648)
//! \param[in] in массив для кодирования
//! \param[in] inSize длина массива для кодирования
//! \param[in] inPos позиция начала в массиве для кодирования
//! \param[out] out выходной массив для закодированных данных
//! \param[in] outSize длина массива для закодированных данных
//! \param[in] outPos позиция в массиве для закодированных данных
//! \return кол-во вставленных символов в выходной массив
forward stock base64Encode(const in{}, inSize, inPos, out{}, outSize, outPos = 0)

//! Проверка символа на принадлежность алфавиту base64
forward stock isBase64(ch)

//! Декодировать из Base64 (RFC 4648)
//! \param[in] in массив закодированных данных
//! \param[in] inSize длина массива закодированных данных
//! \param[in] inPos позиция начала в массиве закодированных данных
//! \param[out] out выходной массив для декодированных данных
//! \param[in] outSize длина массива декодированных данных
//! \param[in] outPos позиция в массиве декодированных данных
//! \return кол-во вставленных символов в выходной массив
forward stock base64Decode(const in{}, inSize, inPos, out{}, outSize, outPos = 0)

//! Вычислить длину закодированного массива в base64 по длине исходного
forward stock base64StrLength(inSize)

//! Вычислить максимальную длину для декодированной строки из base64 по длине кодированной
forward stock fromBase64StrMaxSize(base64Size)

//! Вычислить минимальную длину для декодированной строки из base64 по длине кодированной
forward stock fromBase64StrMinSize(base64Size)

#endif // STRING_H
