#if !defined MODBUS_H
#define MODBUS_H
//{ ============================================================================
//{ Заголовок библиотеки работы с ModBus =======================================
//{ ============================================================================

// Прикладной уровень

// PDU - protocol data unit (блок данных протокола)
// Ведущее устройство == Master == клиент
// Ведомое устройство == Slave  == сервер
// аналог - 16-битный регистр
// дискрет - 1-битный регистр

// PDU
#define MODBUS_PDU_SIZE_MIN 1 // минимальный размер блока данных протокола
#define MODBUS_PDU_SIZE_MAX 253 // максимальный размер блока данных протокола
#define MODBUS_PDU_RESPONSE_SIZE_MIN 2 // минимальный размер PDU ответа сервера
#define MODBUS_PDU_FUNCCODE_POS 0 // позиция кода функции в PDU
#define MODBUS_PDU_FUNCCODE_SIZE 1
#define MODBUS_PDU_FUNCDATA_POS (MODBUS_PDU_FUNCCODE_POS + MODBUS_PDU_FUNCCODE_SIZE)
#define MODBUS_PDU_STARTREG_POS 1 // позиция стартового регистра в PDU запроса клиента
#define MODBUS_PDU_QREGS_POS 3 // позиция количества регистров в PDU запроса клиента
#define MODBUS_PDU_OUT_VAL_POS 3 // позиция устанавливаемой одной величины в PDU запроса клиента
#define MODBUS_PDU_OUTS_VAL_POS 6 // позиция начала устанавливаемых величин в PDU запроса клиента
#define MODBUS_PDU_EXCODE_POS 1 // позиция кода исключения в ответе
#define MODBUS_PDU_EXCODE_SIZE 1

#define MODBUS_EXCEPTION_PDU_SIZE (MODBUS_PDU_SIZE_MIN + MODBUS_PDU_EXCODE_SIZE)

// Коды исключений в ответе с ошибкой
#define MODBUS_EXCODE_OK 0                  // нет ошибки
#define MODBUS_EXCODE_ILLEGAL_FUNCTION 1    // принятый код функции не может быть обработан
#define MODBUS_EXCODE_ILLEGAL_DATA_ADDR 2   // адрес данных, указанный в запросе, недоступен
#define MODBUS_EXCODE_ILLEGAL_DATA_VAL 3    // значение, содержащееся в поле данных запроса, является недопустимой величиной
#define MODBUS_EXCODE_SERVER_DEV_FAIL 4     // невосстанавливаемая ошибка имела место, пока сервер пытался выполнить затребованное действие
#define MODBUS_EXCODE_ACKNOWLEDGE 5         // сервер принял запрос и обрабатывает его. Предохраняет клиента от генерации ошибки тайм-аута
#define MODBUS_EXCODE_SERVER_DEV_BUSY 6     // сервер занят обработкой команды. Клиент должен повторить сообщение позже
#define MODBUS_EXCODE_MEMORY_PARITY 8       // сервер при чтении расширенной памяти обнаружил ошибку паритета
#define MODBUS_EXCODE_GATEWAY_PATH 10       // шлюз: путь недоступен (не настроен или перегружен)
#define MODBUS_EXCODE_GATEWAY_TARGET 11     // шлюз: нет ответа от целевого устройства

// Коды функций
#define MODBUS_FUNCTION_CODE_MIN 1 // минимальный код функции
#define MODBUS_FUNCTION_CODE_MAX 127 // максимальный код функции
#define MODBUS_FUNCTIONS_MAX 127 // максимальное количество функций
#define MODBUS_FUNCTION_CODE_ER_OFFSET 0x80 // смещение кода функции в ответе с ошибкой

// Публичные функции
#define MODBUS_PUBFUNC_R1_CODE_MIN 1 // минимальный код публичной функции диапазона 1
#define MODBUS_PUBFUNC_R1_CODE_MAX 64 // максимальный код публичной функции диапазона 1
#define MODBUS_PUBFUNCTIONS_R1_MAX (MODBUS_PUBFUNC_R1_CODE_MAX - MODBUS_PUBFUNC_R1_CODE_MIN + 1) // кол-во публичных функций диапазона 1
#define MODBUS_PUBFUNC_R2_CODE_MIN 73 // минимальный код публичной функции диапазона 2
#define MODBUS_PUBFUNC_R2_CODE_MAX 99 // максимальный код публичной функции диапазона 2
#define MODBUS_PUBFUNCTIONS_R2_MAX (MODBUS_PUBFUNC_R2_CODE_MAX - MODBUS_PUBFUNC_R2_CODE_MIN + 1) // кол-во публичных функций диапазона 2
#define MODBUS_PUBFUNC_R3_CODE_MIN 111 // минимальный код публичной функции диапазона 3
#define MODBUS_PUBFUNC_R3_CODE_MAX 127 // максимальный код публичной функции диапазона 3
#define MODBUS_PUBFUNCTIONS_R3_MAX (MODBUS_PUBFUNC_R3_CODE_MAX - MODBUS_PUBFUNC_R3_CODE_MIN + 1) // кол-во публичных функций диапазона 3
#define MODBUS_PUBFUNCTIONS_MAX (MODBUS_PUBFUNCTIONS_R1_MAX + MODBUS_PUBFUNCTIONS_R2_MAX + MODBUS_PUBFUNCTIONS_R3_MAX) // общее кол-во публичных функций
#define MODBUS_PUBFUNC_STARTREG_R1_MIN 1 // минимальный код публичной функции с параметром стартового регистра диапазона 1
#define MODBUS_PUBFUNC_STARTREG_R1_MAX 6 // максимальный код публичной функции с параметром стартового регистра диапазона 1
#define MODBUS_PUBFUNC_STARTREG_R2_MIN 15 // минимальный код публичной функции с параметром стартового регистра диапазона 2
#define MODBUS_PUBFUNC_STARTREG_R2_MAX 16 // максимальный код публичной функции с параметром стартового регистра диапазона 2
#define MODBUS_PUBFUNC_QREGS_R1_MIN 1 // минимальный код публичной функции с параметром количества регистров диапазона 1
#define MODBUS_PUBFUNC_QREGS_R1_MAX 4 // максимальный код публичной функции с параметром количества регистров диапазона 1
#define MODBUS_PUBFUNC_QREGS_R2_MIN 15 // минимальный код публичной функции с параметром количества регистров диапазона 2
#define MODBUS_PUBFUNC_QREGS_R2_MAX 16 // максимальный код публичной функции с параметром количества регистров диапазона 2

#define MODBUS_FUNC_READCOILS 0x01 // код публичной функции чтения катушек
#define MODBUS_FUNC_READDI 0x02 // код публичной функции чтения дискретных входов
#define MODBUS_FUNC_READHLDREGS 0x03 // код публичной функции чтения регистров временного хранения
#define MODBUS_FUNC_READINPREGS 0x04 // код публичной функции чтения входных регистров
#define MODBUS_FUNC_WRITESINGLECOIL 0x05 // код публичной функции записи одной катушки
#define MODBUS_FUNC_WRITESINGLEREG 0x06 // код публичной функции записи одного регистра
#define MODBUS_FUNC_WRITEMULTICOILS 0x0F // код публичной функции записи нескольких катушек
#define MODBUS_FUNC_WRITEMULTIREGS 0x10 // код публичной функции записи нескольких регистров

#define MODBUS_FUNC_RBITS_QUANTITY_MAX 2000 // максимальное количество запрашиваемых бит функциями чтения битов
#define MODBUS_FUNC_RREGS_QUANTITY_MAX 125 // максимальное количество запрашиваемых регистров функциями чтения регистров
#define MODBUS_FUNC_WBITS_QUANTITY_MAX 1968 // максимальное количество запрашиваемых бит функциями записи битов
#define MODBUS_FUNC_WREGS_QUANTITY_MAX 123 // максимальное количество запрашиваемых регистров функциями записи регистров

#define MODBUS_FUNC_ERROR_OFFSET 0x80 // смещение кода функции при ошибке

// Пользовательские функции
#define MODBUS_UFUNCTIONS_MAX (MODBUS_FUNCTIONS_MAX - MODBUS_PUBFUNCTIONS_MAX) // максимальное кол-во пользовательских функций

// Регистры
#define MODBUS_REGISTER_ADDRESS_MAX 65535 // наибольший адрес регистра
#define MODBUS_STATE_ON 0xFF00 // включенное состояние дискрета
#define MODBUS_STATE_OFF 0x0000 // выключенное состояние дискрета

// Структура сервера
#define MODBUS_SERVER_DATA [\
    .funcCount,\                        // количество поддерживаемых функций
    .functions{MODBUS_FUNCTIONS_MAX}\   // коды поддерживаемых функций
]

// Общие функции
modbusGetFunction(const pdu{}, start, pduSize, &par);
modbusIsFuncValid(par);
modbusIsExcodeValid(excode);
modbusGetReqMinSize(pubfunc);
modbusGetRespSize(pubfunc);
modbusGetQuantityRegsMax(pubfunc);
modbusIsFuncPub(par);
modbusIsFuncUser(par);
modbusIsFuncHasStartReg(par);
modbusIsFuncHasQuantityRegs(par);
modbusIsFuncException(func);

// Клиент
modbusCltReadHoldRegsReq(pdu{}, &pos, pduSize, regStartAddr, quantityRegs);
modbusCltReadInputRegsReq(pdu{}, &pos, pduSize, regStartAddr, quantityRegs);
modbusCltReadRegsGetReg(const pdu{}, start, pduSize, reg, &value);
modbusCltReadRegsGetRegs(const pdu{}, start, pduSize, reg, quantity, values[]);
modbusCltWriteSingleRegReq(pdu{}, &pos, pduSize, regStartAddr, value);
modbusCltWriteMultipleRegsReq(pdu{}, &pos, pduSize, regStartAddr, const values[], quantity);
modbusCltGetExceptFromResp(const pdu{}, start, pduSize, sentFunc, &excode);

// Сервер
modbusSrvInit(server[MODBUS_SERVER_DATA], const functions{}, funcCount);
modbusSrvIsSupportedFunc(const server[MODBUS_SERVER_DATA], par);
modbusSrvGetStartReg(const pdu{}, start, pduSize, &reg, &excode);
modbusSrvMakeException(pdu{}, pduStart, pduMaxSize, &pduActualSize, excode);
modbusSrvGetQuantityRegs(const pdu{}, start, pduSize, &quantity, &excode);
modbusSrvGetDiscrete(const pdu{}, start, pduSize, &value, &excode);
modbusSrvGetAnalog(const pdu{}, start, pduSize, &value, &excode);
modbusSrvReadDiResp(pdu{}, &pos, pduSize, const values{}, quantity);
modbusSrvReadRegsResp(pdu{}, &pos, pduSize, const values[], quantity);
modbusSrvReadNparse(const pdu{}, start, pduSize, srvStartReg, srvMaxRegs, &startReg, &quantity, &excode);
modbusSrvWrite1parse(const pdu{}, start, pduSize, srvStartReg, srvMaxRegs, &reg, &value, &excode);
modbusSrvWriteNcoilsParse(const pdu{}, start, pduSize, srvStartReg, srvMaxRegs, &startReg, &quantity, values{}, valuesSize, &excode);
modbusSrvWriteNregsParse(const pdu{}, start, pduSize, srvStartReg, srvMaxRegs, &startReg, &quantity, values[], valuesSize, &excode);

//} Конец заголовка библиотеки работы с ModBus =================================
//} ============================================================================
#endif // MODBUS_H