//! @file
//! @brief Проверка входа изменения маршрута

#include "lib.h"
#include "lib.cpp"
#include "..\..\gdefines.h"
#include "..\..\devinfo\devinfo.h"
#include "..\..\devinfo\devinfo.cpp"

#define WAIT_MS 250

main()
{
    while (true)
    {
        new inputIdx = getRouteSwitchInputIdx();
        if (isSettingsInited() && !isNeedSwithRoute() && (inputIdx >= ADC_INPUT_IDX_MIN) && (inputIdx <= ADC_INPUT_IDX_MAX) && getInStatus(inputIdx))
            setNeedSwithRoute(true);

        Delay(WAIT_MS);
    }
}