//! @file
//! @brief Функции для работы с файлами

#ifdef FILE_LIB
#endinput
#endif
#define FILE_LIB

#include "file.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"
#include "..\numeric\numeric.h"
#include "..\cmdhandle\cmdhandle.h"

stock fileRename(const src{}, const dest{})
{
    new srcLength = strLen(src);
    new destLength = strLen(dest);
    new const cmdText{} = "FSMOVE";
    new cmdTextLength = strLen(cmdText);
    new fileNamesLengthMax = CMD_LENGTH_MAX - (cmdTextLength + strLen(CMD_TEXT_SEPARATOR) + strLen(CMD_PARAM_SEPARATOR));
    if ((srcLength + destLength) > fileNamesLengthMax)
        return false;

    new buf{CMD_LENGTH_MAX_W0};
    new bufSize = insertArrayStr(buf, 0, CMD_LENGTH_MAX, cmdText, cmdTextLength);
    bufSize += insertArrayStr(buf, bufSize, CMD_LENGTH_MAX, CMD_TEXT_SEPARATOR, strLen(CMD_TEXT_SEPARATOR));
    bufSize += insertArrayStr(buf, bufSize, CMD_LENGTH_MAX, src, srcLength);
    bufSize += insertArrayStr(buf, bufSize, CMD_LENGTH_MAX, CMD_PARAM_SEPARATOR, strLen(CMD_PARAM_SEPARATOR));
    bufSize += insertArrayStr(buf, bufSize, CMD_LENGTH_MAX, dest, destLength);
    ExecCommand(buf);
    bufSize = GetBinaryDataFromCommand(buf, CMD_LENGTH_MAX);
    toLowerCase(buf, bufSize);
    new const successResult{} = "success";
    return searchSubArBruteForceStr(buf, 0, bufSize, successResult, strLen(successResult)) >= 0;
}

stock fileWriteWrap(const fileName{}, const buf{}, bufSize, fileOffset, bufPos = 0)
{
    if ((bufSize <= 0) || (bufPos >= bufSize))
        return 0;

    if (bufPos <= 0)
        return FileWrite(fileName, buf, bufSize, fileOffset);

    new rest = bufPos % CELL_BYTES;
    new bufCell = (bufPos / CELL_BYTES) + (rest != 0);
    new subSize = rest ? CELL_BYTES - rest : 0;
    if (subSize)
    {
        new sub{CELL_BYTES};
        insertArrayStr(sub, 0, subSize, buf, bufSize, bufPos);
        new written = FileWrite(fileName, sub, subSize, fileOffset);
        if (written != subSize)
            return written;
    }
    return subSize + FileWrite(fileName, buf[bufCell], bufSize - (bufCell * CELL_BYTES), fileOffset < 0 ? fileOffset : fileOffset + subSize);
}
