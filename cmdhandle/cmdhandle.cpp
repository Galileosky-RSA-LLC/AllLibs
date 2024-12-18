// Библиотека обработчика ответов на команды

#ifndef CMDHANDLE_LIB
#define CMDHANDLE_LIB

#include "cmdhandle.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"

//! Добавить текст к ответу
cmdHandlerAddTextToAnswer(obj[CMDHANDLE_DATA], const text{}, textSize = 0)
{
    obj.answerSize += insertArrayStr(obj.answer, obj.answerSize, CMDHANDLE_ANSWER_SIZE_MAX, text, textSize > 0 ? textSize : strLen(text));
}

//! Добавить текстовое представление числа к ответу
cmdHandlerAddNumToAnswer(obj[CMDHANDLE_DATA], num)
{
    obj.answerSize += itoa(num, obj.answer, obj.answerSize, CMDHANDLE_ANSWER_SIZE_MAX);
}

//! Добавить к ответу имя параметра и его числовое значение
cmdHandlerAddParValueToAnswer(obj[CMDHANDLE_DATA], const parName{}, parValue, parNameSize = 0)
{
    cmdHandlerAddTextToAnswer(obj, parName, parNameSize);
    cmdHandlerAddNumToAnswer(obj, parValue);
}

//! Добавить к ответу hex данные
cmdHandlerAddHexAsciiToAnswer(obj[CMDHANDLE_DATA], const data{}, dataSize)
{
    new size = obj.answerSize + (CMDHANDLE_ANSWER_SIZE_MAX - obj.answerSize) / 2;
    new len = insertArrayStr(obj.answer, obj.answerSize, size, data, dataSize);
    toAsciiHex(obj.answer, CMDHANDLE_ANSWER_SIZE_MAX, len, obj.answerSize);
    obj.answerSize += len * 2;
}

//! Послать подготовленный ранее ответ на команду
//! \param[inout] obj объект обработчика, ответ может быть изменен
cmdHandlerSendPreparedAnswer(obj[CMDHANDLE_DATA])
{
    if ((obj.answerSize >= 0) && (obj.answerSize < CMDHANDLE_ANSWER_SIZE_MAX_W0))
        obj.answer{obj.answerSize++} = 0;
        
    new conId = GetVar(CONNECTION_ID);
    if (conId) // багофича
        swapBuf(obj.answer, obj.answerSize);

    SendAnswer(conId, GetVar(COMMAND_NUMBER), obj.answer, obj.answerSize, "", 0);
}

#endif