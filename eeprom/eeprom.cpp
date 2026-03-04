#if defined EEPROM_LIB
#endinput
#endif
#define EEPROM_LIB

//! @file
//! @brief Реализация библиотеки работы с EEPROM

#include "eeprom.h"

//! @publicsection

stock eepromInitParamValues(obj[EEPROM_DATA], const addr[], addrSize)
{
	for (new i = 0; (i < addrSize) && (i < EEPROM_PARAMS_COUNT); i++)
    {    
        obj.keysVarAddr[i] = addr[i];
        obj.keysOldValue[i] = GetVar(obj.keysVarAddr[i]);
    }
}

stock eepromStoreParamsByChange(obj[EEPROM_DATA])
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
                #if defined EEPROM_DEBUG
                Diagnostics("w.p?k=%d", i);
                #endif
            }
            needWrite = false;
        }
	}
}

stock eepromRestoreParams(obj[EEPROM_DATA])
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
            #if defined EEPROM_DEBUG
            Diagnostics((res != 0) ? "r.p[%d]=%d" : "r.p[%d]?def=%d", id, (res != 0) ? obj.keyBuf[j] : obj.keysOldValue[id]);
            #endif
        }
	}
}

//! @privatesection

//! @brief Сохранить параметры группы по ключу, в котором они хранятся
//! @param[in] key идентификатор ключа
//! @return true - успешно, false - ошибка
stock eeprom_storeParams(obj[EEPROM_DATA], key)
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
