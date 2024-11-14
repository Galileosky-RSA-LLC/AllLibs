#ifndef SETTING_LIB
#define SETTING_LIB
//{ ============================================================================
//{ Библиотека работы с настройками ============================================
//{ ============================================================================

#include "setting.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"

// Публичные функции

settingInit(obj[SETTING_DATA], const fileName{}, const varAddresses[], varAddressesSize)
{
	insertArrayStr(obj.fileName, 0, SETTING_FILENAME_SIZE_MAX, fileName, strLen(fileName, SETTING_FILENAME_SIZE_MAX));
    for (new i = 0; (i < varAddressesSize) && (i < SETTING_PARAMS_AMOUNT); i++)
    {    
        obj.varAddr[i] = varAddresses[i];
        obj.oldValue[i] = GetVar(obj.varAddr[i]);
        #ifdef SETTING_DEBUG
        Diagnostics("\"%s\" def[%d]=%d", obj.fileName, i, obj.oldValue[i]);
        #endif
    }
}

//! Сохранить параметры по их изменению
settingStoreParamsByChange(obj[SETTING_DATA])
{
    for (new i = 0; i < SETTING_PARAMS_AMOUNT; i++)
	{
		new actualValue = GetVar(obj.varAddr[i]);
        if (actualValue != obj.oldValue[i])
        {
            new tmp = obj.oldValue[i];
            obj.oldValue[i] = actualValue;
            if (!setting_storeParam(obj, i))
            {
                Diagnostics("w.\"%s\"[%d]?v=%d", obj.fileName, i, actualValue);
                obj.oldValue[i] = tmp; // чтобы сохранить потом, когда работа с файлами восстановится
                break;
            }
        }
	}
}

//! Загрузить сохраненные параметры
settingRestoreParams(obj[SETTING_DATA])
{
    new size = FileRead(obj.fileName, obj.oldValue, SETTING_BUF_SIZE);
    new i;
    for (i = 0; i < size / 4; i++)
    {    
        SetVar(obj.varAddr[i], obj.oldValue[i]);
        #ifdef SETTING_DEBUG
        Diagnostics("r.\"%s\"[%d]=%d", obj.fileName, i, obj.oldValue[i]);
        #endif
    }
    for (; i < SETTING_PARAMS_AMOUNT; i++)
        obj.oldValue[i] = GetVar(obj.varAddr[i]); // т.к. параметры были не все, вернем
}

//! Инициализировать изолированный параметр
settingSingleInit(obj[SETTING_SINGLE_DATA], const fileName{}, varAddress)
{
	insertArrayStr(obj.fileName, 0, SETTING_FILENAME_SIZE_MAX, fileName, strLen(fileName, SETTING_FILENAME_SIZE_MAX));
    obj.varAddr = varAddress;
    obj.oldValue = GetVar(obj.varAddr);
    #ifdef SETTING_DEBUG
    Diagnostics("\"%s\" def=%d", obj.fileName, obj.oldValue);
    #endif
}

//! Восстановить значение изолированного параметра из файла
settingSingleRestoreParam(obj[SETTING_SINGLE_DATA])
{
    new buf{NUM_VALUE_MIN_STR_LENGTH};
    new readSize = FileRead(obj.fileName, buf, NUM_VALUE_MIN_STR_LENGTH, 0);
    new value;
    if ((readSize < 1) || !atoi(buf, 0, readSize, value)) 
    {
        Diagnostics("r.\"%s\"?", obj.fileName);
        return;
    }
    obj.oldValue = value;
    SetVar(obj.varAddr, obj.oldValue);
    #ifdef SETTING_DEBUG
    Diagnostics("r.\"%s\"!v=%d", obj.fileName, obj.oldValue);
    #endif
}

settingSingleStoreParamByChange(obj[SETTING_SINGLE_DATA])
{
    new actualValue = GetVar(obj.varAddr);
    if (actualValue == obj.oldValue)
        return;

    new buf{NUM_VALUE_MIN_STR_LENGTH};
    new size = itoa(actualValue, buf, 0, NUM_VALUE_MIN_STR_LENGTH);
    if (FileDelete(obj.fileName) || (FileWrite(obj.fileName, buf, size, 0) < size))
    {    
        Diagnostics("w.\"%s\"?v=%d", obj.fileName, actualValue);
        return;
    }
    obj.oldValue = actualValue;
}

// Приватные функции

//! Сохранить параметр и все несохраненные до него
setting_storeParam(obj[SETTING_DATA], idx)
{
	if ((idx < 0) || (idx >= SETTING_PARAMS_AMOUNT))
		return false;

    if ((idx > 0) && (FileSize(obj.fileName) < (idx * SETTING_PARAM_SIZE)))
    {    
        new size = SETTING_PARAM_SIZE * (idx + 1);
        return FileWrite(obj.fileName, obj.oldValue, size, 0) == size;
    }
    new buf[1];
    buf[0] = obj.oldValue[idx];
    return FileWrite(obj.fileName, buf, SETTING_PARAM_SIZE, idx * SETTING_PARAM_SIZE) == SETTING_PARAM_SIZE;
}

//!} Библиотека работы с настройками ===========================================
//!} ===========================================================================
#endif // SETTING_LIB
