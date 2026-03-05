#if !defined TCPSRVCM_LIB
#define TCPSRVCM_LIB
//{ ============================================================================
//{ Библиотека менеджера соединений TCP сервера ================================
//{ ============================================================================

#include "tcpsrvcm.h"

#include "..\time\time.h"
#include "..\time\time.cpp"

// Публичные функции

//! Обновить время активности соединения
tcpSrvCmActivateConnection(connection[TCPSRVCM_CONNECTION_DATA])
{
    connection.activityUptime = GetVar(UPTIME);
}

//! Инициализировать соединение
tcpSrvCmInitConnection(connection[TCPSRVCM_CONNECTION_DATA], newHandle)
{
    tcpSrvCmSetHandle(connection, newHandle);
    connection.activityUptime = GetVar(UPTIME);
}

tcpSrvCmIsHandleChanged(const connection[TCPSRVCM_CONNECTION_DATA], handle)
{
    return connection.handle != handle;
}

//! Найти самое старое соединение
tcpSrvCmGetOldestConnection(const connections[][TCPSRVCM_CONNECTION_DATA], maxConnections)
{
    new now = GetVar(UPTIME);
    new connId = -1;
    new maxDuration;
    new maxOverflows;
    for (new i = 0; i < maxConnections; i++)
    {
        if (connections[i].handle < 0)
            continue;

        new overflows;
        new duration = durationMs(connections[i].activityUptime, now, overflows);
        if ((connId < 0) || (overflows > maxOverflows) || ((overflows == maxOverflows) && (duration > maxDuration)))
        {
            connId = i;
            maxDuration = duration;
            maxOverflows = overflows;
        }
    }
    return connId;
}

//! Получить количество активных соединений
tcpSrvCmGetUsedConnections(const connections[][TCPSRVCM_CONNECTION_DATA], maxConnections)
{
    new count;
    for (new i = 0; i < maxConnections; i++)
    {
        if (connections[i].handle >= 0)
            count++;
    }
    return count;
}

tcpSrvCmSetHandle(connection[TCPSRVCM_CONNECTION_DATA], newHandle)
{
    connection.handle = newHandle;
}

// Приватные функции



//!} Конец библиотеки менеджера соединений TCP сервера =========================
//!} ===========================================================================

#endif // TCPSRVCM_LIB