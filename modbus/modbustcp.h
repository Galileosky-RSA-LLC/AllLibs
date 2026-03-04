#if !defined MODBUSTCP_H
#define MODBUSTCP_H
//{ ============================================================================
//{ Библиотека работы с ModBus по TCP/IP =======================================
//{ ============================================================================

#include "modbus.h"
#include "..\tcpsrv\tcpsrvcmd.h"

//#define MODBUSTCP_DEBUG // режим отладки

// Коды взаимодействия
#define MODBUSTCP_ANOTHER_DEV_DATA 2 // получены данные для другого устройства
#define MODBUSTCP_OK 1 // успешный результат операции
#define MODBUSTCP_ERROR_UNEXPECTED 0 // общая ошибка, не попадающая под другие коды 
#define MODBUSTCP_ERROR_TIMEOUT -1 // ошибка истечения времени ожидания
#define MODBUSTCP_ERROR_TRANSFER -2 // ошибка передачи
#define MODBUSTCP_ERROR_INPUT -3 // ошибка входных данных
#define MODBUSTCP_ERROR_EXCODE -4 // получен код исключения

#define MODBUSTCP_PORT_DEFAULT 502
#define MODBUSTCP_OWN_UNIT_ID 255

#define MODBUSTCP_ADU_MBAP_TRANSACTION_ID_POS 0
#define MODBUSTCP_ADU_MBAP_TRANSACTION_ID_SIZE 2
#define MODBUSTCP_ADU_MBAP_PROT_ID_POS (MODBUSTCP_ADU_MBAP_TRANSACTION_ID_POS + MODBUSTCP_ADU_MBAP_TRANSACTION_ID_SIZE)
#define MODBUSTCP_ADU_MBAP_PROT_ID_SIZE 2
#define MODBUSTCP_ADU_MBAP_LEN_POS (MODBUSTCP_ADU_MBAP_PROT_ID_POS + MODBUSTCP_ADU_MBAP_PROT_ID_SIZE)
#define MODBUSTCP_ADU_MBAP_LEN_SIZE 2
#define MODBUSTCP_ADU_MBAP_UNIT_ID_POS (MODBUSTCP_ADU_MBAP_LEN_POS + MODBUSTCP_ADU_MBAP_LEN_SIZE)
#define MODBUSTCP_ADU_MBAP_UNIT_ID_SIZE 1
#define MODBUSTCP_ADU_MBAP_SIZE (MODBUSTCP_ADU_MBAP_TRANSACTION_ID_SIZE + MODBUSTCP_ADU_MBAP_PROT_ID_SIZE + MODBUSTCP_ADU_MBAP_LEN_SIZE \
                                + MODBUSTCP_ADU_MBAP_UNIT_ID_SIZE)
#define MODBUSTCP_ADU_MBAP_WITHOUT_UNIT_ID_SIZE (MODBUSTCP_ADU_MBAP_SIZE - MODBUSTCP_ADU_MBAP_UNIT_ID_SIZE)
#define MODBUSTCP_ADU_PDU_OFFSET MODBUSTCP_ADU_MBAP_SIZE
#define MODBUSTCP_ADU_PDU_FUNC_POS MODBUSTCP_ADU_PDU_OFFSET
#define MODBUSTCP_ADU_PDU_EXCODE_POS (MODBUSTCP_ADU_PDU_OFFSET + MODBUS_PDU_EXCODE_POS)
#define MODBUSTCP_ADU_PDU_FUNCDATA_POS (MODBUSTCP_ADU_PDU_OFFSET + MODBUS_PDU_FUNCDATA_POS)
#define MODBUSTCP_ADU_SIZE_MIN (MODBUSTCP_ADU_MBAP_SIZE + MODBUS_PDU_SIZE_MIN)
#define MODBUSTCP_ADU_SIZE_MAX (MODBUSTCP_ADU_MBAP_SIZE + MODBUS_PDU_SIZE_MAX)

#define MODBUSTCP_EXCEPTION_ADU_SIZE (MODBUSTCP_ADU_MBAP_SIZE + MODBUS_EXCEPTION_PDU_SIZE)
#define MODBUSTCP_EXCEPTION_ADU_LEN (MODBUSTCP_ADU_MBAP_UNIT_ID_SIZE + MODBUS_EXCEPTION_PDU_SIZE)

#define MODBUSTCP_BUF_SIZE_MAX MODBUSTCP_ADU_SIZE_MAX 

#define MODBUSTCP_SRV_RECEIVE_TIMEOUT_MS 40
#define MODBUSTCP_SRV_DELAY_MS 10

#define MODBUSTCP_TRANSCEIVER_DATA [ \
    .tcpPort,\
    .buf{MODBUSTCP_BUF_SIZE_MAX},\
    .bufSize,\
]

// Расширение структуры данных сервера
#define MODBUSTCP_SERVER_DATA [ \
    .maxCons,\
    .lastCon,\
    .conHandlers[TCPSRVCMD_CONNECTIONS_MAX],\
    .conActivityUptimes[TCPSRVCMD_CONNECTIONS_MAX],\
    .conUsedByGateway,\
    .gatewayCon,\
    .gatewayConHandle,\
    .serialGateway,\
    .gatewayReq{MODBUSTCP_BUF_SIZE_MAX},\
    .gatewayReqSize,\
]

// Общие функции
modbusTcpGetFunction(const trans[MODBUSTCP_TRANSCEIVER_DATA], &func);
modbusTcpSetPort(trans[MODBUSTCP_TRANSCEIVER_DATA], tcpPort);

// Клиент


// Сервер
modbusTcpSrvInit(const trans[MODBUSTCP_TRANSCEIVER_DATA], tcpSrv[MODBUSTCP_SERVER_DATA], const gatewayVars[TCPSRVCMD_BROKER_VARS_ADDR]);
modbusTcpSrvAmountClients(const tcpSrv[MODBUSTCP_SERVER_DATA]);
modbusTcpSrvSetGateway(tcpSrv[MODBUSTCP_SERVER_DATA], need);
modbusTcpSrvHandler(trans[MODBUSTCP_TRANSCEIVER_DATA], tcpSrv[MODBUSTCP_SERVER_DATA], const server[MODBUS_SERVER_DATA],
                     const gatewayVars[TCPSRVCMD_BROKER_VARS_ADDR], &excode, diag);
modbusTcpSrvSendResponse(trans[MODBUSTCP_TRANSCEIVER_DATA], const tcpSrv[MODBUSTCP_SERVER_DATA], diag);
modbusTcpSrvSendException(trans[MODBUSTCP_TRANSCEIVER_DATA], const tcpSrv[MODBUSTCP_SERVER_DATA], excode, diag);

//!} Конец библиотеки работы с ModBus по TCP/IP ================================
//!} ===========================================================================

#endif // MODBUSTCP_H