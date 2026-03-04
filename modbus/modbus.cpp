#if !defined MODBUS_LIB
#define MODBUS_LIB
//{ ============================================================================
//{ Библиотека работы с ModBus =================================================
//{ ============================================================================

#include "modbus.h"

#include "..\array\array.h"
#include "..\array\array.cpp"

//// Публичные функции

//! Получить код функции из PDU
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start стартовая позиция PDU в массиве
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[out] par требуемый параметр
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbusGetFunction(const pdu{}, start, pduSize, &par)
{
    return modbus_getPar(pdu, start, pduSize, MODBUS_PDU_FUNCCODE_POS, par);
}

//! Проверить код функции на соответствие стандарту
modbusIsFuncValid(par)
{
    return (par >= MODBUS_FUNCTION_CODE_MIN) && (par <= MODBUS_FUNCTION_CODE_MAX);
}

//! Проверить код исключения на соответствие стандарту
modbusIsExcodeValid(excode)
{
    return ((excode >= MODBUS_EXCODE_ILLEGAL_FUNCTION) && (excode <= MODBUS_EXCODE_SERVER_DEV_BUSY)) ||
            (excode == MODBUS_EXCODE_MEMORY_PARITY) || (excode == MODBUS_EXCODE_GATEWAY_PATH) ||
            (excode == MODBUS_EXCODE_GATEWAY_TARGET);
}

//! Получить минимальный размер PDU запроса клиента по коду публичной функции (0, если код функции зарезервирован или некорректен)
modbusGetReqMinSize(pubfunc)
{
    switch (pubfunc)
    {
        case 1, 2, 3, 4, 5, 6: return 5;
        case 7, 11, 12, 17: return 1;
        case 8, 24: return 3;
        case 15, 22: return 7;
        case 16: return 8;
        case 20: return 9;
        case 21: return 11;
        case 23: return 12;
        case 43: return 2;
    }
    return 0;
}

//! Получить минимальный размер PDU ответа клиенту по коду публичной функции (0, если код функции зарезервирован или некорректен)
modbusGetRespSize(pubfunc)
{
    switch (pubfunc)
    {
        case 5, 6, 15, 16: return 5;
        case 7: return 2;
        case 22: return 7;
    }
    return 0;
}

//! Получить максимальное количество запрашиваемых регистров для публичной функции MODBUS_PUBFUNC_QREGS_R (0, если код функции некорректен)
modbusGetQuantityRegsMax(pubfunc)
{
    switch (pubfunc)
    {
        case MODBUS_FUNC_READCOILS, MODBUS_FUNC_READDI: return MODBUS_FUNC_RBITS_QUANTITY_MAX;
        case MODBUS_FUNC_READHLDREGS, MODBUS_FUNC_READINPREGS: return MODBUS_FUNC_RREGS_QUANTITY_MAX;
        case MODBUS_FUNC_WRITEMULTICOILS: return MODBUS_FUNC_WBITS_QUANTITY_MAX;
        case MODBUS_FUNC_WRITEMULTIREGS: return MODBUS_FUNC_WREGS_QUANTITY_MAX;
    }
    return 0;
}

//! Проверить код функции на принадлежность к публичным
//! \param[in] par проверяемый параметр
modbusIsFuncPub(par)
{
    return ((par >= MODBUS_PUBFUNC_R1_CODE_MIN) && (par <= MODBUS_PUBFUNC_R1_CODE_MAX)) ||
            ((par >= MODBUS_PUBFUNC_R2_CODE_MIN) && (par <= MODBUS_PUBFUNC_R2_CODE_MAX)) ||
            ((par >= MODBUS_PUBFUNC_R3_CODE_MIN) && (par <= MODBUS_PUBFUNC_R3_CODE_MAX));
}

//! Проверить код функции на принадлежность к пользовательским
//! \param[in] par проверяемый параметр
modbusIsFuncUser(par)
{
    return ((par > MODBUS_PUBFUNC_R1_CODE_MAX) && (par < MODBUS_PUBFUNC_R2_CODE_MIN)) ||
            ((par > MODBUS_PUBFUNC_R2_CODE_MAX) && (par < MODBUS_PUBFUNC_R3_CODE_MIN));
}

//! Проверить код функции на наличие в ее параметрах одного стартового регистра
//! \param[in] par проверяемый параметр
modbusIsFuncHasStartReg(par)
{
    return ((par >= MODBUS_PUBFUNC_STARTREG_R1_MIN) && (par <= MODBUS_PUBFUNC_STARTREG_R1_MAX)) ||
            ((par >= MODBUS_PUBFUNC_STARTREG_R2_MIN) && (par <= MODBUS_PUBFUNC_STARTREG_R2_MAX));
}

//! Проверить код функции на наличие в ее параметрах количества регистров
//! \param[in] par проверяемый параметр
modbusIsFuncHasQuantityRegs(par)
{
    return ((par >= MODBUS_PUBFUNC_QREGS_R1_MIN) && (par <= MODBUS_PUBFUNC_QREGS_R1_MAX)) ||
            ((par >= MODBUS_PUBFUNC_QREGS_R2_MIN) && (par <= MODBUS_PUBFUNC_QREGS_R2_MAX));
}

modbusIsFuncException(func)
{
    return (func & 0xFF) >= (MODBUS_PUBFUNC_R1_CODE_MIN + MODBUS_FUNC_ERROR_OFFSET);
}

// Клиент

//! Сформировать запрос клиента на чтение регистров временного хранения
//! \param[out] pdu массив с блоком данных протокола
//! \param[inout] pos позиция для вставки в блок данных протокола
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] regStartAddr адрес стартового регистра (нумерация с 0)
//! \param[in] quantityRegs количество читаемых регистров
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbusCltReadHoldRegsReq(pdu{}, &pos, pduSize, regStartAddr, quantityRegs)
{
    return modbus_cltReadRegsReq(pdu, pos, pduSize, true, regStartAddr, quantityRegs);
}

//! Сформировать запрос клиента на чтение входных регистров
//! \param[out] pdu массив с блоком данных протокола
//! \param[inout] pos позиция для вставки в блок данных протокола
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] regStartAddr адрес стартового регистра (нумерация с 0)
//! \param[in] quantityRegs количество читаемых регистров
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbusCltReadInputRegsReq(pdu{}, &pos, pduSize, regStartAddr, quantityRegs)
{
    return modbus_cltReadRegsReq(pdu, pos, pduSize, false, regStartAddr, quantityRegs);
}

//! Получить значение одного регистра из принятого PDU от сервера 
//! функции чтения входных регистров и регистров временного хранения
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция начала блока данных протокола в массиве
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] reg адрес регистра в PDU (нумерация с 0)
//! \param[out] value значение регистра (0..65535) при успешном возврате
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusCltReadRegsGetReg(const pdu{}, start, pduSize, reg, &value)
{
    new size;
    if (!modbus_cltRespGetPayloadSize(pdu, start, pduSize, size))
        return false;

    if ((2 * (reg + 1) > size) || (reg < 0) || (start + 3 + reg * 2 >= pduSize) || (start < 0))
        return false;

    value = (pdu{start + 2 + reg * 2} << 8) + pdu{start + 3 + reg * 2};
    return true;
}

//! Получить значение нескольких регистров из принятого PDU от сервера 
//! функции чтения входных регистров и регистров временного хранения
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция начала блока данных протокола в массиве
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] reg адрес первого регистра в PDU (нумерация с 0)
//! \param[in] quantity количество регистров
//! \param[out] values значения регистров (каждое 0..65535) при возврате MODBUS_EXCODE_OK
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusCltReadRegsGetRegs(const pdu{}, start, pduSize, reg, quantity, values[])
{
    new size;
    if (!modbus_cltRespGetPayloadSize(pdu, start, pduSize, size))
        return false;

    if ((reg < 0) || (quantity <= 0) || (reg > size / 2) || (2 * quantity * (reg + 1) > size) || (start + 2 * (reg + quantity) + 1 >= pduSize))
        return false;

    for (new i = 0; i < quantity; i++)
        values[i] = (pdu{start + 2 * (reg + i) + 2} << 8) + pdu{start + 2 * (reg + i) + 3};
    
    return true;
}

//! Сформировать запрос клиента на запись одного регистра
//! \param[out] pdu массив с блоком данных протокола
//! \param[inout] pos позиция для вставки в блок данных протокола
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] regStartAddr адрес регистра (нумерация с 0)
//! \param[in] value устанавливаемое значение регистра
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusCltWriteSingleRegReq(pdu{}, &pos, pduSize, regStartAddr, value)
{
    if ((pos + 5 >= pduSize) || (pos < 0))
        return false;

    pdu{pos++} = MODBUS_FUNC_WRITESINGLEREG;
    pdu{pos++} = regStartAddr >> 8;
    pdu{pos++} = regStartAddr & 0xFF;
    pdu{pos++} = value >> 8;
    pdu{pos++} = value & 0xFF;
    return true;
}

//! Сформировать запрос клиента на запись нескольких регистров
//! \param[out] pdu массив с блоком данных протокола, размер не менее MODBUS_PDU_SIZE_MAX от pos
//! \param[inout] pos позиция для вставки в блок данных протокола, увеличится на 6 + 2 * quantity
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] regStartAddr адрес регистра (нумерация с 0)
//! \param[in] values устанавливаемые значения регистров
//! \param[in] quantity количество регистров
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusCltWriteMultipleRegsReq(pdu{}, &pos, pduSize, regStartAddr, const values[], quantity)
{
    if ((pos + 6 + quantity * 2 >= pduSize) || (pos < 0) || (quantity < 0) || (quantity > MODBUS_FUNC_WREGS_QUANTITY_MAX))
        return false;

    pdu{pos++} = MODBUS_FUNC_WRITEMULTIREGS;
    pdu{pos++} = regStartAddr >> 8;
    pdu{pos++} = regStartAddr & 0xFF;
    pdu{pos++} = quantity >> 8;
    pdu{pos++} = quantity & 0xFF;
    pdu{pos++} = quantity * 2;
    for (new i = 0; i < quantity; i++)
    {
        pdu{pos++} = values[i] >> 8;
        pdu{pos++} = values[i] & 0xFF;
    }
    return true;
}

//! Получить код ошибки из ответа сервера
//! \param[in] pdu массив с блоком данных протокола ответа сервера
//! \param[in] start стартовая позиция PDU в массиве
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] sentFunc код отправленной в запросе функции
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusCltGetExceptFromResp(const pdu{}, start, pduSize, sentFunc, &excode)
{
    new func;
    if (!modbusIsFuncValid(sentFunc) || !modbusGetFunction(pdu, start, pduSize, func))
        return false;

    if (func == (sentFunc + MODBUS_FUNCTION_CODE_ER_OFFSET))
    {    
        if (!modbus_getExcode(pdu, start, pduSize, excode))
            return false;
    }
    else
    {
        excode = MODBUS_EXCODE_OK;
    }
    return true;
}

// Сервер

//! Инициализировать сервер
//! \param[out] server объект сервера
//! \param[in] functions коды поддерживаемых публичных функций
//! \param[in] funcCount кол-во поддерживаемых публичных функций, не более MODBUS_PUBFUNCTIONS_MAX
//! \return MODBUS_EXCODE_OK - успешно; MODBUS_EXCODE_ILLEGAL_FUNCTION - неуспешно
modbusSrvInit(server[MODBUS_SERVER_DATA], const functions{}, funcCount)
{
    if (funcCount > MODBUS_FUNCTIONS_MAX)
        funcCount = MODBUS_FUNCTIONS_MAX;

    server.funcCount = 0;
    for (new i = 0; i < funcCount; i++)
    {
        if ((functions{i} >= MODBUS_FUNCTION_CODE_MIN) && (functions{i} <= MODBUS_FUNCTION_CODE_MAX))
            server.functions{server.funcCount++} = functions{i};
        else
            return MODBUS_EXCODE_ILLEGAL_FUNCTION;
    }
    return MODBUS_EXCODE_OK;
}

//! Проверить код функции на принадлежность к поддерживаемым
//! \param[in] par проверяемый параметр
modbusSrvIsSupportedFunc(const server[MODBUS_SERVER_DATA], par)
{
    return searchLinearStr(server.functions, server.funcCount, par) >= 0;
}

//! Сформировать код исключения на запрос клиента
//! \param[inout] pdu массив с блоком данных протокола
//! \param[in] pduStart позиция начала PDU
//! \param[in] pduMaxSize предельный размер массива с блоком данных протокола
//! \param[inout] pduActualSize актуальный размер массива с блоком данных протокола
//! \param[in] excode формируемый код исключения MODBUS_EXCODE_
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusSrvMakeException(pdu{}, pduStart, pduMaxSize, &pduActualSize, excode)
{
    new func;
    if (!modbusGetFunction(pdu, pduStart, pduActualSize, func))
        return false;

    if (!modbusIsExcodeValid(excode) || !modbusIsFuncValid(func) || ((pduMaxSize - pduStart) < MODBUS_EXCEPTION_PDU_SIZE) || (pduStart < 0))
        return false;

    pdu{pduStart + MODBUS_PDU_FUNCCODE_POS} += MODBUS_FUNC_ERROR_OFFSET;
    pdu{pduStart + MODBUS_PDU_EXCODE_POS} = excode;
    pduActualSize = pduStart + MODBUS_EXCEPTION_PDU_SIZE;
    return true;
}

//! Получить из PDU запроса клиента адрес стартового регистра для функций MODBUS_PUBFUNC_STARTREG_R
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция начала PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[out] reg адрес стартового регистра
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusSrvGetStartReg(const pdu{}, start, pduSize, &reg, &excode)
{
    new func;
    if (!modbusGetFunction(pdu, start, pduSize, func))
        return false;

    if (!modbusIsFuncHasStartReg(func))
    {
        excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
        return true;
    }
    excode = MODBUS_EXCODE_OK;
    return array2num16beUnSign(pdu, start + MODBUS_PDU_STARTREG_POS, pduSize, reg);
}

//! Получить из PDU запроса клиента количество регистров для функций MODBUS_PUBFUNC_QREGS_R
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция начала PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[out] quantity количество регистров
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusSrvGetQuantityRegs(const pdu{}, start, pduSize, &quantity, &excode)
{
    new func;
    if (!modbusGetFunction(pdu, start, pduSize, func))
        return false;

    if (!modbusIsFuncHasQuantityRegs(func))
    {
        excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
        return true;
    }
    excode = MODBUS_EXCODE_OK;
    return array2num16beUnSign(pdu, start + MODBUS_PDU_QREGS_POS, pduSize, quantity);
}

//! Получить из запроса клиента устанавливаемое состояние единичного дискрета
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция начала PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[out] value значение (0|1)
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusSrvGetDiscrete(const pdu{}, start, pduSize, &value, &excode)
{
    new func;
    if (!modbusGetFunction(pdu, start, pduSize, func))
        return false;

    if (func != MODBUS_FUNC_WRITESINGLECOIL)
    {
        excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
        return true;
    }
    new tmp;
    if (!array2num16beUnSign(pdu, start + MODBUS_PDU_OUT_VAL_POS, pduSize, tmp))
        return false;

    excode = ((tmp == MODBUS_STATE_ON) || (tmp == MODBUS_STATE_OFF)) ? MODBUS_EXCODE_OK : MODBUS_EXCODE_ILLEGAL_DATA_VAL;
    value = tmp != MODBUS_STATE_OFF;
    return true;
}

//! Получить из запроса клиента устанавливаемое состояние единичного аналога
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция начала PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[out] value значение
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusSrvGetAnalog(const pdu{}, start, pduSize, &value, &excode)
{
    new func;
    if (!modbusGetFunction(pdu, start, pduSize, func))
        return false;

    if (func != MODBUS_FUNC_WRITESINGLEREG)
    {
        excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
        return true;
    }
    excode = MODBUS_EXCODE_OK;
    return array2num16beUnSign(pdu, start + MODBUS_PDU_OUT_VAL_POS, pduSize, value); 
}

//! Сформировать ответ сервера на запрос чтения дискретов
//! \param[out] pdu массив с блоком данных протокола
//! \param[inout] pos позиция в массиве, соответствующая началу PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] values отправляемые дискретные значения: 0-й отправляемый бит - 0-й бит 0 байта, 1-й отпр. бит - 1-й бит 0 байта,...
//! \param[in] quantity количество значений
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbusSrvReadDiResp(pdu{}, &pos, pduSize, const values{}, quantity)
{
    new bytes = quantity / 8 + ((quantity % 8) != 0);
    if ((pduSize <= 0) || (pos < 0) || (quantity <= 0) || (pduSize - pos < bytes + 2))
        return false;

    pos++;
    pdu{pos++} = bytes;
    for (new i = 0; i < bytes; i++)
        pdu{pos++} = values{i};

    return true;
}

//! Сформировать ответ сервера на запрос чтения регистров
//! \param[out] pdu массив с блоком данных протокола
//! \param[inout] pos позиция в массиве, соответствующая началу PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] values отправляемые значения
//! \param[in] quantity количество значений
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbusSrvReadRegsResp(pdu{}, &pos, pduSize, const values[], quantity)
{
    if ((pduSize <= 0) || (pos < 0) || (quantity <= 0) || (pduSize - pos < quantity * 2 + 2))
        return false;

    pos++;
    pdu{pos++} = quantity * 2;
    for (new i = 0; i < quantity; i++, pos += 2)
        num16bit2arrayBe(values[i], pdu, pos, pduSize);

    return true;
}

//! Получить данные из запроса клиента на чтение нескольких регистров/дискретов
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция в массиве, соответствующая началу PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] srvStartReg стартовый адрес регистра области сервера
//! \param[in] srvMaxRegs количество регистров в данной области сервера
//! \param[out] startReg запрашиваемый стартовый адрес регистра
//! \param[out] quantity запрашиваемое количество
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusSrvReadNparse(const pdu{}, start, pduSize, srvStartReg, srvMaxRegs, &startReg, &quantity, &excode)
{
    new func;
    if (!modbusGetFunction(pdu, start, pduSize, func))
        return false;

    if (!modbusIsFuncHasQuantityRegs(func) || !modbusIsFuncHasStartReg(func))
    {    
        excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
        return true;
    }
    if (!modbusSrvGetQuantityRegs(pdu, start, pduSize, quantity, excode))
        return false;

    if (excode != MODBUS_EXCODE_OK)
        return true;

    if ((quantity < 1) || (quantity > modbusGetQuantityRegsMax(func)))
    {    
        excode = MODBUS_EXCODE_ILLEGAL_DATA_VAL;
        return true;
    }
    if (!modbusSrvGetStartReg(pdu, start, pduSize, startReg, excode))
        return false;

    if (excode != MODBUS_EXCODE_OK)
        return true;

    if ((startReg + quantity) > (srvStartReg + srvMaxRegs))
        excode = MODBUS_EXCODE_ILLEGAL_DATA_ADDR;

    return true;
}

//! Получить данные из запроса клиента на запись 1 катушки/регистра
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция в массиве, соответствующая началу PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] srvStartReg стартовый адрес регистра области сервера
//! \param[in] srvMaxRegs количество регистров в данной области сервера
//! \param[out] reg запрашиваемый адрес регистра
//! \param[out] value устанавливаемая величина
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (некорректные входные данные), true - успешно
modbusSrvWrite1parse(const pdu{}, start, pduSize, srvStartReg, srvMaxRegs, &reg, &value, &excode)
{
    new func;
    if (!modbusGetFunction(pdu, start, pduSize, func))
        return false;

    if ((func != MODBUS_FUNC_WRITESINGLECOIL) && (func != MODBUS_FUNC_WRITESINGLEREG))
    {
        excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
        return true;
    }
    new res;
    if (func == MODBUS_FUNC_WRITESINGLECOIL)
        res = modbusSrvGetDiscrete(pdu, start, pduSize, value, excode);
    else
        res = modbusSrvGetAnalog(pdu, start, pduSize, value, excode);
    
    if (!res)
        return false;

    if (excode != MODBUS_EXCODE_OK)
        return true;

    if (!modbusSrvGetStartReg(pdu, start, pduSize, reg, excode))
        return false;

    if (excode != MODBUS_EXCODE_OK)
        return true;

    if ((reg < srvStartReg) || (reg > (srvStartReg + srvMaxRegs)))
        excode = MODBUS_EXCODE_ILLEGAL_DATA_ADDR;

    return true;
}

//! Получить данные из запроса клиента на запись нескольких катушек (бит)
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция в массиве, соответствующая началу PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] srvStartReg стартовый адрес регистра области сервера
//! \param[in] srvMaxRegs количество регистров в данной области сервера
//! \param[out] startReg запрашиваемый стартовый адрес регистра
//! \param[out] quantity запрашиваемое количество бит
//! \param[out] values устанавливаемые битовые значения
//! \param[in] valuesSize размер values
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbusSrvWriteNcoilsParse(const pdu{}, start, pduSize, srvStartReg, srvMaxRegs, &startReg, &quantity, values{}, valuesSize, &excode)
{
    new func;
    if (!modbusGetFunction(pdu, start, pduSize, func) || ((pduSize - start) < modbusGetReqMinSize(MODBUS_FUNC_WRITEMULTICOILS)))
        return false;

    if (func != MODBUS_FUNC_WRITEMULTICOILS)
    {    
        excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
        return true;
    }
    if (!modbusSrvGetQuantityRegs(pdu, start, pduSize, quantity, excode))
        return false;

    if (excode != MODBUS_EXCODE_OK)
        return true;

    if ((quantity < 1) || (quantity > modbusGetQuantityRegsMax(func)))
    {    
        excode = MODBUS_EXCODE_ILLEGAL_DATA_VAL;
        return true;
    }
    if (!modbusSrvGetStartReg(pdu, start, pduSize, startReg, excode))
        return false;

    if (excode != MODBUS_EXCODE_OK)
        return true;

    new len = quantity / 8 + (quantity % 8 != 0);
    if (((startReg + quantity) > (srvStartReg + srvMaxRegs)) || (pdu{start + 5} < len))
    {    
        excode = MODBUS_EXCODE_ILLEGAL_DATA_ADDR;
        return true;
    }
    if ((len > valuesSize) || (start + MODBUS_PDU_OUTS_VAL_POS + len >= pduSize))
        return false;

    for (new i = 0; i < len; i++)
        values{i} = pdu{start + MODBUS_PDU_OUTS_VAL_POS + i};

    return true;
}

//! Получить данные из запроса клиента на запись нескольких регистров
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start позиция в массиве, соответствующая началу PDU
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] srvStartReg стартовый адрес регистра области сервера
//! \param[in] srvMaxRegs количество регистров в данной области сервера
//! \param[out] startReg запрашиваемый стартовый адрес регистра
//! \param[out] quantity запрашиваемое количество регистров
//! \param[out] values устанавливаемые значения регистров
//! \param[in] valuesSize размер values
//! \param[out] excode MODBUS_EXCODE_ при успешном возврате
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbusSrvWriteNregsParse(const pdu{}, start, pduSize, srvStartReg, srvMaxRegs, &startReg, &quantity, values[], valuesSize, &excode)
{
    new func;
    if (!modbusGetFunction(pdu, start, pduSize, func) || ((pduSize - start) < modbusGetReqMinSize(MODBUS_FUNC_WRITEMULTIREGS)))
        return false;

    if (func != MODBUS_FUNC_WRITEMULTIREGS)
    {    
        excode = MODBUS_EXCODE_ILLEGAL_FUNCTION;
        return true;
    }
    if (!modbusSrvGetQuantityRegs(pdu, start, pduSize, quantity, excode))
        return false;

    if (excode != MODBUS_EXCODE_OK)
        return true;

    if ((quantity < 1) || (quantity > modbusGetQuantityRegsMax(func)))
    {    
        excode = MODBUS_EXCODE_ILLEGAL_DATA_VAL;
        return true;
    }
    if (!modbusSrvGetStartReg(pdu, start, pduSize, startReg, excode))
        return false;

    if (excode != MODBUS_EXCODE_OK)
        return true;

    if (((startReg + quantity) > (srvStartReg + srvMaxRegs)) || (pdu{start + 5} < (quantity * 2)))
    {    
        excode = MODBUS_EXCODE_ILLEGAL_DATA_ADDR;
        return true;
    }
    if ((quantity > valuesSize) || ((start + MODBUS_PDU_OUTS_VAL_POS + (quantity * 2)) > pduSize))
        return false;

    for (new i = 0; i < quantity; i++)
    {    
        new num;
        array2num16beUnSign(pdu, start + MODBUS_PDU_OUTS_VAL_POS + 2*i, pduSize, num);
        values[i] = num;
    }
    return true;
}

//// Приватные функции

//! Получить параметр из PDU
//! \param[in] pdu массив с блоком данных протокола
//! \param[in] start стартовая позиция PDU в массиве
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] pos позиция требуемого параметра
//! \param[out] par требуемый параметр при успешном возврате
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbus_getPar(const pdu{}, start, pduSize, pos, &par)
{
    if (start < 0)
        start = 0;
    
    if (pos < 0)
        pos = 0;

    if (start + pos >= pduSize)
        return false;

    par = pdu{start + pos};
    return true;
}

//! Получить код исключения из PDU ответа сервера
//! \param[in] pdu массив с блоком данных протокола ответа сервера
//! \param[in] start стартовая позиция PDU в массиве
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[out] par требуемый параметр
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbus_getExcode(const pdu{}, start, pduSize, &par)
{
    return modbus_getPar(pdu, start, pduSize, MODBUS_PDU_EXCODE_POS, par);
}

// Клиент

//! Сформировать запрос клиента на чтение регистров
//! \param[out] pdu массив с блоком данных протокола
//! \param[inout] pos позиция для вставки в блок данных протокола
//! \param[in] pduSize размер массива с блоком данных протокола
//! \param[in] func признак использования функции MODBUS_FUNC_READHLDREGS: 0 - MODBUS_FUNC_READINPREGS, !=0 - MODBUS_FUNC_READHLDREGS
//! \param[in] regStartAddr адрес стартового регистра (нумерация с 0)
//! \param[in] quantity количество читаемых регистров
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbus_cltReadRegsReq(pdu{}, &pos, pduSize, hld, regStartAddr, quantity)
{
    if ((pos + 5 >= pduSize) || (pos < 0) || (quantity < 0) || (quantity > MODBUS_FUNC_RREGS_QUANTITY_MAX))
        return false;

    pdu{pos++} = hld ? MODBUS_FUNC_READHLDREGS : MODBUS_FUNC_READINPREGS;
    pdu{pos++} = regStartAddr >> 8;
    pdu{pos++} = regStartAddr & 0xFF;
    pdu{pos++} = quantity >> 8;
    pdu{pos++} = quantity & 0xFF;
    return true;
}

//! Получить из ответного PDU длину полезных данных
//! \param[in] pdu массив с блоком данных протокола, размер не менее 2 от start
//! \param[in] start позиция начала блока данных протокола в массиве
//! \param[out] par требуемый параметр при успешном возврате
//! \return false - неуспешно (ошибка входных данных), true - успешно
modbus_cltRespGetPayloadSize(const pdu{}, start, pduSize, &par)
{
    return modbus_getPar(pdu, start, pduSize, 1, par);
}

// Сервер

//} Конец библиотеки работы с ModBus ===========================================
//} ============================================================================
#endif // MODBUS_LIB