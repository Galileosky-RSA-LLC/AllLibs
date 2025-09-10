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

stock getRouteCrc()
{
    return GetVar(gRouteCrc);
}

stock setRouteCrc(value)
{
    SetVar(gRouteCrc, value);
}

stock restoreRouteCurrentData(route[RAI_ROUTE_DATA])
{
    route.nextStationFilePos = GetVar(gNextStationFilePos);
    route.advertismentFilePos = GetVar(gAdvertismentFilePos);
    route.isShowAdvertisment = GetVar(gIsShowAdvertisment);
    route.advertismentStartUptime = GetVar(gAdvertismentStartUptime);
}

stock storeRouteCurrentData(const route[RAI_ROUTE_DATA])
{
    route.nextStationFilePos = GetVar(gNextStationFilePos);
    route.advertismentFilePos = GetVar(gAdvertismentFilePos);
    route.isShowAdvertisment = GetVar(gIsShowAdvertisment);
    route.advertismentStartUptime = GetVar(gAdvertismentStartUptime);
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

// приватные функции

stock _getIsDisplaysInited()
{
    return GetVar(gIsDisplaysInited);
}

stock _setIsDisplaysInited(value)
{
    return SetVar(gIsDisplaysInited, value);
}