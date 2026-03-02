//! @file
//! @brief Заголовок библиотеки для работы со строками

#if defined STRING_H
#endinput
#endif
#define STRING_H

//! @defgroup strnum Конвертация строка <-> число
//! @{
#define NUM_VALUE_MIN_STR "-2147483648"
#define NUM_VALUE_MIN_STR_LENGTH 11
#define NUM_VALUE_MAX_STR "2147483647"
#define NUM_VALUE_MAX_STR_LENGTH 10
//! @}

//! @defgroup base64 Base64
//! @{
#define BASE64_BLOCK_SIZE 4
#define BASE64_PADDING '='
#define BASE64_ALPHABET_SIZE 64
stock const BASE64_ALPHABET{} ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//! @}

//! @defgroup symbols Символы

//! @defgroup symbolsASCII ASCII (ISO 8859-1)
//! @ingroup symbols
//! @{
#define SYMBOL_NUL 0x00
#define SYMBOL_HT '\t'
#define SYMBOL_LF 0x0A
#define SYMBOL_CR 0x0D
#define SYMBOL_US 0x1F

#define SYMBOL_SPACE ' '
#define SYMBOL_PLUS '+'
#define SYMBOL_MINUS '-'

#define SYMBOL_DIGIT_0 '0'
#define SYMBOL_DIGIT_7 '7'
#define SYMBOL_DIGIT_9 '9'

#define SYMBOL_LATIN_CAPITAL_LETTER_A 0x41
#define SYMBOL_LATIN_CAPITAL_LETTER_F 0x46
#define SYMBOL_LATIN_CAPITAL_LETTER_Z 0x5A
#define SYMBOL_LATIN_SMALL_LETTER_A 0x61
#define SYMBOL_LATIN_SMALL_LETTER_F 0x66
#define SYMBOL_LATIN_SMALL_LETTER_Z 0x7A

#define SYMBOL_DEL 0x7F
//! @}

//! @defgroup symbolsCpWin1251 Windows CP-1251
//! @ingroup symbols
//! @{
#define SYMBOL_CYRILLIC_CAPITAL_LETTER_IO 0xA8
#define SYMBOL_CYRILLIC_SMALL_LETTER_IO 0xB8
#define SYMBOL_CYRILLIC_CAPITAL_LETTER_A 0xC0
#define SYMBOL_CYRILLIC_CAPITAL_LETTER_YA 0xDF
#define SYMBOL_CYRILLIC_SMALL_LETTER_A 0xE0
#define SYMBOL_CYRILLIC_SMALL_LETTER_YA 0xFF
//! @}

//! @defgroup general Общие функции
//! @{

//! @brief Вернуть цифру из ее кода символа ASCII
//! @details Например, 0x31 -> 1
forward stock getDigit(byte);

//! @brief Проверить, является ли символ цифрой в кодировке ASCII
//! @details Например, 0x30 -> true, 0x20 -> false
forward bool:stock isDigit(byte);

//! @brief Вычислить длину строкового представления числа
//! @details Например, "-1" -> 2
forward stock numLength(num);

//! @brief Пропустить последовательные пустоты в строке (пробел, табуляция)
//! @details Например, "   -123", .pos=0 -> 3
//! @param[in] str массив со строкой
//! @param[in] strLength длина массива со строкой; если <=0, то длина строки будет вычислена
//! @param[in] pos позиция начала
//! @return кол-во пропущенных символов
forward stock skipSpaces(const str{}, strLength = 0, pos = 0);

//! @brief Вычислить длину подстроки символов
//! @details Например, "abcd" -> 4
//! @details до \0 или до конца массива
//! @param[in] str массив со строкой
//! @param[in] strLength длина массива; если <=0, то игнорируется
//! @param[in] start индекс начала подстроки
forward stock strLen(const str{}, strLength = 0, start = 0);

//! @brief Копировать одну подстроку в другую строку с ее завершением (если позволяет длина)
//! @details Например, .dest="abcdef",.destPos=2,.destLength=6,.source="gh" -> .dest=="abgh"
//! @param[out] dest массив строки-приемника
//! @param[in] destPos позиция начала вставки в строку-приемник
//! @param[in] destLength длина массива строки-приемника
//! @param[in] source массив строки-источника
//! @param[in] sourcePos начальная позиция в строке-источнике
//! @param[in] sourceLength длина массива строки-источника (если <=0, то будет вычислена)
//! @param[in] fromBack признак необходимости вставки с конца источника (например, при совпадении источника с приемником)
//! @return длина вставленной строки
forward stock strncpy(dest{}, destPos, destLength, const source{}, sourcePos = 0, sourceLength = 0, bool:fromBack = false);
//! @}

//! @defgroup sym2anotherSym Преобразование символ <-> другой символ
//! @{

//! @brief Перевести подстроку к нижнему регистру символов
//! @details в т.ч. кириллические. Например, "АвтоVaz" -> "автоvaz"
//! @param[inout] str массив со строкой
//! @param[in] strLength длина массива со строкой; если <=0, то длина строки будет вычислена
//! @param[in] start индекс начала подстроки
forward stock toLowerCase(str{}, strLength = 0, start = 0);

//! @brief Перевести подстроку к верхнему регистру символов
//! @details в т.ч. кириллические. Например, "АвтоVaz" -> "АВТОVAZ"
//! @param[inout] str массив со строкой
//! @param[in] strLength длина массива со строкой; если <=0, то длина строки будет вычислена
//! @param[in] start индекс начала подстроки
forward stock toUpperCase(str{}, strLength = 0, start = 0);

//! @brief Преобразовать нечитаемые символы в пробелы
//! @details Например, {0x30, 0x05, 0x1F, 0x20, 0x7F, 0x31} -> {0x30, 0x20, 0x20, 0x20, 0x20, 0x31}
//! @param[inout] str массив со строкой для преобразования
//! @param[in] strLength длина массива со строкой; если <=0, то длина строки будет вычислена
//! @param[in] start стартовый индекс начала преобразования
//! @param[in] ignoreNull признак необходимости игнорирования промежуточных \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
//! @return кол-во преобразований
forward stock unread2space(str{}, strLength = 0, start = 0, bool:ignoreNull = false);
//! @}

//! @addtogroup strnum
//! @{

//! @brief Преобразовать подстроку ASCII дробного числа в целое число
//! @details Пробелы и табуляции в начале строки пропускаются. 
//! @details Например: .str="- 54,321", .separator=',', .precision=2 -> -5432
//! @param[in] str массив со строкой
//! @param[in] separator символ-разделитель целой и дробной частей числа
//! @param[in] precision требуемая точность - кол-во цифр после разделителя целой и дробной частей
//! @param[out] value преобразованное число
//! @param[in] strLength длина массива со строкой; если <=0, то длина строки будет вычислена
//! @param[in] pos позиция начала числа
//! @return кол-во преобразованных символов подстроки, т.е. если 0 - ошибка
forward stock atofi(const str{}, separator, precision, &value, strLength = 0, pos = 0);

//! @brief Преобразовать подстроку ASCII в целое число
//! @details Пробелы и табуляции в начале строки пропускаются. 
//! @details Если встречается нечисловой символ (после начальных "-" или "+", если они есть), преобразование прекращается. 
//! @details Например: "-123" -> 123; "  +4" -> 4
//! @param[in] str массив со строкой
//! @param[out] value преобразованное число
//! @param[in] strLength длина массива со строкой; если <=0, то длина строки будет вычислена
//! @param[in] pos позиция начала числа
//! @return кол-во преобразованных символов подстроки, т.е. если 0 - ошибка (неверные входные данные, нет числа или неверный формат)
forward stock atoi(const str{}, &value, strLength = 0, pos = 0);

//! @brief Преобразовать десятичную дробь в строку
//! @details Незначащие нули справа также вставляются, по возможности строка оконцовывается \0. 
//! @details Например: .num=-123456,.exp=2,.separator=',' -> "-1234,56"; .num=100,.exp=1,.separator=',' -> "10,0"
//! @param[in] num исходное целое число
//! @param[in] exp степень числа 10, на которое нужно разделить исходное число (= кол-во знаков, на которое нужно переместить дес. точку влево), 0..9
//! @param[out] str массив со строкой
//! @param[in] strMaxSize максимальный размер массива со строкой
//! @param[in] separator символ-разделитель целой и дробной частей
//! @param[in] pos позиция в строке для вставки
//! @return размер вставленной строки (0 - ошибка: недостаточная длина строки, недопустимая позиция вставки или большая степень)
forward stock decfractoa(num, exp, str{}, strMaxSize, separator, pos = 0);

//! @brief Преобразовать число в строку
//! @details По возможности строка оконцовывается \0. 
//! @details Например: -12345 -> "-12345"
//! @param[in] num число для конвертации
//! @param[out] str массив со строкой
//! @param[in] strMaxSize максимальный размер массива со строкой
//! @param[in] pos позиция в строке для вставки
//! @return размер вставленной строки (0 - ошибка: недостаточная длина строки или неверная позиция вставки)
forward stock itoa(num, str{}, strMaxSize, pos = 0);

//! @brief Преобразовать число в строку и дополнить слева незначащими нулями
//! @details По возможности строка оконцовывается \0. 
//! @details Например: .num=-123,.width=5 -> "-0123"; .num=-123,.width=1 -> "-123"
//! @param[in] num число для конвертации
//! @param[out] str массив со строкой
//! @param[in] strMaxSize максимальный размер массива со строкой
//! @param[in] width ширина числа; если больше строкового представления, то будет дополнена слева нулями, если меньше, чем символов в числе, то игнорируется
//! @param[in] pos позиция в строке для вставки
//! @return размер вставленной строки (0 - ошибка: недостаточная длина строки или неверная позиция вставки)
forward stock itoaw(num, str{}, strMaxSize, width = 0, pos = 0);

//! @brief Разобрать целые числа из строки с заданными разделителями вида [<разделитель>]<число>[<разделитель><число>... ]
//! @details Несколько разделителей подряд не допускаются. 
//! @details Преобразование прекращается на нечисловом символе (+, -, пробельные символы перед числом допускаются). 
//! @details Например: .str="12,+34,  -56,abc,78",.separator=',' -> [12, 34, -56]
//! @param[in] str массив со строкой
//! @param[in] separator символ-разделитель
//! @param[out] values найденные числа
//! @param[in] valuesMaxSize предельное кол-во для найденных чисел
//! @param[out] valuesActualSize найденное кол-во чисел
//! @param[in] strLength длина массива со строкой; если <=0, то длина строки будет вычислена
//! @param[in] pos позиция начала в строке
//! @return кол-во обработанных символов в строке
forward stock strSplitNums(const str{}, separator, values[], valuesMaxSize, &valuesActualSize, strLength = 0, pos = 0);
//! @}

//! @defgroup asciihex Преобразования ASCII-hex 
//! @{

//! @brief Преобразовать ASCII-hex символ в полубайт числа
//! @details Например: 0x31 -> 0x01; 0x41 -> 0x0A; 0x61 -> 0x0a
forward stock asciiHex2num(byte);

//! @brief Получить значения из их ASCII-hex строкового представления
//! @details Преобразование прекращается на не-ASCII-hex символе. Если ASCII-hex символов нечетное кол-во, то младший полубайт первого числа считается за 0
//! @details Например: "0001ABFf" -> {0x00, 0x01, 0xAB, 0xFF}; "2abFEgh12" -> {0x02, 0xAB, 0xFE}
//! @param[in] asciiStr строка ASCII-hex символов
//! @param[out] hex массив полученных значений
//! @param[in] hexMaxSize предельный размер массива полученных значений
//! @param[in] asciiStrLength длина строки ASCII-hex символов; если <=0, то длина строки будет вычислена
//! @param[in] asciiStrPos начало подстроки
//! @return кол-во полученных значений (вставленных байт)
forward stock asciiHexStr2array(const asciiStr{}, hex{}, hexMaxSize, asciiStrLength = 0, asciiStrPos = 0);

//! @brief Получить hex строковое представление подмассива 
//! @details (например, {0, 1, 171, 255} -> "0001ABFF")
//! @param[in] ar исходный массив
//! @param[in] arStart индекс начала конвертируемой части в исходном массиве
//! @param[in] arSize длина исходного массива
//! @param[out] str строка для вставки конвертации
//! @param[in] strSize длина конвертированной строки, должно соблюдаться (strSize - strPos) >= 2 * (arSize - arStart)
//! @param[in] strPos позиция начала вставки
//! @return кол-во вставленных байт
forward stock getHexString(const ar{}, arStart, arSize, str{}, strSize, strPos = 0);

//! @brief Проверить символ на принадлежность к ASCII-hex (0..9,A..F,a..f)
forward stock isAsciiHex(byte);

//! @brief Преобразовать последний полубайт числа в его ASCII-hex символ
forward stock num2asciiHexHalfByte(num);

//! @brief Преобразовать каждый элемент подмассива в его ASCII-hex 2-символьное представление
//! @param[inout] ar преобразуемый массив
//! @param[in] arLength длина массива
//! @param[in] subLength длина подмассива
//! @param[in] start стартовый индекс подмассива
//! @return true - успешно, false - ошибка (недостаточная длина массива)
forward bool:stock toAsciiHex(ar{}, arLength, subLength, start = 0);
//! @}

//! @addtogroup base64
//! @{

//! @brief Закодировать в Base64
//! @details RFC 4648
//! @param[in] in массив для кодирования
//! @param[in] inSize длина массива для кодирования
//! @param[in] inPos позиция начала в массиве для кодирования
//! @param[out] out выходной массив для закодированных данных
//! @param[in] outSize длина массива для закодированных данных
//! @param[in] outPos позиция в массиве для закодированных данных
//! @return кол-во вставленных символов в выходной массив
forward stock base64Encode(const in{}, inSize, inPos, out{}, outSize, outPos = 0);

//! @brief Проверка символа на принадлежность алфавиту base64
forward bool:stock isBase64(byte);

//! @brief Декодировать из Base64
//! @details RFC 4648
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
//! \param[in] strMaxSize предельный размер массива для строки (если не указан или <=0, то будет вычислена длина)
//! \param[in] pos позиция начала в записываемой строке
//! \return длина записанной подстроки
forward stock setStrToGlobalVars(const dataOutVarAddresses[], dataOutVarAddressesSize, const str{}, strMaxSize = 0, pos = 0);
//! @}
