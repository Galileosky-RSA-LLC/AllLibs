//! @file
//! @brief Управление настройками

//#define DEBUG // включить для отладки

#define INIT_STORAGE_WAIT_MS 10000

#ifdef DEBUG
#define INIT_DIAG_AFTER_RESET_WAIT_MS 10000
#define DEBUG_INIT_WAIT_MS (INIT_DIAG_AFTER_RESET_WAIT_MS + INIT_STORAGE_WAIT_MS)
#endif

new const varAddr[] =
[
    GetVarAddr(gDisplayDummy0),
    GetVarAddr(gDisplayDummy1),
];

#define CHANGE_PARAMS_DELAY_MS 500

#define SETTING_PARAMS_AMOUNT sizeof(varAddr)

#include "lib.h"
#include "lib.cpp"
#include "..\..\setting\setting.h"
#include "..\..\setting\setting.cpp"
#include "..\..\file\file.h"

stock const DISPLAY_SETTINGS_FILE_PATH{} = "displayname/settings.bin";

main()
{
#ifdef DEBUG
    Delay(DEBUG_INIT_WAIT_MS);
#endif
    new advTime[SETTING_SINGLE_DATA];
    settingSingleInit(advTime, "advtime", GetVarAddr(gMessageShowTimeS));
    settingSingleRestoreParam(advTime);

    new timeZone[SETTING_SINGLE_DATA];
    settingSingleInit(timeZone, TIME_ZONE_FILE_PATH, GetVarAddr(gTimeZone));
    settingSingleRestoreParam(timeZone);

    new routeSwitchInputIdx[SETTING_SINGLE_DATA];
    settingSingleInit(routeSwitchInputIdx, "route_switch_input", GetVarAddr(gRouteSwitchInputIdx));
    settingSingleRestoreParam(routeSwitchInputIdx);
    
    new settings[SETTING_DATA];
    settingInit(settings, DISPLAY_SETTINGS_FILE_PATH, varAddr, SETTING_PARAMS_AMOUNT);
    settingRestoreParams(settings);

    setSettingsInited();
    while (true)
    {
        settingSingleStoreParamByChange(advTime);
        settingSingleStoreParamByChange(timeZone);
        settingSingleStoreParamByChange(routeSwitchInputIdx);

        settingStoreParamsByChange(settings);
        Delay(CHANGE_PARAMS_DELAY_MS);
    }
}
