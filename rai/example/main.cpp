//! @file
//! @brief Основное взаимодействие автоинформатора и табло

#ifdef DEBUG
#define RAI_DEBUG
#endif

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
    
    // изменение часового пояса
	new timeZone = GetVar(gTimeZone);
	if (timeZone != GetVar(gTimeZoneOld))
	{
		SetVar(gTimeZoneOld, timeZone);
		storeSetting(TIMEZONE_FILE, timeZone);
	}

	// изменение времени вывода вспомогательной информации
	new advTime = GetVar(gAdvTime);
	if (advTime != GetVar(gAdvTimeOld))
	{
		SetVar(gAdvTimeOld, advTime);
		storeSetting(ADVTIME_FILE, advTime);
	}
	
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
    new crc = CRC16(route.name, strLen(route.name, RAI_FILE_PATH_LENGTH_MAX));
    if (routeCurData.crc != crc)
    {
        Diagnostics("init new route start");
        setAutoinformerRoute(route.name);
        PlayAudio(route.audioFilePath);
        raiSaveRouteNameInTag(route);
        SavePoint();
        routeCurData.crc = crc;
        routeCurData.isOnStation = false;
        routeCurData.nextStationFilePos = 0;
        routeCurData.advertismentFilePos = 0;
        routeCurData.show = SHOW_UNKNOWN;
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
    const textMaxSize = RAI_STRING_LENGTH_MAX_W0 + ((sizeof(NEXT_STATION_PREFIX) + 1) * 4);
    new const currentStationPrefixLength = strLen(CURRENT_STATION_PREFIX);
    new const nextStationPrefixLength = strLen(NEXT_STATION_PREFIX);
    new text{textMaxSize};
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
        if ((routeCurData.show == SHOW_UNKNOWN) || isTimerExpired(routeCurData.showStartUptime, getMessageShowTimeMs()))
            changeShow(routeCurData);
    }
    else
    {
        if (routeCurData.isAtStation)
        {    
            routeCurData.isAtStation = false;
            routeCurData.show = SHOW_UNKNOWN;
        }
        if ((routeCurData.show == SHOW_UNKNOWN) || isTimerExpired(routeCurData.showStartUptime, getMessageShowTimeMs()))
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

            changeShow(routeCurrentData);
        }
    }
    if (hasMessage)
    {
        !!! переместить в text префикс и название
        if (routeCurData.show == SHOW_CURRENT_STATION)
        {
            
        }
        Diagnostics("show %s", routeCurData.show == SHOW_CURRENT_STATION ? "current station" : (routeCurData.show == SHOW_NEXT_STATION ? "next station" : "adv"));//!!!
        Diagnostics("text=%s", text);//!!!
        
        // показать информацию на табло
        // res = ... text
        if (!res)
            resetShowTimer(routeCurrentData);
    }
    storeRouteCurrentData(routeCurData);
}