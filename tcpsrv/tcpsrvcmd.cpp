#ifndef TCPSRVCMD_LIB
#define TCPSRVCMD_LIB
//{ ============================================================================
//{ Библиотека командного TCP сервера ==========================================
//{ ============================================================================

#include "tcpsrvcmd.h"
#include "..\array\array.h"
#include "..\array\array.cpp"

// Публичные функции

tcpSrvCmdBuf2vars(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR], const buf{}, bufSize)
{
    for (new i = 0; (i < TCPSRVCMD_BROKER_BUF_CELLS) && (i < countArrayUsedCells(bufSize)); i++)
        SetVar(brokerVars.buf[i], buf[i]);

    SetVar(brokerVars.bufSize, bufSize);
}

tcpSrvCmdSetToBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    SetVar(brokerVars.toBroker, true);
}

tcpSrvCmdIsBrokerSuccess(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    return GetVar(brokerVars.success) != 0;
}

tcpSrvCmdSetBrokerSuccess(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    SetVar(brokerVars.success, true);
}

tcpSrvCmdSetBrokerError(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    SetVar(brokerVars.success, false);
}

tcpSrvCmdSetFromBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    SetVar(brokerVars.fromBroker, true);
}

tcpSrvCmdIsToBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    return GetVar(brokerVars.toBroker);
}

tcpSrvCmdIsFromBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    return GetVar(brokerVars.fromBroker);
}

tcpSrvCmdIsCanceledBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    return GetVar(brokerVars.cancel);
}

tcpSrvCmdCancelBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    SetVar(brokerVars.cancel, true);
}

tcpSrvCmdVars2buf(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR], buf{}, &bufSize)
{
    bufSize = GetVar(brokerVars.bufSize);
    for (new i = 0; (i < TCPSRVCMD_BROKER_BUF_CELLS) && (i < countArrayUsedCells(bufSize)); i++)
        buf[i] = GetVar(brokerVars.buf[i]);
}

tcpSrvCmdResetFromBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    SetVar(brokerVars.fromBroker, false);
}

tcpSrvCmdResetToBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    SetVar(brokerVars.toBroker, false);
}

tcpSrvCmdResetCancelBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    SetVar(brokerVars.cancel, false);
}

tcpSrvCmdSendBufToBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR], const buf{}, bufSize)
{
    tcpSrvCmdBuf2vars(brokerVars, buf, bufSize);
    tcpSrvCmdResetCancelBroker(brokerVars);
    tcpSrvCmdResetFromBroker(brokerVars);
    tcpSrvCmdSetToBroker(brokerVars);
}

// Приватные функции


//!} Конец библиотеки командного TCP сервера ===================================
//!} ===========================================================================

#endif // TCPSRVCMD_LIB