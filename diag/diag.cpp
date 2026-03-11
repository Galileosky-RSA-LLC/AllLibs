//! @file
//! @brief Реализация библиотеки расширенного вывода в диагностику

#if defined DIAG_LIB
#endinput
#endif
#define DIAG_LIB

#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"

stock num2bitDiag(number, bytes, const prefix{}, const texts[][])
{
    if (bytes < 1)
        bytes = 1;

    if (bytes > 4)
        bytes = 4;

    const strLength = 95; // ограничение строки диагностики
    new str{strLength + 1}; // строка диагностики
    new length; // длина строки диагностики
    new tLen; // длина текущей вставляемой строки
    tLen = strLen(prefix, strLength);
    length = insertArrayStr(str, 0, strLength, prefix, tLen);
    new prefixLength = length;
    for (new i = 0; (i < 8 * bytes) && (length < strLength); i++)
    {
        if (number & (1 << i))
        {    
            if (length > prefixLength)
            {    
                str{length} = ';';
                length++;
            }
            tLen = strLen(texts[i], strLength - length);
            length = length + insertArrayStr(str, length, strLength, texts[i], tLen);
        }
    }
    Diagnostics(str);
}

stock diagAr(const ar{}, arLength, const arName{}, needStr, pos = 0)
{
	const diagBlockSize = 48;
    new ddBuf{diagBlockSize + 1};
    if (pos < 0)
        pos = 0;

    Diagnostics("%s =%d bts", arName, arLength - pos);
    for (new i = 0; pos < arLength; pos += diagBlockSize, i++)
    {
        new count = insertArrayStr(ddBuf, 0, diagBlockSize, ar, arLength, pos);
        if (i > 0)
            Diagnostics("%s bts %d-%d:", arName, pos, pos + count - 1);
        else
            Diagnostics("bts %d-%d:", pos, pos + count - 1);

        DiagnosticsHex(ddBuf, count);
        if (needStr)
        {    
            ddBuf{count} = 0;
            unread2space(ddBuf, count, 0, true);
            Diagnostics(ddBuf);
        }
    }
}
