//! @file
//! @brief Функции библиотеки общих функций алгоритма

#ifdef ALG_LIB
#endinput
#endif
#define ALG_LIB

#include "..\rai.h"
#include "..\rai.cpp"

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