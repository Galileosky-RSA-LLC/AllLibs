#ifndef DIAG_LIB
#define DIAG_LIB
// Библиотека расширенного вывода в диагностику

#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"


//! Вывести в диагностику текстовые представления битов числа, разделенные символом ";". 
//! Максимальная длина строки диагностики 95 символов
//! \param[in] number число для представления
//! \param[in] bytes разрядность числа в байтах, 1..4
//! \param[in] prefix добавляемый префикс строки диагностики, должен оканчиваться \0
//! \param[in] texts тексты на каждый бит числа, должны оканчиваться \0. Текст добавляется в диагностику, если соотв. его индексу бит == "ИСТИНА"
num2bitDiag(number, bytes, const prefix{}, const texts[][])
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

//! Вывод в диагностику подмассива в hex и его строковой интерпретации с подписью и разделением на читабельные части
//! \param[in] ar массив для вывода
//! \param[in] arLength размер массива
//! \param[in] arName выводимая подпись, должна оканчиваться \0
//! \param[in] needStr признак необходимости вывода строкового представления подмассива
//! \param[in] pos индекс начала вывода
diagAr(const ar{}, arLength, const arName{}, needStr, pos = 0)
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

#endif // DIAG_LIB