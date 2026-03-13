#if defined SETTING_LIB
#endinput
#endif
#define SETTING_LIB

//! @file
//! @brief Реализация библиотеки файловых настроек

#include "setting.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"

//! @publicsection

stock settingInit(obj[SETTING_DATA], const fileName{}, const varAddresses[], varAddressesSize)
{
	strncpy(obj.fileName, SETTING_FILENAME_SIZE_MAX, fileName);
    for (new i = 0; (i < varAddressesSize) && (i < SETTING_PARAMS_AMOUNT); i++)
    {    
        obj.varAddr[i] = varAddresses[i];
        obj.oldValue[i] = GetVar(obj.varAddr[i]);
#if defined SETTING_DEBUG
        Diagnostics("\"%s\" param[%d] def=%d", obj.fileName, i, obj.oldValue[i]);
#endif
    }
}

stock settingStoreParamsByChange(obj[SETTING_DATA])
{
    for (new i = 0; i < SETTING_PARAMS_AMOUNT; i++)
	{
		new actualValue = GetVar(obj.varAddr[i]);
        if (actualValue == obj.oldValue[i])
            continue;

        new tmp = obj.oldValue[i];
        obj.oldValue[i] = actualValue;
        if (!setting_storeParam(obj, i))
        {
            Diagnostics("\"%s\" param[%d] write? v=%d", obj.fileName, i, actualValue);
            obj.oldValue[i] = tmp; // чтобы сохранить потом, когда работа с файлами восстановится
            break;
        }
	}
}

stock settingRestoreParams(obj[SETTING_DATA])
{
    new size = FileRead(obj.fileName, obj.oldValue, SETTING_BUF_SIZE);
    new i;
    for (i = 0; (i < size / SETTING_PARAM_SIZE) && (i < SETTING_PARAMS_AMOUNT); i++)
    {    
        SetVar(obj.varAddr[i], obj.oldValue[i]);
#if defined SETTING_DEBUG
        Diagnostics("\"%s\" param[%d] read=%d", obj.fileName, i, obj.oldValue[i]);
#endif
    }
    for (; i < SETTING_PARAMS_AMOUNT; i++)
        obj.oldValue[i] = GetVar(obj.varAddr[i]); // т.к. параметры восстановлены не все, вернем оставшиеся
}

stock settingSingleInit(obj[SETTING_SINGLE_DATA], const fileName{}, varAddress)
{
	strncpy(obj.fileName, SETTING_FILENAME_SIZE_MAX, fileName);
    obj.varAddr = varAddress;
    obj.oldValue = GetVar(obj.varAddr);
#if defined SETTING_DEBUG
    Diagnostics("\"%s\" def=%d", obj.fileName, obj.oldValue);
#endif
}

stock settingSingleRestoreParam(obj[SETTING_SINGLE_DATA])
{
    new buf{NUM_VALUE_MIN_STR_LENGTH};
    new readSize = FileRead(obj.fileName, buf, NUM_VALUE_MIN_STR_LENGTH, 0);
    new value;
    if ((readSize < 1) || !atoi(buf, value, readSize, 0)) 
    {
        Diagnostics("\"%s\" read?", obj.fileName);
        return;
    }
    obj.oldValue = value;
    SetVar(obj.varAddr, obj.oldValue);
#if defined SETTING_DEBUG
    Diagnostics("\"%s\" read!v=%d", obj.fileName, obj.oldValue);
#endif
}

stock settingSingleStoreParamByChange(obj[SETTING_SINGLE_DATA])
{
    new actualValue = GetVar(obj.varAddr);
    if (actualValue == obj.oldValue)
        return;

    new buf{NUM_VALUE_MIN_STR_LENGTH};
    new size = itoa(actualValue, buf, NUM_VALUE_MIN_STR_LENGTH, 0);
    if (FileDelete(obj.fileName) || (FileWrite(obj.fileName, buf, size, 0) < size))
    {    
        Diagnostics("\"%s\" write?v=%d", obj.fileName, actualValue);
        return;
    }
    obj.oldValue = actualValue;
}

//! @privatesection

//! @brief Сохранить параметр и все несохраненные до него
stock bool:setting_storeParam(obj[SETTING_DATA], idx)
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
