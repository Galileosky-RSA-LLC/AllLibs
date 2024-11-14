#ifndef MODBUSSERIAL_LIB
#define MODBUSSERIAL_LIB
//{ ============================================================================
//{ Библиотека работы с ModBus по последовательной линии =======================
//{ ============================================================================

#include "modbusserial.h"
#include "modbus.h"
#include "modbus.cpp"
#include "..\time\time.h"
#include "..\time\time.cpp"
#include "..\diag\diag.h"
#include "..\diag\diag.cpp"

// Публичные функции

//! Задать настройку межкадрового интервала в режиме RTU
//! \param[out] obj объект приемопередатчика
//! \param[in] par устанавливаемый параметр
//! \return false - неуспешно (настройка неверная, будет использован расчет по стандарту), true - успешно
modbusSerialSetInterFrameDelay(obj[MODBUSSERIAL_TRANSCEIVER_DATA], par)
{
    obj.setInterFrameDelay = par;
    return modbusSerial_setFrameDelays(obj);
}

//! Инициализировать порт для связи
//! \param[out] obj объект приемопередатчика
//! \param[in] portNum номер порта
//! \param[in] baudRate скорость передачи данных, бит/с
//! \param[in] stopBits стоповые биты порта: 0 - 1 бит, 1 - 2 бита
//! \param[in] parity четность
//! \return true - успешная инициализация, false - выполнена попытка инициализации или ошибка инициализации
modbusSerialPortInit(obj[MODBUSSERIAL_TRANSCEIVER_DATA], portNum, baudRate, stopBits, parity)
{
    obj.portNum = portNum;
    if (parity < 0)
        parity = 0;

    if (parity > 2)
        parity = 2;

    if (baudRate <= 0)
    {    
        obj.interFrameDelay = 10;
        obj.interCharDelay = 5;
    }
    else if (!modbusSerial_setFrameDelays(obj))
    {    
        new bits = 10 + (stopBits != 0) + (parity != 0);
        obj.interFrameDelay = (baudRate <= 19200) ? (3500 * bits / baudRate + 1) : 2; // 3.5 символа
        obj.interCharDelay = (baudRate <= 19200) ? (1500 * bits / baudRate + 1) : 1; // 1.5 символа
    }
    obj.inited = false;
    return PortInit(portNum, baudRate, MODBUSSERIAL_PORT_BUFFER_SIZE, stopBits != 0, parity, 0);
}

//! Получить код функции из буфера приемопередатчика
//! \param[in] obj объект приемопередатчика
//! \param[out] func код функции при успешном возврате
//! \return MODBUSSERIAL_ERROR_INPUT - неуспешно, MODBUSSERIAL_OK - успешно
modbusSerialGetFunction(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], &func)
{
    if (!modbusGetFunction(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, min(obj.bufSize, MODBUSSERIAL_BUF_SIZE_MAX), func))
        return MODBUSSERIAL_ERROR_INPUT;

    return MODBUSSERIAL_OK;
}

//! Получить PDU из буфера приемопередатчика после успешного приема данных или приема кода ошибки
//! \return кол-во вставленных байт
modbusSerialGetPdu(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], data{}, dataSize, dataPos = 0)
{
    return insertArrayStr(data, dataPos, dataSize, obj.buf, min(obj.bufSize, MODBUSSERIAL_BUF_SIZE_MAX), MODBUSSERIAL_UF_PDU_OFFSET);
}

//! Инициализировать клиента
//! \param[out] client объект клиента
//! \param[in] turnaroundDelay задержка после широковещательной рассылки, мс
modbusSerialCltInit(client[MODBUSSERIAL_CLIENT_DATA], turnaroundDelay)
{
    client.turnaroundDelay = (turnaroundDelay > MODBUSSERIAL_TURNAROUND_DELAY_MIN) ? turnaroundDelay : MODBUSSERIAL_TURNAROUND_DELAY_MIN;
}

//! Получить значение регистра из сообщения чтения регистров
//! Предварительно должны быть получены данные от другого устройства
//! \param[in] obj объект приемопередатчика
//! \param[in] reg адрес регистра в принятом сообщении (нумерация с 0)
//! \param[out] value значение регистра (0..65535) при успешном возврате
//! \return MODBUSSERIAL_ERROR_INPUT - неуспешно, MODBUSSERIAL_OK - успешно
modbusSerialCltReadRegsGetReg(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], reg, &value)
{
    return modbusCltReadRegsGetReg(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, MODBUSSERIAL_BUF_SIZE_MAX, reg, value)
            ? MODBUSSERIAL_OK
            : MODBUSSERIAL_ERROR_INPUT;
}

//! Получить значение нескольких регистров из сообщения чтения регистров
//! Предварительно должны быть получены данные от другого устройства
//! \param[in] obj объект приемопередатчика
//! \param[in] reg адрес начального регистра в принятом сообщении (нумерация с 0)
//! \param[in] quantity кол-во требуемых регистров
//! \param[out] values значения регистров (все 0..65535) при успешном возврате
//! \return MODBUSSERIAL_ERROR_INPUT - неуспешно, MODBUSSERIAL_OK - успешно
modbusSerialCltReadRegsGetRegs(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], reg, quantity, values[])
{
    return modbusCltReadRegsGetRegs(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, MODBUSSERIAL_BUF_SIZE_MAX, reg, quantity, values)
            ? MODBUSSERIAL_OK
            : MODBUSSERIAL_ERROR_INPUT;
}

//! Прочитать регистры временного хранения сервера. 
//! Предварительно должен быть инициализирован порт с помощью modbusSerialPortInit()
//! \param[inout] obj объект приемопередатчика
//! \param[in] client объект клиента
//! \param[in] setsrv объект настроек сервера
//! \param[in] regStartAddr адрес стартового регистра (нумерация с 0)
//! \param[in] quantityRegs количество читаемых регистров
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerialCltReadHoldRegs(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], regStartAddr,
                             quantityRegs, &excode, diag)
{
    if (diag)
        Diagnostics("mbsCrHr");

    return modbusSerial_cltReadRegs(obj, client, setsrv, MODBUS_FUNC_READHLDREGS, regStartAddr, quantityRegs, excode, diag);
}

//! Прочитать входные регистры сервера. 
//! Предварительно должен быть инициализирован порт с помощью modbusSerialPortInit()
//! \param[inout] obj объект приемопередатчика
//! \param[in] client объект клиента
//! \param[in] setsrv объект настроек сервера
//! \param[in] regStartAddr адрес стартового регистра (нумерация с 0)
//! \param[in] quantityRegs количество читаемых регистров
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerialCltReadInputRegs(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], regStartAddr,
                             quantityRegs, &excode, diag)
{
    if (diag)
        Diagnostics("mbsCrIr");

    return modbusSerial_cltReadRegs(obj, client, setsrv, MODBUS_FUNC_READINPREGS, regStartAddr, quantityRegs, excode, diag);
}

//! Записать в один регистр сервера. 
//! Предварительно должен быть инициализированы порт с помощью modbusSerialPortInit и клиент с помощью modbusSerialCltInit
//! \param[inout] obj объект приемопередатчика
//! \param[in] client объект клиента
//! \param[in] setsrv объект настроек сервера
//! \param[in] reg адрес регистра (нумерация с 0)
//! \param[in] value устанавливаемое значение регистра
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerialCltWrite1Reg(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], reg, value,
                                 &excode, diag)
{
    if (diag)
        Diagnostics("mbsCw1r");

    obj.bufSize = 0;
    obj.buf{obj.bufSize++} = setsrv.addr & 0xFF;
    if (!modbusCltWriteSingleRegReq(obj.buf, obj.bufSize, MODBUSSERIAL_BUF_SIZE_MAX, reg, value))
        return MODBUSSERIAL_ERROR_INPUT;

    new res = modbusSerial_sendData(obj, diag);
    if (res != MODBUSSERIAL_OK)
        return res;

    return modbusSerial_getResponseAndWait(obj, client, setsrv, excode, diag);
}

//! Записать в несколько регистров сервера. 
//! Предварительно должен быть инициализирован порт с помощью modbusSerialPortInit
//! \param[inout] obj объект приемопередатчика
//! \param[in] client объект клиента
//! \param[in] setsrv объект настроек сервера
//! \param[in] reg адрес стартового регистра (нумерация с 0)
//! \param[in] values устанавливаемые значения регистров
//! \param[in] quantity количество регистров
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerialCltWriteNregs(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], reg, 
                                const values[], quantity, &excode, diag)
{
    if (diag)
        Diagnostics("mbsCwNr");

    obj.bufSize = 0;
    obj.buf{obj.bufSize++} = setsrv.addr & 0xFF;
    if (!modbusCltWriteMultipleRegsReq(obj.buf, obj.bufSize, MODBUSSERIAL_BUF_SIZE_MAX, reg, values, quantity))
        return MODBUSSERIAL_ERROR_INPUT;

    new res = modbusSerial_sendData(obj, diag);
    if (res != MODBUSSERIAL_OK)
        return res;

    return modbusSerial_getResponseAndWait(obj, client, setsrv, excode, diag);
}

//! Отправить свободный запрос. 
//! Предварительно должен быть инициализирован порт с помощью modbusSerialPortInit(). 
//! Ответ будет в буфере приемопередатчика
//! \param[inout] obj объект приемопередатчика
//! \param[in] client объект клиента
//! \param[in] setsrv объект настроек сервера
//! \param[in] data массив с PDU
//! \param[in] dataPos начало PDU
//! \param[in] dataSize размер массива с PDU
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerialCltCustomReq(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], 
                            const data{}, dataPos, dataSize, &excode, diag)
{
    if (diag)
        Diagnostics("mbsCcr");

    if (dataPos < 0)
        dataPos = 0;

    new pduSize = dataSize - dataPos;
    if (!modbusIsFuncValid(data{dataPos}) || (pduSize < MODBUS_PDU_SIZE_MIN) || (pduSize > MODBUS_PDU_SIZE_MAX))
        return MODBUSSERIAL_ERROR_INPUT;

    obj.buf{MODBUSSERIAL_UF_ADDR_POS} = setsrv.addr;
    obj.bufSize = 1 + insertArrayStr(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, MODBUSSERIAL_BUF_SIZE_MAX, data, dataSize, dataPos);
    new res = modbusSerial_sendData(obj, diag);
    if (res != MODBUSSERIAL_OK)
        return res;

    return modbusSerial_getResponseAndWait(obj, client, setsrv, excode, diag);
}                                

//! Основной обработчик сервера. 
//! Принимает запросы от клиента, при запросе неподдерживаемой функции отправляет ответ с кодом ошибки. 
//! Предварительно должен быть инициализированы порт с помощью modbusSerialPortInit()
//! \param[inout] obj объект приемопередатчика
//! \param[in] setsrv объект настроек сервера
//! \param[in] server объект сервера
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerialSrvHandler(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], const server[MODBUS_SERVER_DATA], &excode, diag)
{
    if (diag)
        Diagnostics("mbsSh");

    new res = modbusSerial_srvGetReq(obj, setsrv, server, excode, diag);
    if (res == MODBUSSERIAL_ERROR_EXCODE)
        modbusSerialSrvSendException(obj, excode, diag);

    return res;
}

//! Отправить код исключения клиенту в ответ на его запрос. 
//! Предварительно должен быть инициализирован порт с помощью modbusSerialPortInit()
//! \param[inout] obj объект приемопередатчика
//! \param[in] excode код исключения
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerialSrvSendException(obj[MODBUSSERIAL_TRANSCEIVER_DATA], excode, diag)
{
    if (diag)
        Diagnostics("mbsSsE");

    if (!modbusSrvMakeException(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, MODBUSSERIAL_BUF_SIZE_MAX, obj.bufSize, excode))
        return MODBUSSERIAL_ERROR_INPUT;

    return modbusSerial_sendData(obj, diag);
}

//! Проверить адрес сервера на принадлежность к допустимому диапазону
//! \param[in] setsrv объект настроек сервера
modbusSerialIsSrvAddrValid(const setsrv[MODBUSSERIAL_SERVER_DATA])
{
    return (setsrv.addr >= MODBUSSERIAL_SRV_ADDRESS_MIN) && (setsrv.addr <= MODBUSSERIAL_SRV_ADDRESS_MAX);
}

//! Дополнить буфер проверочным кодом и отправить ответ на запрос клиента
//! \param[inout] obj объект приемопередатчика
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_ERROR_ - неуспешно, MODBUSSERIAL_OK - успешно
modbusSerialSrvSendResponse(obj[MODBUSSERIAL_TRANSCEIVER_DATA], diag)
{
    if (diag)
        Diagnostics("mbsSsR");

    return modbusSerial_sendData(obj, diag);
}

modbusSerialGetAddr(const obj[MODBUSSERIAL_TRANSCEIVER_DATA], &addr)
{
    addr = obj.buf{MODBUSSERIAL_UF_ADDR_POS};
    return obj.bufSize > MODBUSSERIAL_UF_ADDR_POS ? MODBUSSERIAL_OK : MODBUSSERIAL_ERROR_INPUT;
}

// Приватные функции

//! Рассчитать контрольную сумму буфера в режиме RTU
//! Приватная
//! \param[in] buf буфер для расчета
//! \param[in] bufSize размер данных буфера, принимаемых в расчет
modbusSerial_rtuCalcCc(const buf{}, bufSize)
{
    return CRC16(buf, bufSize, MODBUSSERIAL_RTU_CRC_INIT);
}

//! Добавить к буферу контрольную сумму в режиме RTU
//! Приватная
//! \param[inout] obj объект приемопередатчика
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_INPUT при недостатке размера буфера
modbusSerial_rtuAddCc(obj[MODBUSSERIAL_TRANSCEIVER_DATA])
{
    if (obj.bufSize + 2 > MODBUSSERIAL_BUF_SIZE_MAX)
        return MODBUSSERIAL_ERROR_INPUT;

    new crc = modbusSerial_rtuCalcCc(obj.buf, obj.bufSize);
    obj.buf{obj.bufSize++} = crc & 0xFF;
    obj.buf{obj.bufSize++} = crc >> 8;
    return MODBUSSERIAL_OK;
}

//! Проверить принятый кадр в режиме RTU
//! Приватная
//! \param[in] obj объект приемопередатчика
modbusSerial_isRtuFrameValid(const obj[MODBUSSERIAL_TRANSCEIVER_DATA])
{
    if (obj.bufSize < MODBUSSERIAL_RTU_FRAME_SIZE_MIN)
        return false;

    return modbusSerial_rtuCalcCc(obj.buf, obj.bufSize - 2) == (obj.buf{obj.bufSize - 1} << 8) + obj.buf{obj.bufSize - 2};
}

//! Получить данные в буфер
//! Приватная
//! \param[inout] obj объект приемопередатчика
//! \param[in] setsrv объект настроек сервера
//! \return MODBUSSERIAL_OK - успешно (получено > 0 байт данных), MODBUSSERIAL_ERROR_TIMEOUT при отсутствии данных
modbusSerial_getData(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], diag)
{
    #ifdef MODBUSSERIAL_DEBUG
    if (diag)
        Diagnostics("mbs_gD");
    #endif
    for (obj.bufSize = 0; obj.bufSize < MODBUSSERIAL_BUF_SIZE_MAX; obj.bufSize++)
    {
        new c;
        if (PortRead(obj.portNum, c, obj.bufSize == 0 ? (setsrv.timeout > 0 ? setsrv.timeout : obj.interFrameDelay) : obj.interCharDelay))
            obj.buf{obj.bufSize} = c;
        else
            break;
    }
    if (obj.bufSize == 0)
        return MODBUSSERIAL_ERROR_TIMEOUT;

    obj.receiveUptime = GetVar(UPTIME);
    return MODBUSSERIAL_OK;
}

//! Отобразить принятый буфер в диагностике 
//! Приватная
//! \param[in] obj объект приемопередатчика
modbusSerial_diagRcvBuf(const obj[MODBUSSERIAL_TRANSCEIVER_DATA])
{
    diagAr(obj.buf, obj.bufSize, "iFrm", false);
}

//! Отобразить отправляемый буфер в диагностике 
//! Приватная
//! \param[in] obj объект приемопередатчика
modbusSerial_diagSndBuf(const obj[MODBUSSERIAL_TRANSCEIVER_DATA])
{
    diagAr(obj.buf, obj.bufSize, "oFrm", false);
}

//! Установить межкадровый и межсимвольный интервалы на основе настройки
//! Приватная
//! \param[inout] obj объект приемопередатчика
//! \return false - не успешно (настройка неверная, будет использован расчет по стандарту), true - успешно
modbusSerial_setFrameDelays(obj[MODBUSSERIAL_TRANSCEIVER_DATA])
{
    if (obj.setInterFrameDelay <= 2)
        return false;

    obj.interFrameDelay = obj.setInterFrameDelay;
    obj.interCharDelay = 3 * obj.interFrameDelay / 7;
    return true;
}

//! Прочитать регистры сервера. 
//! Предварительно должен быть инициализирован порт с помощью modbusSerialPortInit()
//! Приватная
//! \param[inout] obj объект приемопередатчика
//! \param[in] client объект настроек клиента
//! \param[in] setsrv объект настроек сервера
//! \param[in] holding признак использования функции MODBUS_FUNC_READHLDREGS (иначе используется MODBUS_FUNC_READINPREGS)
//! \param[in] regStartAddr адрес стартового регистра (нумерация с 0)
//! \param[in] quantityRegs количество читаемых регистров
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerial_cltReadRegs(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], holding,
                         regStartAddr, quantityRegs, &excode, diag)
{
    #ifdef MODBUSSERIAL_DEBUG
    if (diag)
        Diagnostics("mbs_CrR");
    #endif
    obj.bufSize = 0;
    obj.buf{obj.bufSize++} = setsrv.addr & 0xFF;
    new res;
    if (holding)
        res = modbusCltReadHoldRegsReq(obj.buf, obj.bufSize, MODBUSSERIAL_BUF_SIZE_MAX, regStartAddr, quantityRegs);
    else
        res = modbusCltReadInputRegsReq(obj.buf, obj.bufSize, MODBUSSERIAL_BUF_SIZE_MAX, regStartAddr, quantityRegs);

    if (!res)
        return MODBUSSERIAL_ERROR_INPUT;

    res = modbusSerial_sendData(obj, diag);
    if (res != MODBUSSERIAL_OK)
        return res;

    return modbusSerial_getResponseAndWait(obj, client, setsrv, excode, diag);
}

//! Получить ответ сервера на отправленный запрос. 
//! Приватная
//! \param[inout] obj объект приемопередатчика
//! \param[in] setsrv объект настроек сервера
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerial_getResponse(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], &excode, diag)
{
    #ifdef MODBUSSERIAL_DEBUG
    if (diag)
        Diagnostics("mbs_gR");
    #endif
    new sentFunc;
    if (!modbusGetFunction(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, min(obj.bufSize, MODBUSSERIAL_BUF_SIZE_MAX), sentFunc))
    {
        #ifdef MODBUSSERIAL_DEBUG
        if (diag)
            Diagnostics("rqS=%d", obj.bufSize);
        #endif
        return MODBUSSERIAL_ERROR_INPUT;
    }
    new sentAddr = obj.buf{MODBUSSERIAL_UF_ADDR_POS};
    new res = modbusSerial_getData(obj, setsrv, diag);
    if (res != MODBUSSERIAL_OK)
        return res;

    if (!modbusSerial_isRtuFrameValid(obj))
        return MODBUSSERIAL_ERROR_RECEIVE;

    new func;
    modbusGetFunction(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, MODBUSSERIAL_BUF_SIZE_MAX, func);
    new exceptReceived = false;
    if (modbusIsFuncException(func))
    {
        if (modbusCltGetExceptFromResp(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, MODBUSSERIAL_BUF_SIZE_MAX, sentFunc, excode))
            exceptReceived = excode != MODBUS_EXCODE_OK;
        else
            res = MODBUSSERIAL_ERROR_INPUT;
    }
    if (res == MODBUSSERIAL_OK)
    {
        if (((sentAddr != obj.buf{MODBUSSERIAL_UF_ADDR_POS}) && (sentAddr != MODBUSSERIAL_ADDRESS_BROADCAST))
            || ((sentFunc != func) && !exceptReceived))
            res = MODBUSSERIAL_ERROR_RECEIVE;
        else if (exceptReceived)
            res = MODBUSSERIAL_ERROR_EXCODE;    
    }
    #ifdef MODBUSSERIAL_DEBUG
    if ((res != MODBUSSERIAL_OK) && (obj.bufSize > 0) && diag)
        modbusSerial_diagRcvBuf(obj);
    #endif
    if ((res == MODBUSSERIAL_OK) || (res == MODBUSSERIAL_ERROR_EXCODE))
        obj.bufSize -= MODBUSSERIAL_RTU_CC_SIZE;

    return res;
}

//! Получить ответ сервера и выждать широковещательную задержку (если запрос широковещательный). 
//! Предварительно должен быть отправлен запрос. 
//! Приватная
//! \param[inout] obj объект приемопередатчика
//! \param[in] client объект клиента
//! \param[in] setsrv объект настроек сервера
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке
modbusSerial_getResponseAndWait(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const client[MODBUSSERIAL_CLIENT_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA],
                                 &excode, diag)
{
    new res = modbusSerial_getResponse(obj, setsrv, excode, diag);
    if ((setsrv.addr & 0xFF) == MODBUSSERIAL_ADDRESS_BROADCAST)
    {    
        if (res == MODBUSSERIAL_OK)
            waitFrom(obj.receiveUptime, client.turnaroundDelay);
        else
            wait(client.turnaroundDelay);
    }
    return res;
}

//! Получить запрос от клиента. 
//! Предварительно должен быть инициализированы порт с помощью modbusSerialPortInit() 
//! Приватная
//! \param[inout] obj объект приемопередатчика
//! \param[in] setsrv объект настроек сервера
//! \param[in] server объект сервера
//! \param[out] excode код исключения при возврате MODBUSSERIAL_ERROR_EXCODE
//! \param[in] diag признак необходимости расширенной диагностики
//! \return MODBUSSERIAL_OK - успешно, MODBUSSERIAL_ERROR_ при ошибке, MODBUSSERIAL_ANOTHER_DEV_DATA - получены данные другого адресата
modbusSerial_srvGetReq(obj[MODBUSSERIAL_TRANSCEIVER_DATA], const setsrv[MODBUSSERIAL_SERVER_DATA], const server[MODBUS_SERVER_DATA], &excode, diag)
{
    #ifdef MODBUSSERIAL_DEBUG
    if (diag)
        Diagnostics("mbs_sGr");
    #endif
    if (!obj.inited)
    {
        new c;
        while (PortRead(obj.portNum, c, obj.interCharDelay))
        {} // сначала пропускаем кадр, если уже был поток на входе
        obj.inited = true;
    }
    new res = modbusSerial_getData(obj, setsrv, diag);
    if (res != MODBUSSERIAL_OK)
        return res;

    if (modbusSerial_isRtuFrameValid(obj))
    {
        if ((obj.buf{MODBUSSERIAL_RTU_ADDR_POS} == setsrv.addr) || (obj.buf{MODBUSSERIAL_RTU_ADDR_POS} == MODBUSSERIAL_ADDRESS_BROADCAST))
        {    
            new func;
            modbusGetFunction(obj.buf, MODBUSSERIAL_UF_PDU_OFFSET, MODBUSSERIAL_BUF_SIZE_MAX, func);
            res = modbusSrvIsSupportedFunc(server, func) ? MODBUSSERIAL_OK : MODBUSSERIAL_ERROR_EXCODE;
            excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
            obj.bufSize -= MODBUSSERIAL_RTU_CC_SIZE;
        }                                                                                                            
        else
        {
            res = MODBUSSERIAL_ANOTHER_DEV_DATA;
        }
    }
    else
    {
        res = MODBUSSERIAL_ERROR_RECEIVE;
    }
    #ifdef MODBUSSERIAL_DEBUG
    if ((obj.bufSize > 0) && diag)
        modbusSerial_diagRcvBuf(obj);
    #endif
    return res;
}

//! Дополнить буфер проверочным кодом и отправить
//! \param[inout] obj объект приемопередатчика
//! \return MODBUSSERIAL_ERROR_ - неуспешно, MODBUSSERIAL_OK - успешно
modbusSerial_sendData(obj[MODBUSSERIAL_TRANSCEIVER_DATA], diag)
{
    if (diag)
        Diagnostics("mbs_sD");

    new res = modbusSerial_rtuAddCc(obj);
    if (res != MODBUSSERIAL_OK)
        return res;

    #ifdef MODBUSSERIAL_DEBUG
    if (diag)
        modbusSerial_diagSndBuf(obj);
    #endif
    waitFrom(obj.receiveUptime, obj.interFrameDelay);
    PortWrite(obj.portNum, obj.buf, obj.bufSize);
    return MODBUSSERIAL_OK;
}

//!} Конец библиотеки работы с ModBus по последовательной линии ================
//!} ===========================================================================

#endif // MODBUSSERIAL_LIB