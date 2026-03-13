//! @file
//! @brief Функции библиотеки обработчика команд

#if defined CMDHANDLE_LIB
#endinput
#endif
#define CMDHANDLE_LIB

#include "cmdhandle.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"

stock bool:cmdHandlerAddTextToAnswer(obj[CMDHANDLE_DATA], const text{})
{
    if (strLen(text) > (CMDHANDLE_ANSWER_SIZE_MAX - obj.answerSize))
        return false;

    obj.answerSize += strncpy(obj.answer, CMDHANDLE_ANSWER_SIZE_MAX, text);
    return true;
}

stock bool:cmdHandlerAddNumToAnswer(obj[CMDHANDLE_DATA], num)
{
    new len = itoa(num, obj.answer, CMDHANDLE_ANSWER_SIZE_MAX, obj.answerSize);
    obj.answerSize += len;
    return len > 0;
}

stock bool:cmdHandlerAddParValueToAnswer(obj[CMDHANDLE_DATA], const parName{}, parValue)
{
    new oldSize = obj.answerSize;
    if (cmdHandlerAddTextToAnswer(obj, parName) && cmdHandlerAddNumToAnswer(obj, parValue))
        return true;

    obj.answerSize = oldSize;
    return false;
}

stock bool:cmdHandlerAddHexAsciiToAnswer(obj[CMDHANDLE_DATA], const data{}, dataSize)
{
    if (dataSize < 0)
        dataSize = 0;

    if ((dataSize * 2) > (CMDHANDLE_ANSWER_SIZE_MAX - obj.answerSize))
        return false;

    new size = obj.answerSize + (CMDHANDLE_ANSWER_SIZE_MAX - obj.answerSize) / 2;
    new len = insertArrayStr(obj.answer, obj.answerSize, size, data, dataSize);
    toAsciiHex(obj.answer, CMDHANDLE_ANSWER_SIZE_MAX, len, obj.answerSize);
    obj.answerSize += len * 2;
    return true;
}

stock cmdHandlerSendPreparedAnswer(obj[CMDHANDLE_DATA], const binaryData{} = "", binaryDataSize = 0, bool:useThreadConnection = true,
                                    connectionType = CMDHANDLE_CONNECTION_TYPE_UNKNOWN, connectionId = 0, commandNumber = 0)
{
    if (useThreadConnection)
    {
        connectionType = GetVar(CONNECTION_TYPE);
        connectionId = GetVar(CONNECTION_ID);
        commandNumber = GetVar(COMMAND_NUMBER);
    }
    new bool:addNul = (obj.answerSize >= 0) && (obj.answerSize < CMDHANDLE_ANSWER_SIZE_MAX_W0);
    if (addNul)
        obj.answer{obj.answerSize++} = 0;
        
    new bool:isServerConnection = connectionType == CMDHANDLE_CONNECTION_TYPE_SERVER;
    if (isServerConnection)
        swapBuf(obj.answer, obj.answerSize);

    SendAnswer(connectionId, commandNumber, obj.answer, obj.answerSize, binaryData, binaryDataSize);
    if (isServerConnection)
        swapBuf(obj.answer, obj.answerSize);

    if (addNul)
        obj.answerSize--;
}
