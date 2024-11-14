#ifndef TCPSRVCMD_H
#define TCPSRVCMD_H
//{ ============================================================================
//{ Заголовок командного TCP сервера ===========================================
//{ ============================================================================

#define TCPSRVCMD_CONNECTIONS_MAX 4

#define TCPSRVCMD_BUF_MAX_SIZE 260 // максимум 512
#define TCPSRVCMD_BROKER_BUF_CELLS ((TCPSRVCMD_BUF_MAX_SIZE / 4) + ((TCPSRVCMD_BUF_MAX_SIZE % 4) != 0))

#define TCPSRVCMD_BROKER_VARS_ADDR [\
    .buf[TCPSRVCMD_BROKER_BUF_CELLS],\
    .bufSize,\
    .toBroker,\
    .cancel,\
    .fromBroker,\
    .success\
]

tcpSrvCmdIsBrokerSuccess(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdIsCanceledBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdIsFromBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdIsToBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdBuf2vars(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR], const buf{}, bufSize);
tcpSrvCmdVars2buf(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR], buf{}, &bufSize);
tcpSrvCmdSendBufToBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR], const buf{}, bufSize);
tcpSrvCmdSetBrokerError(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdSetBrokerSuccess(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdSetFromBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdSetToBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdCancelBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdResetCancelBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdResetFromBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);
tcpSrvCmdResetToBroker(const brokerVars[TCPSRVCMD_BROKER_VARS_ADDR]);

//!} Конец заголовка командного TCP сервера ====================================
//!} ===========================================================================
#endif // TCPSRVCMD_H