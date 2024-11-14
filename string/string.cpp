#ifndef STRING_LIB
#define STRING_LIB
// Библиотека для работы со строками

#include "string.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"


//! Переводит подстроку к нижнему регистру символов, в т.ч. кириллических
//! \param[inout] str массив со строкой
//! \param[in] strLength длина массива
//! \param[in] start индекс начала подстроки
//! \param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
toLowerCase(str{}, strLength, start = 0, ignoreNull = false)
{
    if (start < 0)
        start = 0;

    for (new i = start; i < strLength; i++)
    {    
        if ((str{i} >= 0x41) && (str{i} <= 0x5A) || (str{i} >= 0xC0) && (str{i} <= 0xDF))
            str{i} += 0x20; // латиница или А..Е, Ж..Я
        else if (str{i} == 0xA8)
            str{i} = 0xB8; // Ё
        else if ((str{i} == 0) && !ignoreNull)
            break;
    }
}

//! Переводит подстроку к верхнему регистру символов, в т.ч. кириллических
//! \param[inout] str массив со строкой
//! \param[in] strLength длина массива
//! \param[in] start индекс начала подстроки
//! \param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
toUpperCase(str{}, strLength, start = 0, ignoreNull = false)
{
    if (start < 0)
        start = 0;

    for (new i = start; i < strLength; i++)
    {    
        if ((str{i} >= 0x61) && (str{i} <= 0x7A) || (str{i} >= 0xE0) && (str{i} <= 0xFF))
            str{i} -= 0x20; // латиница или а..е, ж..я
        else if (str{i} == 0xB8)
            str{i} = 0xA8; // ё
        else if ((str{i} == 0) && !ignoreNull)
            break;
    }
}

//! Вычисление длины подстроки символов до \0 или до конца массива, если \0 не встретился
//! \param[in] str массив со строкой
//! \param[in] strLength длина массива
//! \param[in] start индекс начала подстроки
strLen(const str{}, strLength, start = 0)
{
    if (start < 0)
        start = 0;

    new i;
    for (i = start; i < strLength; i++)
    {    
        if (str{i} == 0)
            break;
    }
    return i;
}

//! Замена одной подстроки на другую
//! \param[out] strDest массив строки-приемника
//! \param[in] strDestPos позиция начала вставки в строку-приемник
//! \param[in] strDestLength длина массива строки-приемника
//! \param[in] strSource массив строки-источника
//! \param[in] strSourceLength длина массива строки-источника
//! \param[in] strSourcePos начальная позиция в строке-источнике
//! \return число замененных символов или =1, если исходная строка пустая
replaceStr(strDest{}, strDestPos, strDestLength, const strSource{}, strSourceLength, strSourcePos = 0)
{
    if (strDestPos < 0)
        strDestPos = 0;

    if (strSourcePos < 0)
        strSourcePos = 0;

    new i;
	for (i = 0; ((strDestPos + i) < strDestLength) && ((strSourcePos + i) < strSourceLength) && (i < (strSourceLength - strSourcePos)); i++)
	{	
        strDest{strDestPos + i} = strSource{strSourcePos + i};
        if (strSource{strSourcePos + i} == 0)
        {
            if (i == 0)
                i = 1;

            break;
        }
    }
	return i;
}

//! Преобразовать нечитаемые символы в пробелы
//! \param[inout] str массив со строкой для преобразования
//! \param[in] strLength длина массива со строкой
//! \param[in] start стартовый индекс начала преобразования
//! \param[in] ignoreNull признак необходимости игнорирования \0: false - преобразование заканчивается на \0, true - продолжается до конца массива
//! \return кол-во преобразований
unread2space(str{}, strLength, start = 0, ignoreNull = false)
{
    if (start < 0)
        start = 0;

    new count;
    for (new i = start; i < strLength; i++)
    {
        if ((str{i} == 0x00) && !ignoreNull)
            break;

        if (((str{i} >= 0x00) && (str{i} <= 0x1F)) || (str{i} == 0x7F))
        {    
            str{i} = ' ';
            count++;
        }
    }
    return count;
}

//! Конвертация десятичной дроби в строку,
//! незначащие нули справа также вставляются
//! \param[in] num исходное целое число
//! \param[in] exp степень числа 10, на которое нужно разделить исходное число (= кол-во знаков, на которое нужно переместить дес. точку влево), 0..9
//! \param[out] str массив со строкой
//! \param[in] pos позиция в строке для вставки
//! \param[in] length длина массива со строкой
//! \param[in] separator символ-разделитель целой и дробной частей
//! \return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки, недопустимая позиция вставки или большая степень)
decfractoa(num, exp, str{}, pos, length, separator)
{
    if (pos < 0)
        pos = 0;
    
    if (exp <= 0)
        return itoa(num, str, pos, length);

    if ((exp > 9) || ((pos + 3) > length))
        return 0;

    new divider = pow(10, exp);
    new isMinus = num < 0;
    if (isMinus)
        str{pos++} = '-';

    new len = itoa((num == NUM_VALUE_MIN ? -(num + 1) : (isMinus ? -num : num)) / divider, str, pos, length);
    if (len == 0)
        return 0;

    pos += len;
    if (exp + pos + 1 > length)
        return 0;

    str{pos++} = separator;
    num %= divider;
    if (isMinus)
        num = divider - num;

    new zeros = exp - digits(num);
    for (new i = 0; (i < zeros) && (pos < length); i++)
        str{pos++} = '0';

    return (isMinus ? 1 : 0) + len + 1 + zeros + itoa(num, str, pos, length);
}

//! Конвертация числа в строку символов, 
//! завершающий \0 не добавляется
//! \param[in] num число для конвертации
//! \param[out] str массив со строкой
//! \param[in] pos позиция в строке для вставки
//! \param[in] length длина массива со строкой
//! \return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки или неверная позиция вставки)
itoa(num, str{}, pos, length)
{
    new countDigits = digits(num);
    new isMinus = num < 0;
    if (pos < 0)
        pos = 0;
    
    if ((pos + countDigits + (isMinus ? 1 : 0)) > length)
        return 0;

    if (num == 0)
    {
        str{pos} = '0';
        return 1;
    }
    if (num == NUM_VALUE_MIN)
        return insertArrayStr(str, pos, length, NUM_VALUE_MIN_STR, NUM_VALUE_MIN_STR_LENGTH);
    
    if (isMinus) 
    {
        num *= -1;
        str{pos++} = '-';
    }
    new divider = 1;
    for (new i = 0; i < (countDigits - 1); i++)
        divider *= 10;

    for (new i = 0; i < countDigits; i++)
    {
        new c = num / divider;
        str{pos++} = c + 0x30;
        num -= c * divider;
        divider /= 10;
    }
    return countDigits + (isMinus ? 1 : 0);
}

//! Преобразовать число в строку и дополнить слева незначащими нулями
//! \param[in] num число для конвертации
//! \param[out] str массив со строкой
//! \param[in] pos позиция в строке для вставки
//! \param[in] length длина массива со строкой
//! \param[in] width ширина числа, если больше строкового представления, то будет дополнена справа нулями; если меньше, чем символов в числе, то игнорируется
//! \return кол-во вставленных в строку символов (0 - ошибка: недостаточная длина строки или неверная позиция вставки)
itoaw(num, str{}, pos, length, width)
{
    new isMinus = num < 0;
    new numDigits = digits(num);
    new numSymbols = numDigits + (isMinus ? 1 : 0);
    if (width < 0)
        width = 0;

    new totalSymbols = numSymbols >= width ? numSymbols : width;
    if (pos < 0)
        pos = 0;
    
    if ((pos + totalSymbols) > length)
        return 0;

    if (isMinus)
        str{pos++} = '-';

    for (new i = 0; i < (totalSymbols - numSymbols); i++)
        str{pos++} = '0';

    if (isMinus)
    {
        if (num == NUM_VALUE_MIN)
        {    
            insertArrayStr(str, pos, length, "2147483648", NUM_VALUE_MAX_STR_LENGTH);
            return totalSymbols;
        }
        num = -num;
    }
    if (num == 0)
    {
        str{pos} = '0';
    }
    else
    {
        new divider = 1;
        for (new i = 0; i < (numDigits - 1); i++)
            divider *= 10;

        for (new i = 0; i < numDigits; i++)
        {
            new c = num / divider;
            str{pos++} = c + 0x30;
            num -= c * divider;
            divider /= 10;
        }
    }
    return totalSymbols;
}

//! Преобразует последний полубайт числа в его ASCII-hex символ
num2asciiHexHalfByte(num)
{
    num &= 0x0F;
    return num + (num < 10 ? 0x30 : 0x37);
}

//! Преобразовывает каждый элемент подмассива в его ASCII-hex 2-символьное представление
//! \param[inout] ar преобразуемый массив
//! \param[in] arLength длина массива
//! \param[in] subLength длина подмассива
//! \param[in] start стартовый индекс подмассива
//! \return true - успешно, false - ошибка (недостаточная длина массива)
toAsciiHex(ar{}, arLength, subLength, start = 0)
{
    if (start < 0)
        start = 0;

    if ((start + 2 * subLength) > arLength)
        return false;

    for (new i = subLength - 1; i >= 0; i--)
    {
        ar{start + 2 * i + 1} = num2asciiHexHalfByte(ar{start + i});
        ar{start + 2 * i} = num2asciiHexHalfByte(ar{start + i} >> 4);
    }
    return true;
}

//! Преобразование подстроки в целое число. 
//! Если встречается нечисловой символ (после начальных "-" или "+", если они есть), преобразование прекращается
//! \param[in] str массив со строкой
//! \param[in] pos позиция начала числа
//! \param[in] length длина массива со строкой
//! \param[out] value преобразованное число
//! \return кол-во преобразованных символов подстроки, т.е. если 0 - ошибка (неверные входные данные, нет числа или неверный формат)
atoi(const str{}, pos, length, &value)
{
    if (pos < 0)
        pos = 0;

    new startPos = pos;
    pos += skipSpaces(str, length, pos);
    new isMinus = str{pos} == '-';
    if (isMinus || (str{pos} == '+'))
        pos++;

    value = 0;
    new digitFound = false;
    for (; pos < length; pos++)
    {
        if (!isDigit(str{pos}))
            break;

        if (!digitFound)
            digitFound = true;

        new newVal;
        newVal = (value * 10) + (str{pos} - '0');
        if ((((value * 10) / 10) != value) || ((newVal < 0) && ((newVal != NUM_VALUE_MIN) || !isMinus))) // переполнение
            return 0;

        value = newVal;
    }
    if (!digitFound)
        return 0;

    if (isMinus)
        value = -value;

    return pos - startPos;
}

//! Пропустить пустоты в строке (пробел, табуляция)
//! \param[in] str массив со строкой
//! \param[in] length длина массива со строкой
//! \param[in] pos позиция начала
//! \return кол-во пропущенных символов
skipSpaces(const str{}, length, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new startPos = pos;
    for (; pos < length; pos++)
    {
        if ((str{pos} != ' ') && (str{pos} != '\t'))
            break;
    }
    return pos - startPos;
}

//! Проверяет, является ли символ цифрой в формате ASCII
//! \param[in] byte предполагаемый символ цифры в ASCII
isDigit(byte)
{
    return ((byte >= '0') && (byte <= '9'));
}

//! Преобразование подстроки ASCII дробного числа в целое число, 
//! пробельные символы в начале строки пропускаются
//! \param[in] str массив со строкой
//! \param[in] pos позиция начала числа
//! \param[in] length длина массива со строкой
//! \param[in] separator символ-разделитель целой и дробной частей числа
//! \param[in] precision требуемая точность - кол-во цифр после разделителя целой и дробной частей
//! \param[out] value преобразованное число
//! \return кол-во преобразованных символов подстроки, т.е. если 0 - ошибка
atofi(const str{}, pos, length, separator, precision, &value)
{
	if (pos < 0)
        pos = 0;

    new startPos = pos;
    pos += skipSpaces(str, length, pos);
    new isMinus = str{pos} == '-';
    new added = atoi(str, pos, length, value);
    if (added == 0)
        return 0;

    pos += added;
    if (isMinus)
        value = -value;

    new i;
	if (str{pos} == separator)
	{
        pos++;
		for (; (i < precision) && (pos < length); i++, pos++)
		{
			if (!isDigit(str{pos}))
                break;

            new newVal;
            newVal = (value * 10) + (str{pos} - '0');
            if ((((value * 10) / 10) != value) || ((newVal < 0) && ((newVal != NUM_VALUE_MIN) || !isMinus))) // переполнение
                return 0;

            value = newVal;
		}
	}
	for (; i < precision; i++) // оставшуюся точность дополняем нулями
	{	
        if (((value * 10) / 10) != value) // переполнение
            return 0;

        value *= 10;
    }
	for (; pos < length; pos++) // оставшиеся цифры пропускаем
	{
        if (!isDigit(str{pos}))
            break;
    }
    if (isMinus)
        value = -value;

    return pos - startPos;
}

//! Получить hex строковое представление подмассива 
//! (например, {0, 1, 171, 255} -> "0001ABFF")
//! \param[in] ar исходный массив
//! \param[in] arStart индекс начала конвертируемой части в исходном массиве
//! \param[in] arSize длина исходного массива
//! \param[out] str строка для вставки конвертации
//! \param[in] strSize длина конвертированной строки, должно соблюдаться (strSize - strPos) >= 2 * (arSize - arStart)
//! \param[in] strPos позиция для вставки
//! \return кол-во вставленных байт
getHexString(const ar{}, arStart, arSize, str{}, strSize, strPos = 0)
{
    if (arStart < 0)
        arStart = 0;
    
    if (strPos < 0)
        strPos = 0;
    
    new i;
    for (i = 0; (i < (arSize - arStart)) && ((arStart + i) < arSize) && ((strPos + 2 * i + 1) < strSize); i++)
    {
        str{strPos + 2 * i} = num2asciiHexHalfByte(ar{arStart + i} >> 4);
        str{strPos + 2 * i + 1} = num2asciiHexHalfByte(ar{arStart + i});
    }
    return i * 2;
}

//! Посчитать длину символов числа
//! Например -1 -> 2
//! \param[in] num исходное число
//! \return кол-во символов в строковом представлении числа
numLength(num)
{
    new d = digits(num);
    return num < 0 ? d + 1 : d;
}

//! Закодировать в Base64 (RFC 4648)
//! \param[in] in массив для кодирования
//! \param[in] inSize длина массива для кодирования
//! \param[in] inPos позиция начала в массиве для кодирования
//! \param[out] out выходной массив для закодированных данных
//! \param[in] outSize длина массива для закодированных данных
//! \param[in] outPos позиция в массиве для закодированных данных
//! \return кол-во вставленных символов в выходной массив
base64Encode(const in{}, inSize, inPos, out{}, outSize, outPos = 0)
{
    if (inPos < 0)
        inPos = 0;
    
    if (outPos < 0)
        outPos = 0;
    
    new size = inSize - inPos;
    new outStrLength = base64StrLength(size);
    if ((inSize <= inPos) || (outSize <= outPos) || ((outSize - outPos) < outStrLength))
        return 0;

    for (new i = 0; i < size; i += 3, outPos += BASE64_BLOCK_SIZE)
    {
        new buf{BASE64_BLOCK_SIZE};
        new bufPos = 0;
        buf{bufPos++} = BASE64_ALPHABET{(in{inPos + i} >> 2) & 0x3F};
        if ((i + 1) < size)
        {
            buf{bufPos++} = BASE64_ALPHABET{((in{inPos + i} << 4) + (in{inPos + i + 1} >> 4)) & 0x3F};
            if ((i + 2) < size)
            {
                buf{bufPos++} = BASE64_ALPHABET{((in{inPos + i + 1} << 2) + (in{inPos + i + 2} >> 6)) & 0x3F};
                buf{bufPos++} = BASE64_ALPHABET{in{inPos + i + 2} & 0x3F};
            }
            else
            {
                buf{bufPos++} = BASE64_ALPHABET{(in{inPos + i + 1} << 2) & 0x3F};
            }
        }
        else
        {
            buf{bufPos++} = BASE64_ALPHABET{(in{inPos + i} << 4) & 0x3F};
        }
        for (; bufPos < BASE64_BLOCK_SIZE; bufPos++)
            buf{bufPos} = BASE64_PADDING;

        insertArrayStr(out, outPos, outSize, buf, BASE64_BLOCK_SIZE);
    }
    return outStrLength;
}

//! Проверка на принадлежность символа алфавиту base64
isBase64(ch)
{
    return searchLinearStr(BASE64_ALPHABET, BASE64_ALPHABET_SIZE, ch) >= 0;
}

//! Декодировать из Base64 (RFC 4648)
//! \param[in] in массив закодированных данных
//! \param[in] inSize длина массива закодированных данных
//! \param[in] inPos позиция начала в массиве закодированных данных
//! \param[out] out выходной массив для декодированных данных
//! \param[in] outSize длина массива декодированных данных
//! \param[in] outPos позиция в массиве декодированных данных
//! \return кол-во вставленных символов в выходной массив
base64Decode(const in{}, inSize, inPos, out{}, outSize, outPos = 0)
{
    if (inPos < 0)
        inPos = 0;
    
    if (outPos < 0)
        outPos = 0;
    
    new size = inSize - inPos;
    if ((inSize <= inPos) || (outSize <= outPos) || ((outSize - outPos) < fromBase64StrMinSize(size))
        || ((size % BASE64_BLOCK_SIZE) != 0))
        return 0;

    new outPosStart = outPos;
    for (new i = 0; (i + BASE64_BLOCK_SIZE) <= size; i += BASE64_BLOCK_SIZE)
    {
        new idx[1];
        for (new j = 0; j < 4; j++)
            idx{j} = searchLinearStr(BASE64_ALPHABET, BASE64_ALPHABET_SIZE, in{i + j});

        if ((idx{0} >= BASE64_ALPHABET_SIZE) || (idx{1} >= BASE64_ALPHABET_SIZE))
            return 0;

        out{outPos++} = ((idx{0} << 2) | (idx{1} >> 4));
        if (in{i + 2} == BASE64_PADDING)
        {
            if (i + BASE64_BLOCK_SIZE != size)
                return 0;

            if (in{i + 3} != BASE64_PADDING)
                return 0;
        }
        else
        {
            if (idx{2} >= BASE64_ALPHABET_SIZE)
                return 0;

            if (outPos >= outSize)
                return 0;

            out{outPos++} = ((idx{1} << 4) & 0xF0) | (idx{2} >> 2);
            if (in{i + 3} == BASE64_PADDING)
            {
                if (i + BASE64_BLOCK_SIZE != size)
                    return 0;
            }
            else
            {
                if (idx{3} >= BASE64_ALPHABET_SIZE)
                    return 0;

                if (outPos >= outSize)
                    return 0;
                
                out{outPos++} = ((idx{2} << 6) & 0xC0) | idx{3};
            }
        }
    }
    return outPos - outPosStart;
}

//! Вычислить длину закодированного массива в base64 по длине исходного
base64StrLength(inSize)
{
    return ((inSize + 2) / 3) * 4;
}

//! Вычислить максимальную длину для декодированной строки из base64 по длине кодированной
fromBase64StrMaxSize(base64Size)
{
    return (base64Size / 4) * 3;
}

//! Вычислить минимальную длину для декодированной строки из base64 по длине кодированной
fromBase64StrMinSize(base64Size)
{
    return fromBase64StrMaxSize(base64Size) - 2;
}

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
strSplitNums(const str{}, strSize, pos, separator, values[], valuesMaxSize, &valuesActualSize)
{
    if (pos < 0)
        pos = 0;

    new startPos = pos;
    valuesActualSize = 0;
    if (pos < strSize)
    {
        if (str{pos} == separator)
            pos++;
    }
    while ((pos < strSize) && (valuesActualSize < valuesMaxSize))
    {
        new val;
        new count = atoi(str, pos, strSize, val);
        if (count <= 0)
            break;

        if (valuesActualSize < valuesMaxSize)
            values[valuesActualSize++] = val;

        pos += count;
        if ((pos < strSize) && (valuesActualSize < valuesMaxSize))
        {
            if (str{pos} == separator)
                pos++;
            else
                break;
        }
    }
    return pos - startPos;
}

//! Получить значения из их ASCII-hex строкового представления
//! \return кол-во вставленных байт
asciiHexStr2array(const asciiStr{}, asciiStrSize, asciiStrPos, hex{}, hexMaxSize)
{
    if (asciiStrPos < 0)
        asciiStrPos = 0;

    new len;
    for (; ((len / 2) < hexMaxSize) && ((asciiStrPos + len) < asciiStrSize) && isAsciiHex(asciiStr{asciiStrPos + len}); len++)
    {}
    if (!len)
        return 0;

    new hexPos;
    if (len % 2)
        hex{hexPos++} = asciiHex2num(asciiStr{asciiStrPos++});
    
    new hexSize = (len / 2) + (len % 2);
    for (; hexPos < hexSize; asciiStrPos += 2, hexPos++)
        hex{hexPos} = (asciiHex2num(asciiStr{asciiStrPos}) << 4) + asciiHex2num(asciiStr{asciiStrPos + 1});

    return hexPos;
}

//! Вернуть цифру из ее кода символа
getDigit(byte)
{
    return byte - 0x30;
}

//! Преобразовать ASCII-hex символ в полубайт числа
asciiHex2num(byte)
{
    if (isDigit(byte))
        return getDigit(byte);

    return byte - (((byte >= 'A') && (byte <= 'F')) ? 'A' : 'a') + 10;
}

//! Проверить символ на принадлежность к ASCII-hex (0..9,A..F,a..f)
isAsciiHex(byte)
{
    return ((byte >= '0') && (byte <= '9')) || ((byte >= 'A') && (byte <= 'F')) || ((byte >= 'a') && (byte <= 'f'));
}

#endif // STRING_LIB
