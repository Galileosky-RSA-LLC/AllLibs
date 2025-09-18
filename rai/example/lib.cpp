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

stock getMessageShowTimeMs()
{
    return GetVar(gMessageShowTimeMs);
}

stock setMessageShowTimeMs(value)
{
    SetVar(gMessageShowTimeMs, value);
}

stock restoreRouteCurrentData(routeCurrentData[ROUTE_CURRENT_DATA])
{
    routeCurrentData.nextStationFilePos = GetVar(gNextStationFilePos);
    routeCurrentData.advertismentFilePos = GetVar(gAdvertismentFilePos);
    !!!routeCurrentData.isShowAdvertisment = GetVar(gIsShowAdvertisment);
    routeCurrentData.advertismentStartUptime = GetVar(gAdvertismentStartUptime);
    routeCurrentData.crc = GetVar(gRouteCrc);
}

stock storeRouteCurrentData(const routeCurrentData[ROUTE_CURRENT_DATA])
{
    SetVar(gNextStationFilePos, routeCurrentData.nextStationFilePos);
    SetVar(gAdvertismentFilePos, routeCurrentData.advertismentFilePos);
    !!!route.isShowAdvertisment = SetVar(gIsShowAdvertisment);
    route.advertismentStartUptime = GetVar(gAdvertismentStartUptime);
    SetVar(gRouteCrc, routeCurrentData.crc);
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

// приватные функции

stock _getIsDisplaysInited()
{
    return GetVar(gIsDisplaysInited);
}

stock _setIsDisplaysInited(value)
{
    return SetVar(gIsDisplaysInited, value);
}