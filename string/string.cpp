//! @file
//! @brief Функции для работы со строками

#ifdef STRING_LIB
#endinput
#endif
#define STRING_LIB

#include "string.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"

stock toLowerCase(str{}, strLength, start = 0, ignoreNull = false)
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

stock toUpperCase(str{}, strLength, start = 0, ignoreNull = false)
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

stock strLen(const str{}, strLength = 0, start = 0)
{
    if (start < 0)
        start = 0;

    new i;
    for (i = start; (strLength > 0 ? i < strLength : i >= 0) && (str{i} != 0); i++)
    {}
    return i - start;
}

stock strncpy(dest{}, destPos, destLength, const source{}, sourcePos = 0, sourceLength = 0, fromBack = false)
{
    if (destPos < 0)
        destPos = 0;

    new endPos = destPos + insertArrayStr(dest, destPos, destLength, source, sourcePos + strLen(source, sourceLength, sourcePos), sourcePos, fromBack);
    if ((endPos >= 0) && (endPos < destLength))
        dest{endPos} = 0;

    return endPos - destPos;
}

stock unread2space(str{}, strLength, start = 0, ignoreNull = false)
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

stock decfractoa(num, exp, str{}, pos, length, separator)
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

    new len = itoa((num == cellmin ? -(num + 1) : (isMinus ? -num : num)) / divider, str, pos, length);
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

stock itoa(num, str{}, pos, length)
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
    if (num == cellmin)
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

stock itoaw(num, str{}, pos, length, width)
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
        if (num == cellmin)
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

stock num2asciiHexHalfByte(num)
{
    num &= 0x0F;
    return num + (num < 10 ? 0x30 : 0x37);
}

stock toAsciiHex(ar{}, arLength, subLength, start = 0)
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

stock atoi(const str{}, pos, length, &value)
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
        if ((((value * 10) / 10) != value) || ((newVal < 0) && ((newVal != cellmin) || !isMinus))) // переполнение
            return 0;

        value = newVal;
    }
    if (!digitFound)
        return 0;

    if (isMinus)
        value = -value;

    return pos - startPos;
}

stock skipSpaces(const str{}, length, pos = 0)
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

stock isDigit(byte)
{
    return ((byte >= '0') && (byte <= '9'));
}

stock atofi(const str{}, pos, length, separator, precision, &value)
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
            if ((((value * 10) / 10) != value) || ((newVal < 0) && ((newVal != cellmin) || !isMinus))) // переполнение
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

stock getHexString(const ar{}, arStart, arSize, str{}, strSize, strPos = 0)
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

stock numLength(num)
{
    new d = digits(num);
    return num < 0 ? d + 1 : d;
}

stock base64Encode(const in{}, inSize, inPos, out{}, outSize, outPos = 0)
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

stock isBase64(ch)
{
    return searchLinearStr(BASE64_ALPHABET, BASE64_ALPHABET_SIZE, ch) >= 0;
}

stock base64Decode(const in{}, inSize, inPos, out{}, outSize, outPos = 0)
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

stock base64StrLength(inSize)
{
    return ((inSize + 2) / 3) * 4;
}

stock fromBase64StrMaxSize(base64Size)
{
    return (base64Size / 4) * 3;
}

stock fromBase64StrMinSize(base64Size)
{
    return fromBase64StrMaxSize(base64Size) - 2;
}

stock strSplitNums(const str{}, strSize, pos, separator, values[], valuesMaxSize, &valuesActualSize)
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

stock asciiHexStr2array(const asciiStr{}, asciiStrSize, asciiStrPos, hex{}, hexMaxSize)
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

stock getDigit(byte)
{
    return byte - 0x30;
}

stock asciiHex2num(byte)
{
    if (isDigit(byte))
        return getDigit(byte);

    return byte - (((byte >= 'A') && (byte <= 'F')) ? 'A' : 'a') + 10;
}

stock isAsciiHex(byte)
{
    return ((byte >= '0') && (byte <= '9')) || ((byte >= 'A') && (byte <= 'F')) || ((byte >= 'a') && (byte <= 'f'));
}

stock getStrFromGlobalVars(const dataInVarAddresses[], dataInVarAddressesSize, str{}, strMaxSize, pos = 0)
{
    return getArrayFromGlobalVars(dataInVarAddresses, dataInVarAddressesSize, str, strMaxSize, .useDataInSize = false, .dataInSizeVarAddr = 0,
                                    .dataOutPos = pos);
}

stock setStrToGlobalVars(const dataOutVarAddresses[], dataOutVarAddressesSize, const str{}, strMaxSize = 0, pos = 0)
{
    return setArrayToGlobalVars(dataOutVarAddresses, dataOutVarAddressesSize, str, (pos > 0 ? pos : 0) + strLen(str, strMaxSize, pos), .useDataOutSize = false,
                                .dataOutActualSizeVarAddr = 0, .dataInPos = pos);
}