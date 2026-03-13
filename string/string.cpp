//! @file
//! @brief Реализация библиотеки для работы со строками

#if defined STRING_LIB
#endinput
#endif
#define STRING_LIB

#include "string.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"

#define SYMBOL_LATIN_LETTER_CASE_OFFSET 0x20

stock toLowerCase(str{}, strLength = 0, start = 0)
{
    if (start < 0)
        start = 0;

    for (new i = start; ((strLength > 0) ? (i < strLength) : true) && (str{i} != SYMBOL_NUL); i++)
    {    
        if ((str{i} >= SYMBOL_LATIN_CAPITAL_LETTER_A) && (str{i} <= SYMBOL_LATIN_CAPITAL_LETTER_Z)
            || (str{i} >= SYMBOL_CYRILLIC_CAPITAL_LETTER_A) && (str{i} <= SYMBOL_CYRILLIC_CAPITAL_LETTER_YA))
            str{i} += SYMBOL_LATIN_LETTER_CASE_OFFSET;
        else if (str{i} == SYMBOL_CYRILLIC_CAPITAL_LETTER_IO)
            str{i} = SYMBOL_CYRILLIC_SMALL_LETTER_IO;
    }
}

stock toUpperCase(str{}, strLength = 0, start = 0)
{
    if (start < 0)
        start = 0;

    for (new i = start; ((strLength > 0) ? (i < strLength) : true) && (str{i} != SYMBOL_NUL); i++)
    {    
        if ((str{i} >= SYMBOL_LATIN_SMALL_LETTER_A) && (str{i} <= SYMBOL_LATIN_SMALL_LETTER_Z)
            || (str{i} >= SYMBOL_CYRILLIC_SMALL_LETTER_A) && (str{i} <= SYMBOL_CYRILLIC_SMALL_LETTER_YA))
            str{i} -= SYMBOL_LATIN_LETTER_CASE_OFFSET;
        else if (str{i} == SYMBOL_CYRILLIC_SMALL_LETTER_IO)
            str{i} = SYMBOL_CYRILLIC_CAPITAL_LETTER_IO;
    }
}

stock strLen(const str{}, strLength = 0, start = 0)
{
    if (start < 0)
        start = 0;

    new i;
    for (i = start; (strLength > 0 ? i < strLength : i >= 0) && (str{i} != SYMBOL_NUL); i++)
    {}
    return i - start;
}

stock strncpy(dest{}, destMaxSize, const source{}, destPos = 0, sourcePos = 0, sourceLength = 0, bool:fromBack = false)
{
    if (destPos < 0)
        destPos = 0;

    new endPos = destPos + insertArrayStr(dest, destPos, destMaxSize, source, sourcePos + strLen(source, sourceLength, sourcePos), sourcePos, fromBack);
    if ((endPos >= 0) && (endPos < destMaxSize))
        dest{endPos} = SYMBOL_NUL;

    return endPos - destPos;
}

stock unread2space(str{}, strLength = 0, start = 0, bool:ignoreNull = false)
{
    if (start < 0)
        start = 0;

    new count;
    if (strLength <= 0)
        strLength = start + strLen(str, strLength, start);

    for (new i = start; (i < strLength) && ((str{i} != SYMBOL_NUL) || ignoreNull); i++)
    {
        if (((str{i} >= SYMBOL_NUL) && (str{i} <= SYMBOL_US)) || (str{i} == SYMBOL_DEL))
        {    
            str{i} = SYMBOL_SPACE;
            count++;
        }
    }
    return count;
}

stock decfractoa(num, exp, str{}, strMaxSize, separator, pos = 0)
{
    if (pos < 0)
        pos = 0;
    
    if (exp <= 0)
        return itoa(num, str, strMaxSize, pos);

    const fractionalNumberWidthMin = 3;
    if ((exp > 9) || ((pos + fractionalNumberWidthMin) > strMaxSize))
        return 0;

    new const startPos = pos;
    new const divider = pow(10, exp);
    assert divider != 0;
    new const bool:isNegative = num < 0;
    if (isNegative)
        str{pos++} = SYMBOL_MINUS;

    new const numeratorLengh = itoa((num == cellmin ? -(num + 1) : (isNegative ? -num : num)) / divider, str, strMaxSize, pos);
    if (numeratorLengh <= 0)
        return 0;

    pos += numeratorLengh;
    if ((exp + pos) > strMaxSize)
        return 0;

    str{pos++} = separator;
    num %= divider;
    if (isNegative && (num != 0))
        num = divider - num;

    new const zeros = exp - digits(num);
    for (new i = 0; (pos < strMaxSize) && (i < zeros); i++, pos++)
        str{pos} = SYMBOL_DIGIT_0;

    new const restLength = itoa(num, str, strMaxSize, pos);
    if (restLength <= 0)
        return 0;

    new nulPos = pos + restLength;
    if (nulPos < strMaxSize)
        str{nulPos} = SYMBOL_NUL;

    return nulPos - startPos;
}

stock itoa(num, str{}, strMaxSize, pos = 0)
{
    return itoaw(num, str, strMaxSize, 0, pos);
}

stock itoaw(num, str{}, strMaxSize, width = 0, pos = 0)
{
    new const bool:isNegative = num < 0;
    new const digitsQty = digits(num);
    new const numSymbols = digitsQty + (isNegative ? 1 : 0);
    if (width < 0)
        width = 0;

    new const totalSymbols = numSymbols > width ? numSymbols : width;
    if (pos < 0)
        pos = 0;
    
    if ((pos + totalSymbols) > strMaxSize)
        return 0;

    if (isNegative)
        str{pos++} = SYMBOL_MINUS;

    for (new i = 0; i < (totalSymbols - numSymbols); i++)
        str{pos++} = SYMBOL_DIGIT_0;

    if (isNegative)
    {
        if (num == cellmin)
        {    
            insertArrayStr(str, pos, strMaxSize, "2147483648", NUM_VALUE_MAX_STR_LENGTH);
            return totalSymbols;
        }
        num = -num;
    }
    if (num == 0)
    {
        str{pos++} = SYMBOL_DIGIT_0;
    }
    else
    {
        new divider = 1;
        for (new i = 0; i < (digitsQty - 1); i++)
            divider *= 10;

        for (new i = 0; i < digitsQty; i++)
        {
            new const digit = num / divider;
            str{pos++} = digit + SYMBOL_DIGIT_0;
            num -= digit * divider;
            divider /= 10;
        }
    }
    if (pos < strMaxSize)
        str{pos} = 0;

    return totalSymbols;
}

stock num2asciiHexHalfByte(num)
{
    num &= 0x0F;
    return num + (num < 10 ? SYMBOL_DIGIT_0 : SYMBOL_DIGIT_7);
}

stock bool:toAsciiHex(ar{}, arSize, subSize, start = 0)
{
    if (start < 0)
        start = 0;

    if (subSize < 0)
        subSize = 0;

    new const totalLength = start + (2 * subSize);
    if (totalLength > arSize)
        return false;

    for (new i = subSize - 1; i >= 0; i--)
    {
        new const byte = ar{start + i};
        ar{start + (2 * i) + 1} = num2asciiHexHalfByte(byte);
        ar{start + (2 * i)} = num2asciiHexHalfByte(byte >> 4);
    }
    if (totalLength < arSize)
        ar{totalLength} = 0;

    return true;
}

stock atoi(const str{}, &value, strLength = 0, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new const startPos = pos;
    if (strLength <= 0)
        strLength = pos + strLen(str, strLength, pos);

    pos += skipSpaces(str, strLength, pos);
    if ((pos < startPos) || (pos >= strLength))
        return 0;

    new const bool:isNegative = str{pos} == SYMBOL_MINUS;
    if (isNegative || (str{pos} == SYMBOL_PLUS))
        pos++;

    value = 0;
    new bool:digitFound = false;
    for (; (pos < strLength) && isDigit(str{pos}); pos++)
    {
        if (!digitFound)
            digitFound = true;

        new const newVal = (value * 10) + (str{pos} - SYMBOL_DIGIT_0);
        new const bool:isOverflow = (((value * 10) / 10) != value) || ((newVal < 0) && ((newVal != cellmin) || !isNegative));
        if (isOverflow)
            return 0;

        value = newVal;
    }
    if (!digitFound)
        return 0;

    if (isNegative)
        value = -value;

    return pos - startPos;
}

stock skipSpaces(const str{}, strLength = 0, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new const startPos = pos;
    for (; ((strLength > 0) ? (pos < strLength) : (str{pos} != SYMBOL_NUL)) && ((str{pos} == SYMBOL_SPACE) || (str{pos} == SYMBOL_HT)); pos++)
    {}
    return pos - startPos;
}

stock bool:isDigit(byte)
{
    return ((byte >= SYMBOL_DIGIT_0) && (byte <= SYMBOL_DIGIT_9));
}

stock atofi(const str{}, separator, precision, &value, strLength = 0, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new const startPos = pos;
    if (strLength <= 0)
        strLength = pos + strLen(str, strLength, pos);

    pos += skipSpaces(str, strLength, pos);
    if ((pos < startPos) || (pos >= strLength))
        return 0;

    new const bool:isNegative = str{pos} == SYMBOL_MINUS;
    new const added = atoi(str, value, strLength, pos);
    if (added == 0)
        return 0;

    pos += added;
    if (isNegative)
        value = -value;

    new i;
    if (str{pos} == separator)
    {
        pos++;
        for (; (i < precision) && (pos < strLength) && isDigit(str{pos}); i++, pos++)
        {
            new const newVal = (value * 10) + (str{pos} - SYMBOL_DIGIT_0);
            new const bool:isOverflow = (((value * 10) / 10) != value) || ((newVal < 0) && ((newVal != cellmin) || !isNegative));
            if (isOverflow)
                return 0;

            value = newVal;
        }
    }
    for (; i < precision; i++) // оставшуюся точность дополняем нулями
    {    
        new const bool:isOverflow = ((value * 10) / 10) != value;
        if (isOverflow)
            return 0;

        value *= 10;
    }
    for (; (pos < strLength) && isDigit(str{pos}); pos++) // оставшиеся цифры пропускаем
    {}
    if (isNegative)
        value = -value;

    return pos - startPos;
}

stock getHexString(const ar{}, arSize, str{}, strMaxSize, strPos = 0, arStart = 0)
{
    if (arStart < 0)
        arStart = 0;
    
    if (strPos < 0)
        strPos = 0;
    
    new i;
    for (i = 0; (i < (arSize - arStart)) && ((arStart + i) < arSize) && ((strPos + (2 * i) + 1) < strMaxSize); i++)
    {
        str{strPos + (2 * i)} = num2asciiHexHalfByte(ar{arStart + i} >> 4);
        str{strPos + (2 * i) + 1} = num2asciiHexHalfByte(ar{arStart + i});
    }
    new insertedLength = i * 2;
    new strEndPos = strPos + insertedLength;
    if (strEndPos < strMaxSize)
        str{strEndPos} = 0;

    return insertedLength;
}

stock numLength(num)
{
    return digits(num) + (num < 0 ? 1 : 0);
}

stock base64encode(const in{}, inSize, out{}, outMaxSize, outPos = 0, inPos = 0)
{
    if (inPos < 0)
        inPos = 0;
    
    if (outPos < 0)
        outPos = 0;
    
    new const size = inSize - inPos;
    new const outSubStrLength = base64strLength(size);
    if ((inSize <= inPos) || (outMaxSize <= outPos) || ((outMaxSize - outPos) < outSubStrLength))
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

        insertArrayStr(out, outPos, outMaxSize, buf, BASE64_BLOCK_SIZE);
    }
    if (outPos < outMaxSize)
        out{outPos} = 0;

    return outSubStrLength;
}

stock bool:isBase64(byte)
{
    return searchLinearStr(BASE64_ALPHABET, BASE64_ALPHABET_SIZE, byte) >= 0;
}

stock base64decode(const in{}, out{}, outMaxSize, outPos = 0, inSize = 0, inPos = 0)
{
    if (inPos < 0)
        inPos = 0;
    
    if (outPos < 0)
        outPos = 0;
    
    if (inSize <= 0)
        inSize = inPos + strLen(in, inSize, inPos);

    new const size = inSize - inPos;
    if ((inSize <= inPos) || (outMaxSize <= outPos) || ((outMaxSize - outPos) < fromBase64minSize(size))
        || ((size % BASE64_BLOCK_SIZE) != 0))
        return 0;

    new const outPosStart = outPos;
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

            if (outPos >= outMaxSize)
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

                if (outPos >= outMaxSize)
                    return 0;
                
                out{outPos++} = ((idx{2} << 6) & 0xC0) | idx{3};
            }
        }
    }
    return outPos - outPosStart;
}

stock base64strLength(inSize)
{
    return ((inSize + 2) / 3) * 4;
}

stock fromBase64maxSize(base64length)
{
    return (base64length / 4) * 3;
}

stock fromBase64minSize(base64length)
{
    return fromBase64maxSize(base64length) - 2;
}

stock strSplitNums(const str{}, separator, values[], valuesMaxSize, &valuesActualSize, strLength = 0, pos = 0)
{
    if (pos < 0)
        pos = 0;

    new const startPos = pos;
    valuesActualSize = 0;
    if (strLength <= 0)
        strLength = pos + strLen(str, strLength, pos);

    while ((pos < strLength) && (valuesActualSize < valuesMaxSize))
    {
        new const nextPos = pos + 1;
        if (str{pos} == separator)
        {    
            if ((nextPos < strLength) && (str{nextPos} == separator))
                break;
        }
        else if (pos != startPos)
        {
            break;
        }
        new val;
        new const count = atoi(str, val, strLength, nextPos);
        if (count <= 0)
            break;

        values[valuesActualSize++] = val;
        pos = nextPos + count;
    }
    return pos - startPos;
}

stock asciiHexStr2array(const asciiStr{}, hex{}, hexMaxSize, asciiStrLength = 0, asciiStrPos = 0)
{
    if (asciiStrPos < 0)
        asciiStrPos = 0;

    if (asciiStrLength <= 0)
        asciiStrLength = asciiStrPos + strLen(asciiStr, asciiStrLength, asciiStrPos);

    new len;
    for (; ((len / 2) < hexMaxSize) && ((asciiStrPos + len) < asciiStrLength) && isAsciiHex(asciiStr{asciiStrPos + len}); len++)
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

stock getDigit(byte)
{
    return byte - SYMBOL_DIGIT_0;
}

stock asciiHex2num(byte)
{
    if (isDigit(byte))
        return getDigit(byte);

    return byte - (((byte >= SYMBOL_LATIN_CAPITAL_LETTER_A) && (byte <= SYMBOL_LATIN_CAPITAL_LETTER_F))
                    ? SYMBOL_LATIN_CAPITAL_LETTER_A
                    : SYMBOL_LATIN_SMALL_LETTER_A) + 10;
}

stock bool:isAsciiHex(byte)
{
    return ((byte >= SYMBOL_DIGIT_0) && (byte <= SYMBOL_DIGIT_9))
            || ((byte >= SYMBOL_LATIN_CAPITAL_LETTER_A) && (byte <= SYMBOL_LATIN_CAPITAL_LETTER_F))
            || ((byte >= SYMBOL_LATIN_SMALL_LETTER_A) && (byte <= SYMBOL_LATIN_SMALL_LETTER_F));
}

stock getStrFromGlobalVars(const dataInVarAddresses[], dataInVarAddressesSize, str{}, strMaxSize, pos = 0)
{
    return getArrayFromGlobalVars(dataInVarAddresses, dataInVarAddressesSize, str, strMaxSize, .useDataInSize = false, .dataInSizeVarAddr = 0,
                                    .dataOutPos = pos);
}

stock setStrToGlobalVars(const dataOutVarAddresses[], dataOutVarAddressesSize, const str{}, strLength = 0, pos = 0)
{
    return setArrayToGlobalVars(dataOutVarAddresses, dataOutVarAddressesSize, str, (pos > 0 ? pos : 0) + strLen(str, strLength, pos), .useDataOutSize = false,
                                .dataOutActualSizeVarAddr = 0, .dataInPos = pos);
}

stock searchStr(const str{}, const sub{}, strLength = 0, strStart = 0, subLength = 0, subStart = 0)
{
    return searchSubArBruteForceStr(str, strStart, (strStart > 0 ? strStart : 0) + strLen(str, strLength, strStart), sub,
                                    (subStart > 0 ? subStart : 0) + strLen(sub, subLength, subStart), subStart);
}
