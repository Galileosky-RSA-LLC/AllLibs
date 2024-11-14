#ifndef FILE_LIB
#define FILE_LIB
// Функции работы с файлами

#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"

//! Переименовать файл 
//! с использованием вызова команды FSMOVE
//! \param[in] src исходное имя файла, должно оканчиваться \0, длина не более 123
//! \param[in] dest требуемое имя файла, должно оканчиваться \0, длина не более 123
//! \return false - ошибка, true - успешно
fileRename(const src{}, const dest{})
{
    const bufLength = 255;
    const cmdLength = 7;
    const fileNameLengthMax = (bufLength - cmdLength - 1) / 2;
    new buf{bufLength} = "FSMOVE "; // команда и ответ
    new pos = cmdLength;
    pos += insertArrayStr(buf, pos, bufLength, src, strLen(src, fileNameLengthMax));
    buf{pos++} = ',';
    pos += insertArrayStr(buf, pos, bufLength, dest, strLen(dest, fileNameLengthMax));
    ExecCommand(buf);
    clearArrayStr(buf, bufLength);
    GetBinaryDataFromCommand(buf, bufLength);
    toLowerCase(buf, bufLength);
    pos = -1;
    pos = searchSubArBruteForceStr(buf, 0, bufLength, "success", 7);
    return pos >= 0;
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