#ifndef TCPSRVCM_H
#define TCPSRVCM_H
//{ ============================================================================
//{ Заголовок менеджера соединений TCP сервера =================================
//{ ============================================================================

#define TCPSRVCM_CONNECTION_DATA [\
    .handle,\
    .activityUptime\
]

tcpSrvCmActivateConnection(connection[TCPSRVCM_CONNECTION_DATA]);
tcpSrvCmGetOldestConnection(const connections[][TCPSRVCM_CONNECTION_DATA], maxConnections);
tcpSrvCmGetUsedConnections(const connections[][TCPSRVCM_CONNECTION_DATA], maxConnections);
tcpSrvCmInitConnection(connection[TCPSRVCM_CONNECTION_DATA], newHandle);
tcpSrvCmIsHandleChanged(const connection[TCPSRVCM_CONNECTION_DATA], handle);
tcpSrvCmSetHandle(connection[TCPSRVCM_CONNECTION_DATA], newHandle);

//!} Конец заголовка менеджера соединений TCP сервера ==========================
//!} ===========================================================================

#endif // TCPSRVCM_H