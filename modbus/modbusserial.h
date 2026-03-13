#if !defined MODBUSSERIAL_H
#define MODBUSSERIAL_H
//{ ============================================================================
//{ Заголовок библиотеки работы с ModBus по последовательной линии =============
//{ ============================================================================

// Уровень передачи данных
// RTU (пока только)

// Термины и определения
// полезный кадр (UF) - кадр RTU без проверочного кода. К нему приводятся оба типа кадров RTU и ASCII и с ним идет работа

#include "modbus.h"

//#define MODBUSSERIAL_DEBUG // режим отладки

// Коды взаимодействия
#define MODBUSSERIAL_ANOTHER_DEV_DATA 2 // получен запрос для другого устройства
#define MODBUSSERIAL_OK 1 // успешный результат операции
#define MODBUSSERIAL_ERROR_UNEXPECTED 0 // общая ошибка, не попадающая под другие коды 
#define MODBUSSERIAL_ERROR_TIMEOUT -1 // ошибка истечения времени ожидания
#define MODBUSSERIAL_ERROR_RECEIVE -2 // ошибка передачи
#define MODBUSSERIAL_ERROR_INPUT -3 // ошибка входных данных
#define MODBUSSERIAL_ERROR_EXCODE -4 // получен код исключения

// Полезный кадр
#define MODBUSSERIAL_UF_PDU_OFFSET 1 // смещение начала PDU в полезном кадре
#define MODBUSSERIAL_UF_ADDR_SIZE 1 // размер адреса в полезном кадре
#define MODBUSSERIAL_UF_PDU_WRAP_SIZE MODBUSSERIAL_UF_ADDR_SIZE // размер обертки PDU в полезном кадре
#define MODBUSSERIAL_UF_ADDR_POS 0 // позиция адреса в полезном кадре
#define MODBUSSERIAL_UF_FUNC_POS (MODBUSSERIAL_UF_PDU_OFFSET + MODBUS_PDU_FUNCCODE_POS) // позиция кода функции в полезном кадре
#define MODBUSSERIAL_UF_FUNCDATA_POS (MODBUS_PDU_FUNCDATA_POS + MODBUSSERIAL_UF_PDU_OFFSET)
#define MODBUSSERIAL_UF_EXCODE_POS (MODBUSSERIAL_UF_PDU_OFFSET + MODBUS_PDU_EXCODE_POS) // позиция кода исключения в полезном кадре
#define MODBUSSERIAL_UF_SIZE_MIN (MODBUSSERIAL_UF_PDU_WRAP_SIZE + MODBUS_PDU_SIZE_MIN) // минимальный размер полезного кадра

// Параметры RTU
#define MODBUSSERIAL_RTU_ADDR_POS 0 // позиция адреса в режиме RTU
#define MODBUSSERIAL_RTU_ADDR_SIZE MODBUSSERIAL_UF_ADDR_SIZE // размер адреса в режиме RTU
#define MODBUSSERIAL_RTU_CC_SIZE 2 // размер проверочного кода в режиме RTU
#define MODBUSSERIAL_RTU_PDU_WRAP_SIZE (MODBUSSERIAL_UF_ADDR_SIZE + MODBUSSERIAL_RTU_CC_SIZE) // размер обертки PDU в режиме RTU
#define MODBUSSERIAL_RTU_FRAME_SIZE_MIN (MODBUS_PDU_SIZE_MIN + MODBUSSERIAL_RTU_PDU_WRAP_SIZE) // минимальная длина кадра в режиме RTU
#define MODBUSSERIAL_RTU_RESP_SIZE_MIN (MODBUS_PDU_RESPONSE_SIZE_MIN + MODBUSSERIAL_RTU_PDU_WRAP_SIZE) // минимальная длина ответа в режиме RTU
#define MODBUSSERIAL_RTU_RESPBEG_SZ_MIN (MODBUS_PDU_RESPONSE_SIZE_MIN + MODBUSSERIAL_RTU_ADDR_SIZE) // минимальный размер начала ответа в режиме RTU
#define MODBUSSERIAL_RTU_CRC_INIT 0xFFFF // начальное значение CRC для режима RTU
#define MODBUSSERIAL_RTU_FRAME_SIZE_MAX 256 // максимальный размер кадра в режиме RTU

#define MODBUSSERIAL_UF_FUNC_DATA_SIZE_MAX (MODBUSSERIAL_RTU_FRAME_SIZE_MAX - MODBUSSERIAL_RTU_PDU_WRAP_SIZE - MODBUSSERIAL_UF_ADDR_SIZE)

// Адреса
#define MODBUSSERIAL_ADDRESS_BROADCAST 0 // адрес широковещательной рассылки
#define MODBUSSERIAL_SRV_ADDRESS_MIN 1 // минимальный допустимый адрес сервера
#define MODBUSSERIAL_SRV_ADDRESS_MAX 247 // максимальный допустимый адрес сервера

#define MODBUSSERIAL_TURNAROUND_DELAY_MIN 200 // задержка клиента после широковещательной рассылки по умолчанию, мс

#define MODBUSSERIAL_BUF_SIZE_MAX MODBUSSERIAL_RTU_FRAME_SIZE_MAX // максимальный размер буфера передатчика
#define MODBUSSERIAL_PORT_BUFFER_SIZE (MODBUSSERIAL_BUF_SIZE_MAX * 2) // размер буфера порта

// Структура данных приемопередатчика
#define MODBUSSERIAL_TRANSCEIVER_DATA [\
    .portNum,\                          // номер порта
    .buf{MODBUSSERIAL_BUF_SIZE_MAX},\   // передаваемый буфер
    .bufSize,\                          // размер передаваемого буфера
    .interFrameDelay,\                  // актуальный минимальный межкадровый интервал в режиме RTU, мс
    .setInterFrameDelay,\               // настройка минимального межкадрового интервала в режиме RTU, мс
    .interCharDelay,\                   // максимальный межсимвольный интервал в режиме RTU, мс
    .inited,\                           // порт инициализирован (можно принимать данные без пропуска)
    .receiveUptime,\                    // время получения данных
]

// Расширение структуры данных клиента - настройки
#define MODBUSSERIAL_CLIENT_DATA [\
    .turnaroundDelay,\  // задержка после широковещательной рассылки, мс
]

// Расширение структуры данных сервера - настройки
#define MODBUSSERIAL_SERVER_DATA [\
    .addr,\    // адрес в сети
    .timeout,\  // таймаут приема данных, мс
]

// Общие функции
modbusSerialSetInterFrameDelay(obj[MODBUSSERIAL_TRANSCEIVER_DATA], par);
modbusSerialPortInit(obj[MODBUSSERIAL_TRANSCEIVER_DATA], portNum, baudRate, stopBits, parity);
modbusSerialGetFunction(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], &func);
modbusSerialGetPdu(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], data{}, dataSize, dataPos = 0);
modbusSerialIsSrvAddrValid(const setsrv[MODBUSSERIAL_SERVER_DATA]);
modbusSerialGetAddr(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], &addr);

// Клиент
modbusSerialCltInit(client[MODBUSSERIAL_CLIENT_DATA], turnaroundDelay);
modbusSerialCltReadRegsGetReg(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], reg, &value);
modbusSerialCltReadRegsGetRegs(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], reg, quantity, values[]);
modbusSerialCltReadHoldRegs(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], regStartAddr,
                             quantityRegs, &excode, diag);
modbusSerialCltReadInputRegs(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], regStartAddr,
                             quantityRegs, &excode, diag);
modbusSerialCltWrite1Reg(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], reg, value,
                                 &excode, diag);
modbusSerialCltWriteNregs(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], reg, 
                                const values[], quantity, &excode, diag);
modbusSerialCltCustomReq(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA],
                            const data{}, dataPos, dataSize, &excode, diag);

// Сервер
modbusSerialSrvHandler(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], const server[MODBUS_SERVER_DATA], &excode, diag);
modbusSerialSrvSendResponse(obj[MODBUSSERIAL_TRANSCEIVER_DATA], diag);
modbusSerialSrvSendException(obj[MODBUSSERIAL_TRANSCEIVER_DATA], excode, diag);

//!} Конец заголовка библиотеки работы с ModBus по последовательной линии ======
//!} ===========================================================================
#endif // MODBUSSERIAL_H