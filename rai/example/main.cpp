//! @file
//! @brief Основное взаимодействие автоинформатора и табло

#ifdef DEBUG
#define RAI_DEBUG
#endif

#include "..\rai.h"
#include "..\rai.cpp"
#include "..\..\time\time.h"
#include "..\..\time\time.cpp"
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

    if (!isNeedSwithRoute())
    {
        if (!route.name)
            return;
    }
    else
    {
        // обработка команды на смену маршрута
        if (!raiGetNewRoute(route, route))
        {
            Diagnostics("new route?");
            return;
        }
		setNeedSwithRoute(false);
    }
    Diagnostics("route=%s", route.name);
    if (route.crc == getRouteCrc())
    {
        //!!! восстановить остальные поля объекта из глобальных переменных
        restoreRouteCurrentData(route);
    }
    else
    {
        // маршрут изменился
        Diagnostics("init new route start");
        setAutoinformerRoute(route.name);
        PlayAudio(route.audioFilePath);
        raiSaveRouteNameInTag(route);
        SavePoint();
        setRouteCrc(route.crc);
		SetVar(gDisplaysInit, 0xFFFFFFFF); // Нужна инициализация всех табло
        Diagnostics("init new route done");
	}

    if (GetVar(gRouteInit)) // инициализация маршрута: вывод маршрутной информации
	{
		// требуется инициализация всех табло
		SetVar(gFrontDisplayInit, 1);
		SetVar(gSideDisplayInit, 1);
		SetVar(gRearDisplayInit, 1);
		SetVar(gSalonDisplayInit, 1);
        SetVar(gRouteInit, 0);
	}

	if (GetVar(gFrontDisplayInit) || GetVar(gSideDisplayInit))
	{
		if (!raiGetFinalStations(route))
		{
			Diagnostics("ERROR: Final stations empty");
			return;
		}
	}

	new portNum = GetVar(gPortnum);
    new baudRate = GetVar(gBaudRate);
    new stopBits = GetVar(gStopbits);
    new parity = GetVar(gParity);
    PortInit(portNum, baudRate, ISDT_PORT_BUFFER_SIZE, stopBits, parity);
	new timeout = GetVar(gTimeout);
	new res; // результат взаимодействия с табло
	// вести учет инициализации каждого табло и если неуспешная, то повторять в следующих вызовах
	new display[DISPLAY_DATA];
	if (GetVar(gFrontDisplayInit)) // Инициализация лобового табло
	{
		display.addr = GetVar(gFrontDisplayAddr);
		display.width = GetVar(gFrontDisplayWidth);
		display.height = GetVar(gFrontDisplayHeight);
		display.fontSize = GetVar(gFrontDisplayFontSize);
		display.routeWidth = GetVar(gFrontDisplayRouteWidth);
		display.routeFontSize = GetVar(gFrontDisplayRouteFontSize);
		display.textSpeed = GetVar(gFrontDisplayTextSpeed);
		res = isdtInitFrontSideDisplay(portNum, timeout, display, route, diag);
		if (res == ISDT_OK)
			SetVar(gFrontDisplayInit, 0);
		else
			Diagnostics("ERROR %d. Front display not initialized", res);
	}

	if (GetVar(gSideDisplayInit)) // Инициализация бокового табло
	{
		display.addr = GetVar(gSideDisplayAddr);
		display.width = GetVar(gSideDisplayWidth);
		display.height = GetVar(gSideDisplayHeight);
		display.fontSize = GetVar(gSideDisplayFontSize);
		display.routeWidth = GetVar(gSideDisplayRouteWidth);
		display.routeFontSize = GetVar(gSideDisplayRouteFontSize);
		display.textSpeed = GetVar(gSideDisplayTextSpeed);
		res = isdtInitFrontSideDisplay(portNum, timeout, display, route, diag);
		if (res == ISDT_OK)
			SetVar(gSideDisplayInit, 0);
		else
			Diagnostics("ERROR %d. Side display not initialized", res);
	}

	if (GetVar(gRearDisplayInit)) // Инициализация заднего табло
	{
		display.addr = GetVar(gRearDisplayAddr);
		display.width = GetVar(gRearDisplayWidth);
		display.height = GetVar(gRearDisplayHeight);
		display.routeFontSize = GetVar(gRearDisplayRouteFontSize);
		res = isdtInitRearDisplay(portNum, timeout, display, route, diag);
		if (res == ISDT_OK)
			SetVar(gRearDisplayInit, 0);
		else
			Diagnostics("ERROR %d. Rear display not initialized", res);
	}

	display.addr = GetVar(gSalonDisplayAddr);
	if (GetVar(gSalonDisplayInit)) // Инициализация салонного табло
	{
		display.width = GetVar(gSalonDisplayWidth);
		display.height = GetVar(gSalonDisplayHeight);
		res = isdtInitSalonDisplay(portNum, timeout, display, diag);
		if (res == ISDT_OK)
			SetVar(gSalonDisplayInit, 0);
		else
			Diagnostics("ERROR %d. Salon display not initialized", res);
	}

    // вывод текущей остановки и сообщений
    if (!GetVar(gSalonDisplayInit))
	{
		new nextStationFilePos = GetVar(gNextStationFilePos); // восстановить сохраненную позицию следующей остановки в файле остановок;
		new text{RAI_STRING_LENGTH_MAX}; // выводимый текст
		display.fontSize = GetVar(gSalonDisplayFontSize);
		display.textSpeed = GetVar(gSalonDisplayTextSpeed);
		if (raiInStationGeoPos(route, text, nextStationFilePos, diag))
		{
			// вывод текущей остановки на табло
			res = isdtSendSalonText(portNum, timeout, display, text, diag);
			if (res != ISDT_OK)
				Diagnostics("ERROR %d. Station not displayed", res);
			unsetTimer(); // сброс таймера отображения остановочной и вспомогательной информации
			SetVar(gNextStationFilePos, nextStationFilePos);
		}
		else if (isTimerExpired()) // истек таймер смены остановочной и вспомогательной информации
		{
			new showAdv = GetVar(gShowAdv); // восстановить, что отображалось в прошлом вызове - следущая остановка или сообщение
			if (nextStationFilePos == 0)
				showAdv = 1; // неверная позиция следующей остановки, показать сообщение

			if (diag)
				Diagnostics("timer expired. nextStationFilePos=%d, showAdv=%d", nextStationFilePos, showAdv);

			new advFilePos = GetVar(gAdvertismentFilePos); // восстановить сохраненную позицию в файле сообщений
			if (diag)
				Diagnostics("restored advFilePos=%d", advFilePos);
			showAdv ? raiGetAdvertisment(route, advFilePos, text) : raiGetNextStation(route, nextStationFilePos, text);
			SetVar(gShowAdv, !showAdv); // сохранить между вызовами отображаемое - следующую остановку или сообщение, чтобы в следующий раз отображать другое
			SetVar(gAdvertismentFilePos, advFilePos); // сохранить между вызовами позицию в файле сообщений
			if (diag)
				Diagnostics("store advFilePos=%d", advFilePos);

			Delay(100);
			Diagnostics(text);
			// показать информацию text на табло
			res = isdtSendSalonText(portNum, timeout, display, text, diag);
			if (res != ISDT_OK)
				Diagnostics("ERROR %d. Message not displayed", res);
			setTimer(GetVar(gAdvTime)); // запустить таймер показа текста
		}
	}

    //!!! сохраняем маршрут в глобальных переменных
}