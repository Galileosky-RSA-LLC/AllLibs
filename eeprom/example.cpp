//! @file
//! @brief пример управления настройками

//#define DEBUG // включить для отладки

#define INIT_STORAGE_WAIT_MS 10000

#if defined DEBUG
#define EEPROM_DEBUG
#define INIT_DIAG_AFTER_RESET_WAIT_MS 10000
#define INIT_WAIT_MS (INIT_STORAGE_WAIT_MS + INIT_DIAG_AFTER_RESET_WAIT_MS)
#else
#define INIT_WAIT_MS INIT_STORAGE_WAIT_MS
#endif

// адреса глобальных переменных с настройками
new const g_varAddr[] =
[
    GetVarAddr(gPortNum),
    GetVarAddr(gPortBaudRate),
    GetVarAddr(gPortStopBits),
    GetVarAddr(gPortParity),
    GetVarAddr(gPortTimeoutMs),
    GetVarAddr(gAddress),
];

#define PARAMS_AMOUNT sizeof(g_varAddr)
#define EEPROM_PARAMS_COUNT PARAMS_AMOUNT

#define CHANGE_PARAMS_DELAY_MS 500

#include "..\devinfo\devinfo.h"
#include "..\devinfo\devinfo.cpp"
#include "eeprom.h"
#include "eeprom.cpp"

main()
{
    Delay(INIT_WAIT_MS);
    new eeprom[EEPROM_DATA];
    new devModel = getModel();
    new softMaj;
    new softMin;
    getSoftVersion(softMaj, softMin);
    new romAvailable = isRomAvailable(devModel, softMaj, softMin);
    if (romAvailable)
    {
        eepromInitParamValues(eeprom, g_varAddr, PARAMS_AMOUNT);
        eepromRestoreParams(eeprom);
    }
    else
    {
        #if defined DEBUG
        Diagnostics("ROM-");
        #endif
        // тут можно инициализировать настройки из файлов
    }
    SetVar(gIsSettingsInited, true); //!< сообщаем другим потокам, что настройки инициализированы; а до этого момента они должны ждать
    while (true)
    {
        if (romAvailable)
        {
            eepromStoreParamsByChange(eeprom);
        }
        else
        {
            // мониторим изменения и сохраняем в файлы
        }
        Delay(CHANGE_PARAMS_DELAY_MS);
    }
}