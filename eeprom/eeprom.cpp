#ifndef EEPROM_LIB
#define EEPROM_LIB
//{ ============================================================================
//{ Библиотека работы с EEPROM =================================================
//{ ============================================================================

#include "eeprom.h"

// Публичные функции

eepromInitParamValues(obj[EEPROM_DATA], const addr[], addrSize)
{
	for (new i = 0; (i < addrSize) && (i < EEPROM_PARAMS_COUNT); i++)
    {    
        obj.keysVarAddr[i] = addr[i];
        obj.keysOldValue[i] = GetVar(obj.keysVarAddr[i]);
    }
}

//! Сохранить параметры по их изменению
eepromStoreParamsByChange(obj[EEPROM_DATA])
{
	new id;
    new needWrite = false;
    for (new i = 0; i < EEPROM_KEYS_COUNT; i++)
	{
		for (new j = 0; (j < EEPROM_PARAMS_PER_KEY) && (id < EEPROM_PARAMS_COUNT); j++, id++)
        {    
            if (GetVar(obj.keysVarAddr[id]) != obj.keysOldValue[id])
            {
                obj.keysOldValue[id] = GetVar(obj.keysVarAddr[id]);
                if (!needWrite)
                    needWrite = true;
            }
        }
        if (needWrite)
        {    
            if (!eeprom_storeParams(obj, i))
            {    
                #ifdef EEPROM_DEBUG
                Diagnostics("w.p?k=%d", i);
                #endif
            }
            needWrite = false;
        }
	}
}

//! Загрузить сохраненные параметры
eepromRestoreParams(obj[EEPROM_DATA])
{
    new id;
    for (new i = 0; i < EEPROM_KEYS_COUNT; i++)
	{
		new res = ROMRead(1 + i, obj.keyBuf, EEPROM_KEY_BUFFER_MAX_SIZE);
        for (new j = 0; (j < EEPROM_PARAMS_PER_KEY) && (id < EEPROM_PARAMS_COUNT); j++, id++)
        {
            if (res != 0)
                SetVar(obj.keysVarAddr[id], obj.keyBuf[j]);

            obj.keysOldValue[id] = (res != 0) ? obj.keyBuf[j] : GetVar(obj.keysVarAddr[id]);
            #ifdef EEPROM_DEBUG
            Diagnostics((res != 0) ? "r.p[%d]=%d" : "r.p[%d]?def=%d", id, (res != 0) ? obj.keyBuf[j] : obj.keysOldValue[id]);
            #endif
        }
	}
}

/*
// Шаблон использования

#define DEBUG // режим отладки

#ifdef DEBUG
#define EEPROM_DEBUG
#endif

#define EEPROM_PARAMS_COUNT 5 // указать требуемое количество сохраняемых параметров
#include "eeprom.h"
#include "eeprom.cpp"
#include "devinfo.h"
#include "devinfo.cpp"

#define CONF_CMD_WAIT_MS 5000 // время ожидания конфигурационных команд при отсутствии доступа к ROM
#define CHANGE_PARAMS_DELAY_MS 500 // время ожидания изменения параметров

mainT()
{
    #ifdef DEBUG
    SetVar(gDiag, 1); // устанавливаем расширенную диагностику
    #endif
    new devModel = getModel();
    new softMaj;
    new softMin;
    getSoftVersion(softMaj, softMin);
    #ifdef DEBUG
    Delay(20000); // чтобы успеть после перезагрузки увидеть начальную диагностику
    #endif
    if (!isRomAvailable(devModel, softMaj, softMin))
    {
        #ifdef DEBUG
        Diagnostics("ROM-");
        #endif
        #ifndef DEBUG
        Delay(CONF_CMD_WAIT_MS); // т.к. нет поддержки сохранения параметров в ПЗУ, даем время на прием настроечных конфигурационных команд
        #endif
        SetVar(gParamsInited, 1);
        return;
    }
    new eeprom[EEPROM_DATA];
    new varAddr[EEPROM_PARAMS_COUNT] = [
        GetVarAddr(gPortNum),
        GetVarAddr(gPortBaudRate),
        GetVarAddr(gPortStopBits),
        GetVarAddr(gPortParity),
        GetVarAddr(gTimeout)
        ];
    eepromInitParamValues(eeprom, varAddr, sizeof varAddr);
    eepromRestoreParams(eeprom);
    SetVar(gParamsInited, 1);
    while (true)
    {
        eepromStoreParamsByChange(eeprom);
        Delay(CHANGE_PARAMS_DELAY_MS);
    }
}

// Конец шаблона использования */


// Приватные функции

//! Сохранить параметры группы по ключу, в котором они хранятся
//! \param[in] key идентификатор ключа
//! \return !=0 - успешно, ==0 - ошибка
eeprom_storeParams(obj[EEPROM_DATA], key)
{
	if ((key < 0) || (key >= EEPROM_KEYS_COUNT))
		return false;

    new i;
    new paramCount = min(EEPROM_PARAMS_PER_KEY, (key < EEPROM_KEY_LAST)
                                                ? EEPROM_PARAMS_PER_KEY
                                                : (EEPROM_PARAMS_COUNT - EEPROM_KEY_LAST*EEPROM_PARAMS_PER_KEY));
    for (i = 0; i < paramCount; i++)
		obj.keyBuf[i] = GetVar(obj.keysVarAddr[key*EEPROM_PARAMS_PER_KEY + i]);

	return ROMWrite(1 + key, obj.keyBuf, i*EEPROM_PARAM_SIZE);
}

//!} Библиотека работы с EEPROM ================================================
//!} ===========================================================================
#endif // EEPROM_LIB