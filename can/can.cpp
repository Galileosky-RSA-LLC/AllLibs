#if !defined CAN_LIB
#define CAN_LIB
// Библиотека работы с CAN

#include "can.h"
#include "..\string\string.h"
#include "..\string\string.cpp"
#include "..\array\array.h"
#include "..\array\array.cpp"

//! Вывод в диагностику сообщения CAN
//! \param[in] mes выводимое сообщение
//! \param[in] portNum номер порта
//! \param[in] send признак отправки сообщения: !0 - отправка, 0 - прием
canDiagMessage(const mes[CANMSG], portNum, send)
{
    Diagnostics("CAN%d %s.id=0x%X,ds=%d:0x%X %X", portNum, send ? "s" : "r", mes.id, mes.dataSize, mes.data[0], mes.data[1]);
}

//! Вывод в диагностику отправляемого сообщения CAN
//! \param[in] mes выводимое сообщение
//! \param[in] portNum номер порта
canDiagSentMessage(const mes[CANMSG], portNum)
{
    canDiagMessage(mes, portNum, true);
}

//! Вывод в диагностику принятого сообщения CAN
//! \param[in] mes выводимое сообщение
//! \param[in] portNum номер порта
canDiagReceivedMessage(const mes[CANMSG], portNum)
{
    canDiagMessage(mes, portNum, false);
}

//! Получить установленные параметры для CAN порта
//! \param[in] portIndex индекс порта
//! \param[out] mode установленный режим CAN_MODE_
//! \param[out] baudRate установленная скорость, бит/с
//! \return true - успешно, false - ошибка
getCanRegime(portIndex, &mode, &baudRate)
{
    if (portIndex < CAN_PORT_NUM_MIN)
        portIndex = CAN_PORT_NUM_MIN;
    
    if (portIndex > CAN_PORT_NUM_MAX)
        portIndex = CAN_PORT_NUM_MAX;

    ExecCommand(portIndex == 0 ? "CANREGIME" : (portIndex == 1 ? "CAN1REGIME" : "CAN2REGIME"));
    const answerMaxSize = 80;
    new answer{answerMaxSize};
    GetBinaryDataFromCommand(answer, answerMaxSize); //типа "CAN1REGIME:Mode=11,BaudRate=1000000,TimeOut=2000,DoNotCleanAfterTimeOut=1;"
    new len = strLen(answer, answerMaxSize);
    toLowerCase(answer, len);
    const modeStrSize = 5;
    new const modeStr{} = "mode=";
    new idx = searchStr(answer, modeStr);
    if ((idx < 0) || !atoi(answer, mode, len, idx + modeStrSize))
        return false;

    const baudRateStrSize = 9;
    new const baudRateStr{} = "baudrate=";
    idx = searchStr(answer, baudRateStr);
    if (idx < 0)
        return false;
    
    return atoi(answer, baudRate, len, idx + baudRateStrSize);
}

#endif