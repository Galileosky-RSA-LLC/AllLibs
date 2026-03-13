//! @file
//! @brief Функции библиотеки общих функций алгоритма

#if defined ALG_LIB
#endinput
#endif
#define ALG_LIB

#include "..\..\numeric\numeric.h"
#include "..\..\numeric\numeric.cpp"

stock bool:isSettingsInited()
{
    return GetVar(gIsSettingsInited) != 0;
}

stock setSettingsInited()
{
    SetVar(gIsSettingsInited, true);
}

stock bool:isNeedSwithRoute()
{
    return GetVar(gIsNeedSwithRoute) != 0;
}

stock setNeedSwithRoute(bool:value)
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
    routeCurrentData.currentAdvertisementFilePos = 0;
    routeCurrentData.nextAdvertisementFilePos = 0;
    routeCurrentData.show = SHOW_UNKNOWN;
}

stock restoreRouteCurrentData(routeCurrentData[ROUTE_CURRENT_DATA])
{
    routeCurrentData.crc = GetVar(gRouteCrc);
    routeCurrentData.isAtStation = GetVar(gIsAtStation) != 0;
    routeCurrentData.nextStationFilePos = GetVar(gNextStationFilePos);
    routeCurrentData.currentAdvertisementFilePos = GetVar(gCurrentAdvertisementFilePos);
    routeCurrentData.nextAdvertisementFilePos = GetVar(gNextAdvertisementFilePos);
    routeCurrentData.show = GetVar(gShow);
    routeCurrentData.showStartUptime = GetVar(gShowStartUptime);
}

stock storeRouteCurrentData(const routeCurrentData[ROUTE_CURRENT_DATA])
{
    SetVar(gRouteCrc, routeCurrentData.crc);
    SetVar(gIsAtStation, routeCurrentData.isAtStation);
    SetVar(gNextStationFilePos, routeCurrentData.nextStationFilePos);
    SetVar(gCurrentAdvertisementFilePos, routeCurrentData.currentAdvertisementFilePos);
    SetVar(gNextAdvertisementFilePos, routeCurrentData.nextAdvertisementFilePos);
    SetVar(gShow, routeCurrentData.show);
    SetVar(gShowStartUptime, routeCurrentData.showStartUptime);
}

stock bool:isDisplayInited(displayIndex)
{
    return getBit(_getIsDisplaysInited(), displayIndex);
}

stock setDisplayInit(displayIndex, bool:value)
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
                                ? (routeCurrentData.isAtStation ? SHOW_CURRENT_STATION : SHOW_ADVERTISEMENT)
                                : SHOW_NEXT_STATION);
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

stock calcRouteCrc(const routeName{})
{
    return CRC16(routeName, strLen(routeName, RAI_FILE_PATH_LENGTH_MAX));
}

stock bool:isNeedChangeShow(const routeCurrentData[ROUTE_CURRENT_DATA])
{
    return (routeCurrentData.show == SHOW_UNKNOWN) || isTimerExpired(routeCurrentData.showStartUptime, getMessageShowTimeS() * MS_PER_SECOND);
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
