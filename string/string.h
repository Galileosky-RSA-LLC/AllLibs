//! @file
//! @brief Заголовок библиотеки для работы со строками

#ifdef STRING_H
#endinput
#endif
#define STRING_H

#define NUM_VALUE_MIN_STR "-2147483648"
#define NUM_VALUE_MIN_STR_LENGTH 11
#define NUM_VALUE_MAX_STR "2147483647"
#define NUM_VALUE_MAX_STR_LENGTH 10

#define BASE64_BLOCK_SIZE 4
#define BASE64_PADDING '='
#define BASE64_ALPHABET_SIZE 64
stock const BASE64_ALPHABET{} ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define SYMBOL_CR 0x0D
#define SYMBOL_LF 0x0A

//! @defgroup general Общие функции
//! @{

//! @brief Вернуть цифру из ее кода символа ASCII
forward stock getDigit(byte);

//! @brief Проверяет, является ли символ цифрой в кодировке ASCII
//! @param[in] byte предполагаемый символ цифры в ASCII
forward stock isDigit(byte);

//! @brief Посчитать длину символов числа
//! @details Например -1 -> 2
//! @param[in] num исходное число
//! @return кол-во символов в строковом представлении числа
forward stock numLength(num);

//! @brief Пропустить последовательные пустоты в строке (пробел, табуляция)
//! @param[in] str массив со строкой
//! @param[in] length длина массива со строкой
//! @param[in] pos позиция начала
//! @return кол-во пропущенных символов
forward stock skipSpaces(const str{}, length, pos = 0);

//! @brief Вычисление длины подстроки символов
//! @details до \0 или до конца массива
//! @param[in] str массив со строкой
//! @param[in] strLength длина массива, если <=0, то игнорируется
//! @param[in] start индекс начала подстроки
forward stock strLen(const str{}, strLength = 0, start = 0);

//! @brief Копирование одной подстроки в другую с ее завершением (если позволяет длина)
//! @param[out] dest массив строки-приемника
//! @param[in] destPos позиция начала вставки в строку-приемник
//! @param[in] destLength длина массива строки-приемника
//! @param[in] source массив строки-источника
//! @param[in] sourcePos начальная позиция в строке-источнике
//! @param[in] sourceLength длина массива строки-источника (если <=0, то будет вычислена)
//! @param[in] fromBack признак необходимости вставки с конца источника (при совпадении источника с приемником)
forward stock strncpy(dest{}, destPos, destLength, const source{}, sourcePos = 0, sourceLength = 0, fromBack = false);
//! @}


//! @defgroup sym2anotherSym Преобразование символ <-> другой символ
//! @{

//! @brief Переводит подстроку к нижнему регистру символов
//! @details в т.ч. кириллические
//! @param[inout] str массив со строкой
//! @param[in] strLength длина массива
//! @param[in] start индекс начала подстроки
//! @param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
forward stock toLowerCase(str{}, strLength, start = 0, ignoreNull = false);

//! @brief Переводит подстроку к верхнему регистру символов
//! @details в т.ч. кириллические
//! @param[inout] str массив со строкой
//! @param[in] strLength длина массива
//! @param[in] start индекс начала подстроки
//! @param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
forward stock toUpperCase(str{}, strLength, start = 0, ignoreNull = false);

//! @brief Преобразовать нечитаемые символы в пробелы
//! @param[inout] str массив со строкой для преобразования
//! @param[in] strLength длина массива со строкой
//! @param[in] start стартовый индекс начала преобразования
//! @param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
//! @return кол-во преобразований
forward stock unread2space(str{}, strLength, start = 0, ignoreNull = false);
//! @}


//! @defgroup strnum Конвертация строка <-> число
//! @{

//! @brief Преобразование подстроки ASCII дробного числа в целое число
//! @details Пробельные символы в начале строки пропускаются
//! @param[in] str массив со строкой
//! @param[in] pos позиция начала числа
//! @param[in] length длина массива со строкой
//! @param[in] separator символ-разделитель целой и дробной частей числа
//! @param[in] precision требуемая точность - кол-во цифр после разделителя целой и дробной частей
//! @param[out] value преобразованное число
//! @return кол-во преобразованных символов подстроки, т.е. если 0 - ошибка
forward stock atofi(const str{}, pos, length, separator, precision, &value);

//! @brief Преобразование подстроки в целое число
//! @details Если встречается нечисловой символ (после начальных "-" или "+", если они есть), преобразование прекращается
//! @param[in] str массив со строкой
//! @param[in] pos позиция начала числа
//! @param[in] length длина массива со строкой
//! @param[out] value преобразованное число
//! @return кол-во преобразованных символов подстроки, т.е. если 0 - ошибка (неверные входные данные, нет числа или неверный формат)
forward stock atoi(const str{}, pos, length, &value);

//! @brief Конвертация десятичной дроби в строку
//! @details Незначащие нули справа также вставляются
//! @param[in] num исходное целое число
//! @param[in] exp степень числа 10, на которое нужно разделить исходное число (= кол-во знаков, на которое нужно переместить дес. точку влево), 0..9
//! @param[out] str массив со строкой
//! @param[in] pos позиция в строке для вставки
//! @param[in] length длина массива со строкой
//! @param[in] separator символ-разделитель целой и дробной частей
//! @return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки, недопустимая позиция вставки или большая степень)
forward stock decfractoa(num, exp, str{}, pos, length, separator);

//! @brief Конвертация числа в строку символов
//! @details завершающий \0 не добавляется
//! @param[in] num число для конвертации
//! @param[out] str массив со строкой
//! @param[in] pos позиция в строке для вставки
//! @param[in] length длина массива со строкой
//! @return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки или неверная позиция вставки)
forward stock itoa(num, str{}, pos, length);

//! @brief Преобразовать число в строку и дополнить слева незначащими нулями
//! @param[in] num число для конвертации
//! @param[out] str массив со строкой
//! @param[in] pos позиция в строке для вставки
//! @param[in] length длина массива со строкой
//! @param[in] width ширина числа, если больше строкового представления, то будет дополнена справа нулями; если меньше, чем символов в числе, то игнорируется
//! @return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки или неверная позиция вставки)
forward stock itoaw(num, str{}, pos, length, width);

//! @brief Разобрать целые числа из строки с заданными разделителями вида [<разделитель>]<число>[<разделитель><число>... ]
//! @details Несколько разделителей подряд не допускаются, преобразование прекращается на нечисловом символе (+, -, пробельные символы перед числом допускаются)
//! @param[in] str строка
//! @param[in] strSize длина строки
//! @param[in] pos позиция начала в строке
//! @param[in] separator символ-разделитель
//! @param[out] values найденные числа
//! @param[in] valuesMaxSize предельное кол-во для найденных значений
//! @param[out] valuesActualSize найденное кол-во чисел
//! @return кол-во обработанных символов в строке
forward stock strSplitNums(const str{}, strSize, pos, separator, values[], valuesMaxSize, &valuesActualSize);
//! @}


//! @defgroup asciihex Преобразования ASCII-hex 
//! @{

//! @brief Преобразовать ASCII-hex символ в полубайт числа
forward stock asciiHex2num(byte);

//! @brief Получить значения из их ASCII-hex строкового представления
//! @return кол-во вставленных байт
forward stock asciiHexStr2array(const asciiStr{}, asciiStrSize, asciiStrPos, hex{}, hexMaxSize);

//! @brief Получить hex строковое представление подмассива 
//! @details (например, {0, 1, 171, 255} -> "0001ABFF")
//! @param[in] ar исходный массив
//! @param[in] arStart индекс начала конвертируемой части в исходном массиве
//! @param[in] arSize длина исходного массива
//! @param[out] str строка для вставки конвертации
//! @param[in] strSize длина конвертированной строки, должно соблюдаться (strSize - strPos) >= 2 * (arSize - arStart)
//! @param[in] strPos позиция для вставки
//! @return кол-во вставленных байт
forward stock getHexString(const ar{}, arStart, arSize, str{}, strSize, strPos = 0);

//! @brief Проверить символ на принадлежность к ASCII-hex (0..9,A..F,a..f)
forward stock isAsciiHex(byte);

//! @brief Преобразует последний полубайт числа в его ASCII-hex символ
forward stock num2asciiHexHalfByte(num);

//! @brief Преобразовывает каждый элемент подмассива в его ASCII-hex 2-символьное представление
//! @param[inout] ar преобразуемый массив
//! @param[in] arLength длина массива
//! @param[in] subLength длина подмассива
//! @param[in] start стартовый индекс подмассива
//! @return true - успешно, false - ошибка (недостаточная длина массива)
forward stock toAsciiHex(ar{}, arLength, subLength, start = 0);
//! @}


//! @defgroup base64 Base64 
//! @{

//! @brief Закодировать в Base64 (RFC 4648)
//! @param[in] in массив для кодирования
//! @param[in] inSize длина массива для кодирования
//! @param[in] inPos позиция начала в массиве для кодирования
//! @param[out] out выходной массив для закодированных данных
//! @param[in] outSize длина массива для закодированных данных
//! @param[in] outPos позиция в массиве для закодированных данных
//! @return кол-во вставленных символов в выходной массив
forward stock base64Encode(const in{}, inSize, inPos, out{}, outSize, outPos = 0);

//! @brief Проверка символа на принадлежность алфавиту base64
forward stock isBase64(ch);

//! @brief Декодировать из Base64 (RFC 4648)
//! @param[in] in массив закодированных данных
//! @param[in] inSize длина массива закодированных данных
//! @param[in] inPos позиция начала в массиве закодированных данных
//! @param[out] out выходной массив для декодированных данных
//! @param[in] outSize длина массива декодированных данных
//! @param[in] outPos позиция в массиве декодированных данных
//! @return кол-во вставленных символов в выходной массив
forward stock base64Decode(const in{}, inSize, inPos, out{}, outSize, outPos = 0);

//! @brief Вычислить длину закодированного массива в base64 по длине исходного
forward stock base64StrLength(inSize);

//! @brief Вычислить максимальную длину для декодированной строки из base64 по длине кодированной
forward stock fromBase64StrMaxSize(base64Size);

//! @brief Вычислить минимальную длину для декодированной строки из base64 по длине кодированной
forward stock fromBase64StrMinSize(base64Size);
//! @}

//! @defgroup globVarsStore Хранение строки в глобальных переменных
//! @{

//! Получить строку из глобальных переменных
//! \param[in] dataInVarAddresses адреса глобальных переменных для хранения строки
//! \param[in] dataInVarAddressesSize количество адресов глобальных переменных для хранения строки
//! \param[out] str выходная строка
//! \param[in] strMaxSize предельный размер массива для строки
//! \param[in] pos позиция вставки в строку
//! \return длина вставленной подстроки
forward stock getStrFromGlobalVars(const dataInVarAddresses[], dataInVarAddressesSize, str{}, strMaxSize, pos = 0);

//! Записать строку в глобальные переменные
//! \param[in] dataOutVarAddresses адреса глобальных переменных для хранения строки
//! \param[in] dataOutVarAddressesSize количество адресов глобальных переменных для хранения строки
//! \param[in] str записываемая строка
//! \param[in] strMaxSize предельный размер массива для строки
//! \param[in] pos позиция начала в записываемой строке
//! \return длина записанной подстроки
forward stock setStrToGlobalVars(const dataOutVarAddresses[], dataOutVarAddressesSize, const str{}, strMaxSize, pos = 0);
//! @}
