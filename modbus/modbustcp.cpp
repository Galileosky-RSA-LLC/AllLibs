#ifndef MODBUSTCP_LIB
#define MODBUSTCP_LIB
//{ ============================================================================
//{ Библиотека работы с ModBus по TCP/IP =======================================
//{ ============================================================================

#include "modbustcp.h"
#include "modbus.h"
#include "modbus.cpp"
#include "..\tcpsrv\tcpsrvcm.h"
#include "..\tcpsrv\tcpsrvcm.cpp"
#include "..\tcpsrv\tcpsrvcmd.h"
#include "..\tcpsrv\tcpsrvcmd.cpp"
#include "..\array\array.h"
#include "..\array\array.cpp"

// Публичные функции

//! Установить TCP порт
modbusTcpSetPort(trans[MODBUSTCP_TRANSCEIVER_DATA], tcpPort)
{
    trans.tcpPort = tcpPort;
}

//! Получить код функции из буфера приемопередатчика
//! \param[in] trans объект приемопередатчика
//! \param[out] func код функции при успешном возврате
//! \return MODBUSTCP_OK - успешно, MODBUSTCP_ERROR_INPUT - ошибка
modbusTcpGetFunction(const trans[MODBUSTCP_TRANSCEIVER_DATA], &func)
{
    return modbusGetFunction(trans.buf, MODBUSTCP_ADU_PDU_OFFSET, MODBUSTCP_BUF_SIZE_MAX, func) ? MODBUSTCP_OK : MODBUSTCP_ERROR_INPUT;
}

//! Инициализировать сервер
//! \return true - успешно, false - ошибка
modbusTcpSrvInit(const trans[MODBUSTCP_TRANSCEIVER_DATA], tcpSrv[MODBUSTCP_SERVER_DATA], const gatewayVars[TCPSRVCMD_BROKER_VARS_ADDR])
{
    tcpSrvCmdCancelBroker(gatewayVars);
    ServerSocketRelease();
    tcpSrv.maxCons = min(ServerMaxConnections(), TCPSRVCMD_CONNECTIONS_MAX);
    for (new i = 0; i < tcpSrv.maxCons; i++)
    {
        tcpSrv.conHandlers[i] = 0;
        tcpSrv.conActivityUptimes[i] = 0;
    }
    tcpSrv.lastCon = -1;
    tcpSrv.conUsedByGateway = false;
    return ServerSocketOccupy(trans.tcpPort);
}

modbusTcpSrvSetGateway(tcpSrv[MODBUSTCP_SERVER_DATA], need)
{
    tcpSrv.serialGateway = need != 0;
}

modbusTcpSrvAmountClients(const tcpSrv[MODBUSTCP_SERVER_DATA])
{
    new amount;
    for (new i = 0; (i < TCPSRVCMD_CONNECTIONS_MAX) && (i < tcpSrv.maxCons); i++)
    {
        if (tcpSrv.conHandlers[i] >= 0)
            amount++;
    }
    return amount;
}

//! Основной обработчик сервера. 
//! Принимает запросы от клиентов: запросы на шлюзование перенаправляются внешнему обработчику, его результат перенаправляется клиенту; 
//! при получении запроса на локальную обработку останавливается для его дальнейшей обработки; 
//! при запросе неподдерживаемой функции локального сервера отправляет ответ с кодом ошибки. 
//! Предварительно должна быть выполнена инициализация с помощью modbusTcpSrvInit(). 
//! Должен вызываться в основном цикле 
//! \param[inout] trans объект приемопередатчика
//! \param[inout] tcpSrv объект Modbus TCP сервера
//! \param[in] server объект Modbus сервера
//! \param[in] gatewayVars объект с адресами переменных для внешней обработки (шлюза)
//! \param[out] excode сформированный код исключения
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSTCP_OK - получен запрос локальному серверу, MODBUSTCP_ANOTHER_DEV_DATA - получены данные шлюза, MODBUSTCP_ERROR_ при ошибке
modbusTcpSrvHandler(trans[MODBUSTCP_TRANSCEIVER_DATA], tcpSrv[MODBUSTCP_SERVER_DATA], const server[MODBUS_SERVER_DATA],
                     const gatewayVars[TCPSRVCMD_BROKER_VARS_ADDR], &excode, diag)
{
    excode = MODBUS_EXCODE_OK;
    if (diag)
        Diagnostics("mbtSh");

    new res = modbusTcp_srvConMngGetRequest(trans, tcpSrv, gatewayVars, diag);
    if (res != MODBUSTCP_OK)
        return res;

    res = modbusTcp_srvCheckReq(trans, tcpSrv, server, excode);
    if (res == MODBUSTCP_ERROR_EXCODE)
    {    
        res = modbusTcpSrvSendException(trans, tcpSrv, excode, diag);
        return (res == MODBUSTCP_OK) ? MODBUSTCP_ERROR_EXCODE : res;
    }
    if (res != MODBUSTCP_OK)
        return res;

    if (tcpSrv.serialGateway)
    {
        if (!modbusTcp_isUnitIdOwn(trans))
            res = modbusTcp_srvGatewayReqProc(trans, tcpSrv, gatewayVars, excode, diag);
    }
    else
    {
        if (tcpSrvCmdIsToBroker(gatewayVars) && !tcpSrvCmdIsCanceledBroker(gatewayVars))
            tcpSrvCmdCancelBroker(gatewayVars);

        if (tcpSrvCmdIsFromBroker(gatewayVars))
        {
            tcpSrvCmdResetCancelBroker(gatewayVars);
            tcpSrvCmdResetFromBroker(gatewayVars);
            tcpSrvCmdResetToBroker(gatewayVars);
            tcpSrv.conUsedByGateway = false;
            tcpSrv.gatewayCon = -1;
        }
    }
    return res;
}

//! Отправить код исключения клиенту в ответ на его запрос. 
//! \param[inout] trans объект приемопередатчика
//! \param[in] tcpSrv объект tcp сервера
//! \param[in] excode код исключения MODBUS_EXCODE_
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSTCP_OK - успешно, MODBUSTCP_ERROR_ при ошибке
modbusTcpSrvSendException(trans[MODBUSTCP_TRANSCEIVER_DATA], const tcpSrv[MODBUSTCP_SERVER_DATA], excode, diag)
{
    if (diag)
        Diagnostics("mbtSsE");

    if (!modbusSrvMakeException(trans.buf, MODBUSTCP_ADU_PDU_OFFSET, MODBUSTCP_BUF_SIZE_MAX, trans.bufSize, excode))
        return MODBUSTCP_ERROR_INPUT;

    num16bit2arrayBe(MODBUSTCP_EXCEPTION_ADU_LEN, trans.buf, MODBUSTCP_ADU_MBAP_LEN_POS, trans.bufSize);
    return modbusTcp_srvSendData(trans, tcpSrv, diag);
}

//! Отправить сформированный в буфере приемопередатчика ответ клиенту на его запрос
//! \param[inout] trans объект приемопередатчика
//! \param[in] tcpSrv объект tcp сервера
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusTcpSrvSendResponse(trans[MODBUSTCP_TRANSCEIVER_DATA], const tcpSrv[MODBUSTCP_SERVER_DATA], diag)
{
    if (diag)
        Diagnostics("mbtSsR");

    if (trans.bufSize < MODBUSTCP_ADU_SIZE_MIN)
        return MODBUSTCP_ERROR_INPUT;

    num16bit2arrayBe(trans.bufSize - MODBUSTCP_ADU_MBAP_WITHOUT_UNIT_ID_SIZE, trans.buf, MODBUSTCP_ADU_MBAP_LEN_POS, trans.bufSize);
    return modbusTcp_srvSendData(trans, tcpSrv, diag);
}

// Приватные функции

//! Управление соединениями и получение запроса
modbusTcp_srvConMngGetRequest(trans[MODBUSTCP_TRANSCEIVER_DATA], tcpSrv[MODBUSTCP_SERVER_DATA], const gatewayVars[TCPSRVCMD_BROKER_VARS_ADDR], diag)
{
    if (diag)
        Diagnostics("mbt_sGr");

    new connections[TCPSRVCMD_CONNECTIONS_MAX][TCPSRVCM_CONNECTION_DATA];
    for (new i = 0; i < tcpSrv.maxCons; i++)
    {
        connections[i].handle = tcpSrv.conHandlers[i];
        connections[i].activityUptime = tcpSrv.conActivityUptimes[i];
    }
    new res = MODBUSTCP_ERROR_TIMEOUT;
    for (new i = 0; (i < tcpSrv.maxCons) && (res != MODBUSTCP_OK); i++)
    {
        if (++tcpSrv.lastCon >= tcpSrv.maxCons)
            tcpSrv.lastCon = 0;
        
        new handle = ServerGetHandle(tcpSrv.lastCon);
        if (diag)
            Diagnostics("con=%d,h=%d", tcpSrv.lastCon, handle);

        new isHandleChanged = tcpSrvCmIsHandleChanged(connections[tcpSrv.lastCon], handle);
        if (isHandleChanged && tcpSrv.conUsedByGateway && (tcpSrv.gatewayCon == tcpSrv.lastCon))
        {
            if (tcpSrvCmdIsToBroker(gatewayVars) && !tcpSrvCmdIsCanceledBroker(gatewayVars))
                tcpSrvCmdCancelBroker(gatewayVars);

            tcpSrv.conUsedByGateway = false;
        }
        if (handle >= 0)
        {
            if (isHandleChanged)
            {    
                tcpSrvCmInitConnection(connections[tcpSrv.lastCon], handle);
                if (tcpSrvCmGetUsedConnections(connections, tcpSrv.maxCons) >= tcpSrv.maxCons)
                {    
                    new oldestCon = tcpSrvCmGetOldestConnection(connections, tcpSrv.maxCons);
                    if ((oldestCon == tcpSrv.gatewayCon) && tcpSrv.conUsedByGateway)
                    {
                        new activityTime = connections[oldestCon].activityUptime;
                        connections[oldestCon].activityUptime = -1;
                        oldestCon = tcpSrvCmGetOldestConnection(connections, tcpSrv.maxCons);
                        connections[oldestCon].activityUptime = activityTime;
                    }
                    ServerSocketClose(connections[oldestCon].handle);
                }
            }
            res = modbusTcp_srvGetData(trans, handle, diag);
        }
        else if (diag)
        {
            Diagnostics("no clt");
        }
        tcpSrvCmSetHandle(connections[tcpSrv.lastCon], handle);
        Delay(MODBUSTCP_SRV_DELAY_MS);
    }    
    if (res == MODBUSTCP_OK)
        tcpSrvCmActivateConnection(connections[tcpSrv.lastCon]);

    for (new i = 0; i < tcpSrv.maxCons; i++)
    {
        tcpSrv.conHandlers[i] = connections[i].handle;
        tcpSrv.conActivityUptimes[i] = connections[i].activityUptime;
    }
    return res;
}

//! Принять данные от клиента. 
//! Предварительно должно быть установлено соединение. 
//! Приватная
//! \param[inout] trans объект приемопередатчика
//! \param[in] handle хэндл соединения
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSTCP_OK - успешно, MODBUSTCP_ERROR_ при ошибке
modbusTcp_srvGetData(trans[MODBUSTCP_TRANSCEIVER_DATA], handle, diag)
{
    new dataReceived = ServerSocketRecv(handle, trans.buf, MODBUSTCP_BUF_SIZE_MAX, trans.bufSize, MODBUSTCP_SRV_RECEIVE_TIMEOUT_MS);
    dataReceived = dataReceived && (trans.bufSize > 0);
    if (diag)
    {
        Diagnostics("rcv %d bts", trans.bufSize);
        #ifdef DEBUG
        DiagnosticsHex(trans.buf, trans.bufSize);
        #endif
    }
    return dataReceived ? MODBUSTCP_OK : MODBUSTCP_ERROR_TIMEOUT;
}

//! Первично проверить запрос от клиента на соответствие протоколу и поддерживаемым сервером функциям. 
//! Предварительно должен быть принят запрос с помощью modbusTcp_srvGetData() 
//! Приватная
//! \param[inout] trans объект приемопередатчика
//! \param[in] server объект сервера
//! \param[out] excode код исключения при возврате MODBUSTCP_ERROR_EXCODE
//! \return MODBUSTCP_OK - успешно, MODBUSTCP_ERROR_ при ошибке
modbusTcp_srvCheckReq(const trans[MODBUSTCP_TRANSCEIVER_DATA], const tcpSrv[MODBUSTCP_SERVER_DATA], const server[MODBUS_SERVER_DATA], &excode)
{
    if (!modbusTcp_isAduValid(trans))
        return MODBUSTCP_ERROR_TRANSFER;

    new res = MODBUSTCP_OK;
    if (modbusTcp_isUnitIdOwn(trans) || !tcpSrv.serialGateway)
    {
        new func;
        modbusGetFunction(trans.buf, MODBUSTCP_ADU_PDU_OFFSET, MODBUSTCP_BUF_SIZE_MAX, func);
        res = modbusSrvIsSupportedFunc(server, func) ? MODBUSTCP_OK : MODBUSTCP_ERROR_EXCODE;
        if (res == MODBUSTCP_ERROR_EXCODE)
            excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
    }
    return res;
}

//! Проверить принятый ADU
//! Приватная
//! \param[in] trans объект приемопередатчика
modbusTcp_isAduValid(const trans[MODBUSTCP_TRANSCEIVER_DATA])
{
    new protId;
    array2num16beUnSign(trans.buf, MODBUSTCP_ADU_MBAP_PROT_ID_POS, trans.bufSize, protId);
    new len;
    array2num16beUnSign(trans.buf, MODBUSTCP_ADU_MBAP_LEN_POS, trans.bufSize, len);
    return (trans.bufSize >= MODBUSTCP_ADU_SIZE_MIN)
            && (protId == 0)
            && (len <= (trans.bufSize - MODBUSTCP_ADU_MBAP_WITHOUT_UNIT_ID_SIZE));
}

modbusTcp_isUnitIdOwn(const trans[MODBUSTCP_TRANSCEIVER_DATA])
{
    return trans.buf{MODBUSTCP_ADU_MBAP_UNIT_ID_POS} == MODBUSTCP_OWN_UNIT_ID;
}

//! Отправить данные буфера приемопередатчика клиенту
//! \param[inout] trans объект приемопередатчика
//! \return MODBUSSERIAL_OK - успешно, MODBUSTCP_ERROR_TRANSFER - ошибка
modbusTcp_srvSendData(const trans[MODBUSTCP_TRANSCEIVER_DATA], const tcpSrv[MODBUSTCP_SERVER_DATA], diag)
{
    #ifdef DEBUG
    if (diag)
        Diagnostics("mbt_sSd");
    #endif
    new sent;
    new res = ServerSocketSend(tcpSrv.conHandlers[tcpSrv.lastCon], trans.buf, trans.bufSize, sent);
    if (diag)
    {
        Diagnostics("sent %d bts", sent);
        #ifdef DEBUG
        DiagnosticsHex(trans.buf, sent);
        #endif
    }
    return res ? MODBUSTCP_OK : MODBUSTCP_ERROR_TRANSFER;
}

modbusTcp_srvGatewayReqProc(trans[MODBUSTCP_TRANSCEIVER_DATA], tcpSrv[MODBUSTCP_SERVER_DATA], const gatewayVars[TCPSRVCMD_BROKER_VARS_ADDR], &excode, diag)
{
    if (diag)
        Diagnostics("mbt_sGrP");

    new res;
    new toGateway = tcpSrvCmdIsToBroker(gatewayVars);
    new fromGateway = tcpSrvCmdIsFromBroker(gatewayVars);
    new isGatewayHandle = tcpSrv.gatewayConHandle == tcpSrv.conHandlers[tcpSrv.lastCon];
    if ((toGateway && !fromGateway) || (fromGateway && !isGatewayHandle && tcpSrv.conUsedByGateway))
    {
        excode = MODBUS_EXCODE_SERVER_DEV_BUSY;
        res = modbusTcpSrvSendException(trans, tcpSrv, excode, diag);
        return (res == MODBUSTCP_OK) ? MODBUSTCP_ERROR_EXCODE : res;
    }
    new repeatReq = countIdenticalStr(trans.buf, MODBUSTCP_ADU_MBAP_LEN_POS, trans.bufSize, tcpSrv.gatewayReq, MODBUSTCP_ADU_MBAP_LEN_POS,
                                        tcpSrv.gatewayReqSize) == (tcpSrv.gatewayReqSize - MODBUSTCP_ADU_MBAP_LEN_POS);
    if (fromGateway && tcpSrv.conUsedByGateway && isGatewayHandle && repeatReq)
    {
        new reqTransactionId;
        array2num16beUnSign(trans.buf, MODBUSTCP_ADU_MBAP_TRANSACTION_ID_POS, trans.bufSize, reqTransactionId);
        new bufSize;
        if (tcpSrvCmdIsCanceledBroker(gatewayVars) || !tcpSrvCmdIsBrokerSuccess(gatewayVars))
            bufSize = 0;
        else
            tcpSrvCmdVars2buf(gatewayVars, trans.buf, bufSize);

        if (bufSize > 0)
        {    
            trans.bufSize = bufSize;
            num16bit2arrayBe(reqTransactionId, trans.buf, MODBUSTCP_ADU_MBAP_TRANSACTION_ID_POS, trans.bufSize);
            res = modbusTcp_srvSendData(trans, tcpSrv, diag);
        }
        else
        {    
            excode = tcpSrvCmdIsBrokerSuccess(gatewayVars) ? MODBUS_EXCODE_GATEWAY_TARGET : MODBUS_EXCODE_GATEWAY_PATH;
            res = modbusTcpSrvSendException(trans, tcpSrv, excode, diag);
        }
        if (res == MODBUSTCP_OK)
            res = MODBUSTCP_ANOTHER_DEV_DATA;

        tcpSrvCmdResetCancelBroker(gatewayVars);
        tcpSrvCmdResetFromBroker(gatewayVars);
        tcpSrvCmdResetToBroker(gatewayVars);
        tcpSrv.conUsedByGateway = false;
        return res;
    }
    tcpSrvCmdSendBufToBroker(gatewayVars, trans.buf, trans.bufSize);
    tcpSrv.gatewayCon = tcpSrv.lastCon;
    tcpSrv.gatewayConHandle = tcpSrv.conHandlers[tcpSrv.gatewayCon];
    tcpSrv.conUsedByGateway = true;
    tcpSrv.gatewayReqSize = insertArrayStr(tcpSrv.gatewayReq, 0, MODBUSTCP_BUF_SIZE_MAX, trans.buf, trans.bufSize);
    excode = MODBUS_EXCODE_ACKNOWLEDGE;
    res = modbusTcpSrvSendException(trans, tcpSrv, excode, diag);
    if (res == MODBUSTCP_OK)
        res = MODBUSTCP_ANOTHER_DEV_DATA;

    return res;
}

//!} Конец библиотеки работы с ModBus по TCP/IP ================================
//!} ===========================================================================

#endif // MODBUSTCP_LIB