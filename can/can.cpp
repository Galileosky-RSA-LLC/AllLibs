//! @file
//! @brief Реализация библиотеки работы с CAN

#if defined CAN_LIB
#endinput
#endif
#define CAN_LIB

#include "can.h"
#include "..\string\string.h"
#include "..\string\string.cpp"
#include "..\array\array.h"
#include "..\array\array.cpp"

stock canDiagMessage(const mes[CANMSG], portNum, bool:isSend)
{
    Diagnostics("CAN%d %s.id%d=%Xh,ds=%d:%X %Xh", portNum, isSend ? "s" : "r", mes.idType, mes.id, mes.dataSize, mes.dataSize > 0 ? mes.data[0] : 0,
                mes.dataSize > 4 ? mes.data[1] : 0);
}

stock canDiagSentMessage(const mes[CANMSG], portNum)
{
    canDiagMessage(mes, portNum, true);
}

stock canDiagReceivedMessage(const mes[CANMSG], portNum)
{
    canDiagMessage(mes, portNum, false);
}

stock bool:getCanRegime(portNum, &mode, &baudRate)
{
    if (portNum < CAN_PORT_NUM_MIN)
        portNum = CAN_PORT_NUM_MIN;
    
    if (portNum > CAN_PORT_NUM_MAX)
        portNum = CAN_PORT_NUM_MAX;

    ExecCommand(portNum == 0 ? "CANREGIME" : (portNum == 1 ? "CAN1REGIME" : "CAN2REGIME"));
    const answerMaxSize = 80;
    new answer{answerMaxSize};
    GetBinaryDataFromCommand(answer, answerMaxSize); //типа "CAN1REGIME:Mode=11,BaudRate=1000000,TimeOut=2000,DoNotCleanAfterTimeOut=1;"
    new len = strLen(answer, answerMaxSize);
    toLowerCase(answer, len);
    new const modeStr{} = "mode=";
    new idx = searchStr(answer, modeStr);
    if ((idx < 0) || !atoi(answer, mode, len, idx + strLen(modeStr)))
        return false;

    new const baudRateStr{} = "baudrate=";
    idx = searchStr(answer, baudRateStr);
    if (idx < 0)
        return false;
    
    return atoi(answer, baudRate, len, idx + strLen(baudRateStr)) > 0;
}
