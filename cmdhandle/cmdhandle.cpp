//! @file
//! @brief Функции библиотеки обработчика команд

#ifdef CMDHANDLE_LIB
#endinput
#endif
#define CMDHANDLE_LIB

#include "cmdhandle.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"

stock cmdHandlerAddTextToAnswer(obj[CMDHANDLE_DATA], const text{}, textSize = 0)
{
    obj.answerSize += insertArrayStr(obj.answer, obj.answerSize, CMDHANDLE_ANSWER_SIZE_MAX, text, textSize > 0 ? textSize : strLen(text));
}

stock cmdHandlerAddNumToAnswer(obj[CMDHANDLE_DATA], num)
{
    obj.answerSize += itoa(num, obj.answer, obj.answerSize, CMDHANDLE_ANSWER_SIZE_MAX);
}

stock cmdHandlerAddParValueToAnswer(obj[CMDHANDLE_DATA], const parName{}, parValue, parNameSize = 0)
{
    cmdHandlerAddTextToAnswer(obj, parName, parNameSize);
    cmdHandlerAddNumToAnswer(obj, parValue);
}

stock cmdHandlerAddHexAsciiToAnswer(obj[CMDHANDLE_DATA], const data{}, dataSize)
{
    new size = obj.answerSize + (CMDHANDLE_ANSWER_SIZE_MAX - obj.answerSize) / 2;
    new len = insertArrayStr(obj.answer, obj.answerSize, size, data, dataSize);
    toAsciiHex(obj.answer, CMDHANDLE_ANSWER_SIZE_MAX, len, obj.answerSize);
    obj.answerSize += len * 2;
}

stock cmdHandlerSendPreparedAnswer(obj[CMDHANDLE_DATA])
{
    if ((obj.answerSize >= 0) && (obj.answerSize < CMDHANDLE_ANSWER_SIZE_MAX_W0))
        obj.answer{obj.answerSize++} = 0;
        
    new conId = GetVar(CONNECTION_ID);
    if (conId) // багофича
        swapBuf(obj.answer, obj.answerSize);

    SendAnswer(conId, GetVar(COMMAND_NUMBER), obj.answer, obj.answerSize, "", 0);
}
