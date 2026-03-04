//! @file
//! @brief Обработчик команд

//#define DEBUG // включить для отладки

#define CMD_SWITCH_ROUTE 1
#define CMD_ROUTE_INPUT 2
#define CMD_SET_ADV_TIME 3
#define CMD_SET_TIME_ZONE 4
#define CMD_DISPLAY 5

#define CMD_MIN CMD_SWITCH_ROUTE
#define CMD_MAX CMD_DISPLAY

#define PARAMS_QTY_MAX 1

#define ADV_TIME_S_MIN 20

#define TIME_ZONE_MIN -12
#define TIME_ZONE_MAX 14
#define TIME_ZONE_UNKNOWN (TIME_ZONE_MIN - 1)

#define SWITCH_TO_DIAG_WAIT_MS 2000

#include "lib.h"
#include "lib.cpp"
#include "..\..\gdefines.h"
#include "..\..\cmdhandle\cmdhandle.h"
#include "..\..\cmdhandle\cmdhandle.cpp"

new g_cmdhdl[CMDHANDLE_DATA];
new g_cmdParams[PARAMS_QTY_MAX];

main()
{
    new cmd = GetVar(gCmd);
    if ((cmd < CMD_MIN) || (cmd > CMD_MAX))
    {
        sendTextAnswer("cmd?");
        return;
    }
    if ((cmd > CMD_SWITCH_ROUTE) && !isSettingsInited())
    {
        sendTextAnswer("wait init");
        return;
    }
#if defined DEBUG
    Delay(SWITCH_TO_DIAG_WAIT_MS);
#endif
    if (cmd >= CMD_DISPLAY)
        getCmdParams();

    switch (cmd)
    {
        case CMD_SWITCH_ROUTE: cmdSwitchRoute();
        case CMD_ROUTE_INPUT: cmdRouteInput();
        case CMD_SET_ADV_TIME: cmdAdvTime();
        case CMD_SET_TIME_ZONE: cmdTimeZone();
        case CMD_DISPLAY: displayCmd();
    }
    cmdHandlerSendPreparedAnswer(g_cmdhdl);
}

sendTextAnswer(const text{})
{
    cmdHandlerAddTextToAnswer(g_cmdhdl, text);
    cmdHandlerSendPreparedAnswer(g_cmdhdl);
}

getCmdParams()
{
    g_cmdParams[0] = GetVar(gCmdParam0);
}

cmdSwitchRoute()
{
    setNeedSwithRoute(true);
    cmdHandlerAddTextToAnswer(g_cmdhdl, "route switch sheduled");
}

cmdRouteInput()
{
    new inputIdx = GetVar(gRouteInputIdxNew);
    if ((inputIdx >= ADC_INPUT_IDX_MIN) && (inputIdx <= ADC_INPUT_IDX_MAX))
        setRouteSwitchInputIdx(inputIdx);

    SetVar(gRouteInputIdxNew, -1);
    makeRouteInputIdxAnswer();
}

makeRouteInputIdxAnswer()
{
    cmdHandlerAddParValueToAnswer(g_cmdhdl, "routeSwitchInputIdx=", getRouteSwitchInputIdx());
}

cmdAdvTime()
{
    new advTimeS = GetVar(gAdvTimeSnew);
    if (advTimeS >= ADV_TIME_S_MIN)
        setMessageShowTimeS(advTimeS);

    SetVar(gAdvTimeSnew, 0);
    makeAdvTimeAnswer();
}

makeAdvTimeAnswer()
{
    cmdHandlerAddParValueToAnswer(g_cmdhdl, "advTime=", getMessageShowTimeS());
}

cmdTimeZone()
{
    new timeZone = GetVar(gTimeZoneNew);
    if ((timeZone >= TIME_ZONE_MIN) && (timeZone <= TIME_ZONE_MAX))
        setTimeZone(timeZone);

    SetVar(gTimeZoneNew, TIME_ZONE_UNKNOWN);
    makeTimeZoneAnswer();
}

makeTimeZoneAnswer()
{
    cmdHandlerAddParValueToAnswer(g_cmdhdl, "timeZone=", getTimeZone());
}

displayCmd()
{
    // выполнение команды для табло
    cmdHandlerAddParValueToAnswer(g_cmdhdl, "display cmd accepted param=", g_cmdParams[0]);
}