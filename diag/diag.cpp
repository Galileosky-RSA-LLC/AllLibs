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
#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"

stock num2bitDiag(number, const texts[][], textsCount, const prefix{} = "")
{
    const strMaxLength = 95;
    const strMaxLengthW0 = strMaxLength + 1;
    new str{strMaxLengthW0};
    new strLength = strncpy(str, strMaxLength, prefix);
    new const prefixLength = strLength;
    for (new bit = 0; (bit < cellbits) && (bit < textsCount) && (strLength < strMaxLength); bit++)
    {
        if (!getBit(number, bit))
            continue;

        if (strLength > prefixLength)
            strLength += strncpy(str, strMaxLength, ";", strLength);

        strLength += strncpy(str, strMaxLength, texts[bit], strLength);
    }
    Diagnostics(str);
}

stock diagAr(const ar{}, arSize, const arName{}, bool:needStr = false, pos = 0)
{
    if (pos < 0)
        pos = 0;

    Diagnostics("%s =%d bytes", arName, arSize - pos);
    while (pos < arSize)
    {
        const diagBlockSize = 48;
        new diagBuf{diagBlockSize + 1};
        new const count = insertArrayStr(diagBuf, 0, diagBlockSize, ar, arSize, pos);
        Diagnostics("%s bytes %d..%d:", arName, pos, pos + count - 1);
        DiagnosticsHex(diagBuf, count);
        if (needStr)
        {    
            diagBuf{count} = 0;
            unread2space(diagBuf, count, 0, true);
            Diagnostics(diagBuf);
        }
        pos += diagBlockSize;
    }
}
