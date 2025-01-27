#ifndef FILE_LIB
#define FILE_LIB
// Функции работы с файлами

#include "file.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"

//! Переименовать файл 
//! с использованием вызова команды FSMOVE
//! \param[in] src исходное имя файла, не должно содержать запятых, должно оканчиваться \0
//! \param[in] dest требуемое имя файла, не должно содержать запятых, должно оканчиваться \0
//! \return false - ошибка, true - успешно
fileRename(const src{}, const dest{})
{
    new srcSize = strLen(src);
    new destSize = strLen(dest);
    new const cmd{} = "FSMOVE";
    new cmdSize = strLen(cmd);
    new const cmdTextSep{} = " ";// !!! в библиотеку cmdhandle
    new const cmdParamSep{} = ",";// !!! в библиотеку cmdhandle
    const cellSize = 4;// !!! в библиотеку gdefines
    const bufMaxSize = 255;// !!! в библиотеку cmdhandle
    const fileNamesSizeMax = bufMaxSize - (6 + 1 + 1);
    if ((srcSize + destSize) > fileNamesSizeMax)
        return false;

    new buf{bufMaxSize + 1};
    new bufSize = insertArrayStr(buf, 0, bufMaxSize, cmd, cmdSize);
    bufSize += insertArrayStr(buf, bufSize, bufMaxSize, cmdTextSep, strLen(cmdTextSep));
    bufSize += insertArrayStr(buf, bufSize, bufMaxSize, src, srcSize);
    bufSize += insertArrayStr(buf, bufSize, bufMaxSize, cmdParamSep, strLen(cmdParamSep));
    bufSize += insertArrayStr(buf, bufSize, bufMaxSize, dest, destSize);
    ExecCommand(buf);
    bufSize = GetBinaryDataFromCommand(buf, bufMaxSize);
    toLowerCase(buf, bufSize);
    new const successResult{} = "success";
    return searchSubArBruteForceStr(buf, 0, bufSize, successResult, strLen(successResult)) >= 0;
}

//! Записать в файл буфер с произвольной позиции
//! \param[in] fileName имя файла, должно оканчиваться \0
//! \param[in] buf записываемый буфер
//! \param[in] bufSize размер буфера
//! \param[in] fileOffset смещение в файле (-1 - запись в конец)
//! \param[in] bufPos начало записываемой части буфера
//! \return количество записанных байт
fileWriteWrap(const fileName{}, const buf{}, bufSize, fileOffset, bufPos = 0)
{
    if ((bufSize <= 0) || (bufPos >= bufSize))
        return 0;

    if (bufPos <= 0)
        return FileWrite(fileName, buf, bufSize, fileOffset);

    const cellSize = 4;
    new rest = bufPos % cellSize;
    new bufCell = (bufPos / cellSize) + (rest != 0);
    new subSize = rest ? cellSize - rest : 0;
    if (subSize)
    {
        new sub{cellSize};
        insertArrayStr(sub, 0, subSize, buf, bufSize, bufPos);
        new written = FileWrite(fileName, sub, subSize, fileOffset);
        if (written != subSize)
            return written;
    }
    return subSize + FileWrite(fileName, buf[bufCell], bufSize - (bufCell * cellSize), fileOffset < 0 ? fileOffset : fileOffset + subSize);
}

#endif // FILE_LIB