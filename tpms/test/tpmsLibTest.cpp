// тест библиотеки TPMS

#include "..\tpms.h"
#include "..\tpms.cpp"

#define CMD_GET_WHEEL 1
#define CMD_GET_ALL 2
#define CMD_SET_WHEEL 3
#define CMD_WRITE_WHEEL 4
#define CMD_WRITE_ALL 5
#define CMD_CLEAR_COMMON_TAG 6

#define CMD_MIN CMD_GET_WHEEL
#define CMD_MAX CMD_CLEAR_COMMON_TAG

#define CMD_PARAMS_MAX 6
#define WAIT_CMD_MS 250
#define WAIT_TO_DIAG_MS 2000

new wheels[TPMS_WHEELS_MAX][TPMS_WHEEL_DATA];

main()
{
    while (true)
    {
        new cmd = GetVar(gCmd);
        if ((cmd < CMD_MIN) || (cmd > CMD_MAX))
        {
            Delay(WAIT_CMD_MS);
            continue;
        }
        new param[CMD_PARAMS_MAX];
        param[0] = GetVar(gParam0);
        param[1] = GetVar(gParam1);
        param[2] = GetVar(gParam2);
        param[3] = GetVar(gParam3);
        param[4] = GetVar(gParam4);
        param[5] = GetVar(gParam5);
        if ((cmd == CMD_GET_WHEEL) || (cmd == CMD_SET_WHEEL) || (cmd == CMD_WRITE_WHEEL))
            coerce(param[0], TPMS_WHEEL_INDEX_MIN, TPMS_WHEEL_INDEX_MAX);
        
        Delay(WAIT_TO_DIAG_MS);
        switch (cmd)
        {
            case CMD_GET_WHEEL, CMD_GET_ALL:
            {
                new initIdx = (cmd == CMD_GET_WHEEL ? param[0] : 0);
                new count = (cmd == CMD_GET_WHEEL ? param[0] + 1 : TPMS_WHEELS_MAX);
                for (new i = initIdx; i < count; i++)
                    Diagnostics("Wheel=%d:pres=%d,temp=%d,online=%d,ok=%d,reason=%d", i, wheels[i].pressurePsi, wheels[i].temperature,
                                wheels[i].sensorOnline, wheels[i].sensorOk, wheels[i].reason);
            }
            case CMD_SET_WHEEL:
            {
                Diagnostics("Set wheel=%d:pres=%d,temp=%d,online=%d,ok=%d,reason=%d", param[0], param[1], param[2], param[3], param[4], param[5]);
                wheels[param[0]].pressurePsi = param[1];
                wheels[param[0]].temperature = param[2];
                wheels[param[0]].sensorOnline = param[3];
                wheels[param[0]].sensorOk = param[4];
                wheels[param[0]].reason = param[5];
            }
            case CMD_WRITE_WHEEL:
            {
                tpmsWriteWheelTag(param[0], wheels[param[0]]);
            }
            case CMD_WRITE_ALL, CMD_CLEAR_COMMON_TAG:
            {
                tpmsWriteCommonTag(wheels, cmd == CMD_WRITE_ALL);
            }
        }
        if ((((cmd == CMD_WRITE_WHEEL) && param[1]) || (((cmd == CMD_WRITE_ALL) || (cmd == CMD_CLEAR_COMMON_TAG)) && param[0])))
            SavePoint();

        SetVar(gCmd, 0);
    }
}