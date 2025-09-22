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
    if (!raiGetCurrentRoute(route))
        Diagnostics("current route?");

    if (isNeedSwithRoute())
    {
        if (!raiGetNewRoute(route, route))
        {
            Diagnostics("new route?");
            return;
        }
		setNeedSwithRoute(false);
    }
    if (!route.name)
        return;

    Diagnostics("route=%s", route.name);
    new routeCurData[ROUTE_CURRENT_DATA];
    restoreRouteCurrentData(routeCurData);
    new crc = calcRouteCrc(route);
    if (routeCurData.crc != crc)
    {
        Diagnostics("init new route start");
        setAutoinformerRoute(route.name);
        PlayAudio(route.audioFilePath);
        raiSetRouteNameInUserArray(route);
        SavePoint();
        initRouteCurrentData(crc, routeCurData);
        storeRouteCurrentData(routeCurData);
		resetAlldisplaysInit();
        Diagnostics("init new route done");
	}
    new isFrontDisplayInited = isDisplayInited(FRONT_DISPLAY_INDEX);
    new isSideDisplayInited = isDisplayInited(SIDE_DISPLAY_INDEX);
    new hasFinalStations = false;
	if (!isFrontDisplayInited || !isSideDisplayInited)
    {
        if (hasFinalStations = raiGetFinalStations(route))
            Diagnostics("final stations:\"%s\"-\"%s\"", route.startStation, route.endStation);
        else
            Diagnostics("final stations?");
    }
	new res = true; // для шаблона - положительный результат, в реальных применениях - сначала отрицательный
    if (!isFrontDisplayInited)
	{
        Diagnostics("init front display start");
        // res = ...
		if (res)
			setDisplayInit(FRONT_DISPLAY_INDEX, true);
    	
        Diagnostics("init front display %s", res ? "done" : "error");
	}
	if (!isSideDisplayInited)
	{
		Diagnostics("init side display start");
        // res = ...
		if (res)
			setDisplayInit(FRONT_DISPLAY_INDEX, true);
    	
        Diagnostics("init side display %s", res ? "done" : "error");
	}
	if (!isDisplayInited(REAR_DISPLAY_INDEX))
	{
		Diagnostics("init rear display start");
        // res = ...
		if (res)
			setDisplayInit(FRONT_DISPLAY_INDEX, true);
    	
        Diagnostics("init rear display %s", res ? "done" : "error");
	}
	if (!isDisplayInited(SALON_DISPLAY_INDEX))
	{
		Diagnostics("init salon display start");
        // res = ...
        Diagnostics("init salon display %s", res ? "done" : "error");
        if (!res)
            return;

		setDisplayInit(FRONT_DISPLAY_INDEX, true);
	}
    new timeZone = getTimeZone();
    // res = ... timeZone
    if (!res)
        return;

    const textMaxSize = RAI_STRING_LENGTH_MAX + (sizeof(NEXT_STATION_PREFIX) * CELL_BYTES);
    const textMaxSizeW0 = textMaxSize + 1;
    new const currentStationPrefixLength = strLen(CURRENT_STATION_PREFIX);
    new const nextStationPrefixLength = strLen(NEXT_STATION_PREFIX);
    new text{textMaxSizeW0};
    new nextStation{RAI_STRING_LENGTH_MAX_W0};
    new hasMessage = false;
    if (raiIsAtStation(route, text, RAI_STRING_LENGTH_MAX, nextStation, RAI_STRING_LENGTH_MAX, routeCurData.nextStationFilePos))
    {
        hasMessage = true;
        Diagnostics("at station=\"%s\",next=\"%s\"", text, nextStation);
        if (!routeCurData.isAtStation)
        {    
            routeCurData.isAtStation = true;
            routeCurData.show = SHOW_UNKNOWN;
        }
        if ((routeCurData.show == SHOW_UNKNOWN) || isTimerExpired(routeCurData.showStartUptime, getMessageShowTimeS() * MS_PER_SECOND))
            changeShow(routeCurData);
    }
    else
    {
        if (routeCurData.isAtStation)
        {    
            routeCurData.isAtStation = false;
            routeCurData.show = SHOW_UNKNOWN;
        }
        if ((routeCurData.show == SHOW_UNKNOWN) || isTimerExpired(routeCurData.showStartUptime, getMessageShowTimeS() * MS_PER_SECOND))
        {
            changeShow(routeCurData);
            if (routeCurData.show == SHOW_ADVERTISMENT)
                routeCurData.currentAdvertismentFilePos = routeCurData.nextAdvertismentFilePos;
        }
        for (new i = 0; i < 2; i++)
        {
            if (routeCurData.show == SHOW_ADVERTISMENT)
            {
                hasMessage = raiGetAdvertisment(route, routeCurData.currentAdvertismentFilePos, text, RAI_STRING_LENGTH_MAX, routeCurData.nextAdvertismentFilePos);
                Diagnostics(hasMessage ? "adv=\"%s\"" : "adv?", text);
            }
            else
            {
                hasMessage = raiGetNextStation(route, routeCurData.nextStationFilePos, text, RAI_STRING_LENGTH_MAX);
                Diagnostics(hasMessage ? "next station=\"%s\"" : "next station?", text);
            }
            if (hasMessage)
                break;

            changeShow(routeCurData);
        }
    }
    if (hasMessage)
    {
        if (routeCurData.show == SHOW_CURRENT_STATION)
        {
            insertArrayStr(text, currentStationPrefixLength, textMaxSize, text, strLen(text), 0, true);
            insertArrayStr(text, 0, textMaxSize, CURRENT_STATION_PREFIX, currentStationPrefixLength);
        }
        else if (routeCurData.show == SHOW_NEXT_STATION)
        {
            insertArrayStr(text, 0, textMaxSize, NEXT_STATION_PREFIX, nextStationPrefixLength);
            strncpy(text, nextStationPrefixLength, textMaxSize, nextStation);
        }
        Diagnostics("show %s", routeCurData.show == SHOW_CURRENT_STATION ? "current station" : (routeCurData.show == SHOW_NEXT_STATION ? "next station" : "adv"));//!!!
        Diagnostics("text=%s", text);//!!!
        
        // показать информацию на табло
        // res = ... text
        if (!res)
            resetShowTimer(routeCurData);
    }
    storeRouteCurrentData(routeCurData);
}