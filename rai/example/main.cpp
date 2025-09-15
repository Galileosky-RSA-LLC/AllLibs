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
	if ((!isFrontDisplayInited || !isSideDisplayInited) && !raiGetFinalStations(route))
    	Diagnostics("final stations?");

	new res = true; // для шаблона - положительный результат, в реальных применениях - сначала отрицательный
    if (!isFrontDisplayInited)
	{
        Diagnostics("init front display start");
        // res = ...
		if (res)
			setDisplayInit(FRONT_DISPLAY_INDEX, true);
    	
        Diagnostics(res ? "init front display done" : "init front display error");
	}
	if (!isSideDisplayInited)
	{
		Diagnostics("init side display start");
        // res = ...
		if (res)
			setDisplayInit(FRONT_DISPLAY_INDEX, true);
    	
        Diagnostics(res ? "init side display done" : "init side display error");
	}
	if (!isDisplayInited(REAR_DISPLAY_INDEX))
	{
		Diagnostics("init rear display start");
        // res = ...
		if (res)
			setDisplayInit(FRONT_DISPLAY_INDEX, true);
    	
        Diagnostics(res ? "init rear display done" : "init rear display error");
	}
	if (!isDisplayInited(SALON_DISPLAY_INDEX))
	{
		Diagnostics("init salon display start");
        // res = ...
        Diagnostics(res ? "init salon display done" : "init salon display error");
        if (!res)
            return;

		setDisplayInit(FRONT_DISPLAY_INDEX, true);
	}
    new text{RAI_STRING_LENGTH_MAX_W0};
    if (raiIsOnStation(route, text, RAI_STRING_LENGTH_MAX, routeCurData.nextStationFilePos))
    {
        // вывод текущей остановки на табло
        // res = ...
        if (res != ISDT_OK)
            Diagnostics("ERROR %d. Station not displayed", res);
        // !!! хорошо бы показывать: текущую, следующую, рекламу
        unsetTimer(); // сброс таймера отображения остановочной и вспомогательной информации
    }
    else if (isTimerExpired()) // истек таймер смены остановочной и вспомогательной информации
    {
        new showAdv = GetVar(gShowAdv); // восстановить, что отображалось в прошлом вызове - следущая остановка или сообщение
        if (nextStationFilePos == 0)
            showAdv = 1; // неверная позиция следующей остановки, показать сообщение

        new advFilePos = GetVar(gAdvertismentFilePos); // восстановить сохраненную позицию в файле сообщений
        showAdv ? raiGetAdvertisment(route, advFilePos, text) : raiGetNextStation(route, nextStationFilePos, text);
        SetVar(gShowAdv, !showAdv); // сохранить между вызовами отображаемое - следующую остановку или сообщение, чтобы в следующий раз отображать другое
        SetVar(gAdvertismentFilePos, advFilePos); // сохранить между вызовами позицию в файле сообщений

        Delay(100);
        Diagnostics(text);
        // показать информацию text на табло
        res = isdtSendSalonText(portNum, timeout, display, text, diag);
        if (res != ISDT_OK)
            Diagnostics("ERROR %d. Message not displayed", res);
        setTimer(GetVar(gAdvTime)); // запустить таймер показа текста
    }
    storeRouteCurrentData(routeCurData);
}