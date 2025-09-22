//! @file
//! @brief Функции библиотеки общих функций алгоритма

#ifdef ALG_LIB
#endinput
#endif
#define ALG_LIB

#include "..\rai.h"
#include "..\rai.cpp"
#include "..\..\numeric\numeric.h"
#include "..\..\numeric\numeric.cpp"

stock isSettingsInited()
{
    return GetVar(gIsSettingsInited);
}

stock setSettingsInited()
{
    SetVar(gIsSettingsInited, true);
}

stock isNeedSwithRoute()
{
    return GetVar(gIsNeedSwithRoute);
}

stock setNeedSwithRoute(value)
{
    SetVar(gIsNeedSwithRoute, value);
}

stock getMessageShowTimeS()
{
    return GetVar(gMessageShowTimeS);
}

stock setMessageShowTimeS(value)
{
    SetVar(gMessageShowTimeS, value);
}

stock initRouteCurrentData(routeCrc, routeCurrentData[ROUTE_CURRENT_DATA])
{
    routeCurrentData.crc = routeCrc;
    routeCurrentData.isAtStation = false;
    routeCurrentData.nextStationFilePos = 0;
    routeCurrentData.currentAdvertismentFilePos = 0;
    routeCurrentData.nextAdvertismentFilePos = 0;
    routeCurrentData.show = SHOW_UNKNOWN;
}

stock restoreRouteCurrentData(routeCurrentData[ROUTE_CURRENT_DATA])
{
    routeCurrentData.crc = GetVar(gRouteCrc);
    routeCurrentData.isAtStation = GetVar(gIsAtStation);
    routeCurrentData.nextStationFilePos = GetVar(gNextStationFilePos);
    routeCurrentData.currentAdvertismentFilePos = GetVar(gCurrentAdvertismentFilePos);
    routeCurrentData.nextAdvertismentFilePos = GetVar(gNextAdvertismentFilePos);
    routeCurrentData.show = GetVar(gShow);
    routeCurrentData.showStartUptime = GetVar(gShowStartUptime);
}

stock storeRouteCurrentData(const routeCurrentData[ROUTE_CURRENT_DATA])
{
    SetVar(gRouteCrc, routeCurrentData.crc);
    SetVar(gIsAtStation, routeCurrentData.isAtStation);
    SetVar(gNextStationFilePos, routeCurrentData.nextStationFilePos);
    SetVar(gCurrentAdvertismentFilePos, routeCurrentData.currentAdvertismentFilePos);
    SetVar(gNextAdvertismentFilePos, routeCurrentData.nextAdvertismentFilePos);
    SetVar(gShow, routeCurrentData.show);
    SetVar(gShowStartUptime, routeCurrentData.showStartUptime);
}

stock isDisplayInited(displayIndex)
{
    return getBit(_getIsDisplaysInited(), displayIndex);
}

stock setDisplayInit(displayIndex, value)
{
    _setIsDisplaysInited(setBit(_getIsDisplaysInited(), displayIndex, value));
}

stock resetAlldisplaysInit()
{
    _setIsDisplaysInited(0);
}

stock resetShowTimer(routeCurrentData[ROUTE_CURRENT_DATA])
{
    routeCurrentData.showStartUptime = GetVar(UPTIME);
}

stock changeShow(routeCurrentData[ROUTE_CURRENT_DATA])
{
    routeCurrentData.show = routeCurrentData.show == SHOW_UNKNOWN
                            ? (routeCurrentData.isAtStation ? SHOW_CURRENT_STATION : SHOW_NEXT_STATION)
                            : (routeCurrentData.show == SHOW_NEXT_STATION
                                ? (routeCurrentData.isAtStation ? SHOW_CURRENT_STATION : SHOW_ADVERTISMENT)
                                : SHOW_NEXT_STATION);
    resetShowTimer(routeCurrentData);
}

stock getTimeZone()
{
    return GetVar(gTimeZone);
}

stock setTimeZone(value)
{
    SetVar(gTimeZone, value);
}

stock getRouteSwitchInputIdx()
{
    return GetVar(gRouteSwitchInputIdx);
}

stock setRouteSwitchInputIdx(value)
{
    SetVar(gRouteSwitchInputIdx, value);
}

stock calcRouteCrc(const route[RAI_ROUTE_DATA])
{
    return CRC16(route.name, strLen(route.name, RAI_FILE_PATH_LENGTH_MAX));
}

//! @privatesection

stock _getIsDisplaysInited()
{
    return GetVar(gIsDisplaysInited);
}

stock _setIsDisplaysInited(value)
{
    return SetVar(gIsDisplaysInited, value);
}
