//! @file
//! @brief Основное взаимодействие автоинформатора и табло

#include "..\rai.h"
#include "..\rai.cpp"
#include "..\..\time\time.h"
#include "..\..\time\time.cpp"
#include "..\..\string\string.h"
#include "..\..\string\string.cpp"
#include "lib.h"
#include "lib.cpp"

#define PERIOD_MS 1000

stock const CURRENT_STATION_PREFIX{} = "Остановка: ";
stock const NEXT_STATION_PREFIX{} = "Следующая остановка: ";

main()
{
    if (!isSettingsInited() || !isTimerExpired(GetVar(gMainUptime), PERIOD_MS))
        return;

    SetVar(gMainUptime, GetVar(UPTIME));
    new route[RAI_ROUTE_DATA];
    new routeCurData[ROUTE_CURRENT_DATA];
    if (!getRoute(route, routeCurData))
        return;

    if (!checkAndInitDisplays(route))
        return;

    outputToSalonDisplay(route, routeCurData);
    storeRouteCurrentData(routeCurData);
}

bool:getRoute(route[RAI_ROUTE_DATA], routeCurData[ROUTE_CURRENT_DATA])
{
    if (!raiGetCurrentRoute(route))
        Diagnostics("current route?");

    if (isNeedSwithRoute())
    {
        if (!raiGetNewRoute(route, route))
        {
            Diagnostics("new route?");
            return false;
        }
        setNeedSwithRoute(false);
    }
    if (!route.name{0})
        return false;

    Diagnostics("route=%s", route.name);
    restoreRouteCurrentData(routeCurData);
    new const crc = calcRouteCrc(route.name);
    if (routeCurData.crc != crc)
    {
        Diagnostics("init new route start");
        setAutoinformerRoute(route.name);
        raiSetCurrentRoute(route);
        PlayAudio(route.audioFilePath);
        raiSetRouteNameInUserArray(route);
        SavePoint();
        initRouteCurrentData(crc, routeCurData);
        storeRouteCurrentData(routeCurData);
        resetAlldisplaysInit();
        Diagnostics("init new route done");
    }
    return true;
}

bool:checkAndInitDisplays(route[RAI_ROUTE_DATA])
{
    new const bool:isFrontDisplayInited = isDisplayInited(FRONT_DISPLAY_INDEX);
    new const bool:isSideDisplayInited = isDisplayInited(SIDE_DISPLAY_INDEX);
    new bool:hasFinalStations = false;
    if (!isFrontDisplayInited || !isSideDisplayInited)
    {
        if ((hasFinalStations = raiGetFinalStations(route)))
            Diagnostics("final stations:\"%s\"-\"%s\"", route.startStation, route.endStation);
        else
            Diagnostics("final stations?");
    }
    new bool:res = true; // для шаблона - положительный результат, в реальных применениях - сначала отрицательный
    if (!isFrontDisplayInited)
    {
        Diagnostics("init front display start");
        // res = ... route.name, hasFinalStations, route.startStation, route.endStation
        if (res)
            setDisplayInit(FRONT_DISPLAY_INDEX, true);
        
        Diagnostics("init front display %s", res ? "done" : "error");
    }
    if (!isSideDisplayInited)
    {
        Diagnostics("init side display start");
        // res = ... route.name, hasFinalStations, route.startStation, route.endStation
        if (res)
            setDisplayInit(FRONT_DISPLAY_INDEX, true);
        
        Diagnostics("init side display %s", res ? "done" : "error");
    }
    if (!isDisplayInited(REAR_DISPLAY_INDEX))
    {
        Diagnostics("init rear display start");
        // res = ... route.name
        if (res)
            setDisplayInit(FRONT_DISPLAY_INDEX, true);
        
        Diagnostics("init rear display %s", res ? "done" : "error");
    }
    if (!isDisplayInited(SALON_DISPLAY_INDEX))
    {
        Diagnostics("init salon display start");
        // res = ... route.name, hasFinalStations, route.startStation, route.endStation
        Diagnostics("init salon display %s", res ? "done" : "error");
        if (!res)
            return false;

        setDisplayInit(FRONT_DISPLAY_INDEX, true);
    }
    return true;
}

outputToSalonDisplay(const route[RAI_ROUTE_DATA], routeCurData[ROUTE_CURRENT_DATA])
{
    new bool:res = true; // для шаблона - положительный результат, в реальных применениях - сначала отрицательный
    
    // если нужно - отобразим время на салонном табло
    new const timeZone = getTimeZone();
    new const unixTime = GetVar(UNIX_TIME);
    // res = ... timeZone, unixTime
    if (!res)
        return;

    // если нужно - отобразим температуру на салонном табло
    // res = ...
    if (!res)
        return;

    const textMaxSize = RAI_STRING_LENGTH_MAX + (sizeof(NEXT_STATION_PREFIX) * CELL_BYTES);
    const textMaxSizeW0 = textMaxSize + 1;
    new text{textMaxSizeW0};
    if (!getCurrentSalonInfo(route, routeCurData, text, textMaxSize))
        return;

    // показываем информацию на табло
    // res = ... text
    if (res)
        resetShowTimer(routeCurData);
}

bool:getCurrentSalonInfo(const route[RAI_ROUTE_DATA], routeCurData[ROUTE_CURRENT_DATA], text{}, textMaxSize)
{
    new const currentStationPrefixLength = strLen(CURRENT_STATION_PREFIX);
    new const nextStationPrefixLength = strLen(NEXT_STATION_PREFIX);
    new nextStation{RAI_STRING_LENGTH_MAX_W0};
    new bool:hasMessage = false;
    if (raiIsAtStation(route, text, RAI_STRING_LENGTH_MAX, nextStation, RAI_STRING_LENGTH_MAX, routeCurData.nextStationFilePos))
    {
        Diagnostics("at station=\"%s\",next=\"%s\"", text, nextStation);
        if (!routeCurData.isAtStation)
        {    
            routeCurData.isAtStation = true;
            routeCurData.show = SHOW_UNKNOWN;
        }
        if (isNeedChangeShow(routeCurData))
        {    
            hasMessage = true;
            changeShow(routeCurData);
            if ((routeCurData.show == SHOW_NEXT_STATION) && !nextStation{0})
                changeShow(routeCurData);
        }
    }
    else
    {
        if (routeCurData.isAtStation)
        {    
            routeCurData.isAtStation = false;
            routeCurData.show = SHOW_UNKNOWN;
        }
        if (isNeedChangeShow(routeCurData))
        {
            for (new i = 0; (i < 2) && !hasMessage; i++)
            {
                changeShow(routeCurData);
                if (routeCurData.show == SHOW_ADVERTISEMENT)
                {    
                    routeCurData.currentAdvertisementFilePos = routeCurData.nextAdvertisementFilePos;
                    for (new j = 0; j < 2; j++)
                    {
                        hasMessage = raiGetAdvertisement(route, routeCurData.currentAdvertisementFilePos, text, RAI_STRING_LENGTH_MAX,
                                                        routeCurData.nextAdvertisementFilePos);
                        if (hasMessage)
                            break;

                        routeCurData.currentAdvertisementFilePos = 0;
                    }
                    Diagnostics(hasMessage ? "adv=\"%s\"" : "adv?", text);
                }
                else
                {
                    hasMessage = raiGetNextStation(route, routeCurData.nextStationFilePos, text, RAI_STRING_LENGTH_MAX);
                    Diagnostics(hasMessage ? "next station=\"%s\"" : "next station?", text);
                }
            }
        }
    }
    if (!hasMessage)
        return false;

    if (routeCurData.show == SHOW_CURRENT_STATION)
    {
        strncpy(text, textMaxSize, text, currentStationPrefixLength, .fromBack = true);
        strncpy(text, currentStationPrefixLength, CURRENT_STATION_PREFIX);
    }
    else if (routeCurData.show == SHOW_NEXT_STATION)
    {
        strncpy(text, textMaxSize, routeCurData.isAtStation ? nextStation : text, nextStationPrefixLength, .fromBack = true);
        strncpy(text, nextStationPrefixLength, NEXT_STATION_PREFIX);
    }
    Diagnostics("show text=\"%s\"", text);
    return true;
}