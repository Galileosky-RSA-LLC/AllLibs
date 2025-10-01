//! @file
//! @brief Функции библиотеки маршрутного автоинформатора

#ifdef RAI_LIB
#endinput
#endif
#define RAI_LIB

#include "rai.h"
#include "..\string\string.h"
#include "..\string\string.cpp"
#include "..\file\file.h"
#include "..\file\file.cpp"
#include "..\userArray\userArray.h"
#include "..\userArray\userArray.cpp"

#define RAI_FRAC_SEPARATOR_SYMBOL '.'
#define RAI_COORD_PRECISION 6
#define RAI_ANGLE_PRECISION 2
#define RAI_RADIUS_PRECISION 2
#define RAI_PARAMS_SEPARATOR_SYMBOL ';'
#define RAI_PARAMS_SEPARATOR_SIZE 1

//! @publicsection

stock raiGetCurrentRoute(route[RAI_ROUTE_DATA])
{
    new res = rai_restoreRouteName(route) && rai_getFilePaths(route);
    if (!res)
        route.name{0} = 0;

    return res;
}

stock raiSetCurrentRoute(const route[RAI_ROUTE_DATA])
{
    new len = strLen(route.name);
    return (len > 0) && !FileDelete(RAI_CURRENT_ROUTE_FILE_PATH) && (FileWrite(RAI_CURRENT_ROUTE_FILE_PATH, route.name, len, 0) == len);
}

stock raiGetNewRoute(const currentRoute[RAI_ROUTE_DATA], nextRoute[RAI_ROUTE_DATA])
{
    strncpy(nextRoute.name, 0, RAI_FILE_PATH_LENGTH_MAX, currentRoute.name);
    new isReScan = !currentRoute.name{0};
    while (true)
    {
        if (NextDir("", nextRoute.name, nextRoute.name, RAI_FILE_PATH_LENGTH_MAX))
        {
            if (rai_getFilePaths(nextRoute))
                return true;

            continue;
        }
        else if (isReScan)
        {
            break;
        }
        isReScan = true;
        nextRoute.name{0} = 0;
    }
    nextRoute.name{0} = 0;
    return false;
}

stock raiGetFinalStations(route[RAI_ROUTE_DATA])
{
    route.startStation{0} = 0;
    route.endStation{0} = 0;
    if (!route.name{0} || (FileSize(route.finalStationsFilePath) < 0))
        return false;

    new pos = fileReadLine(route.finalStationsFilePath, route.startStation, RAI_STRING_LENGTH_MAX, 0);
    if (pos)
        fileReadLine(route.finalStationsFilePath, route.endStation, RAI_STRING_LENGTH_MAX, pos);

    return true;
}

stock raiIsAtStation(const route[RAI_ROUTE_DATA], currentStation{}, currentStationMaxSize, nextStation{}, nextStationMaxSize, &nextStationFilePos)
{
    new filePos = 0;
    new const fileSize = FileSize(route.busLineFilePath);
    while (filePos < fileSize)
    {
        new str{RAI_STRING_LENGTH_MAX};
        new readSize = fileReadLine(route.busLineFilePath, str, RAI_STRING_LENGTH_MAX, filePos);
        if (!readSize)
            return false;

        // В строке файла идет:
        // Широта;долгота;угол направления;угол разброса;внешний радиус R0;внутренний радиус R1;имя аудио файла;название остановки
        // типа:
        // 51.540487;46.004783;111.00;45.00;60.00;20.00;Universitatska_.wav;Остановка ул. Университетская
        new strPos = 0;
        new strLength = strLen(str);
        const zoneParamsQty = 6;
        new const precisions[zoneParamsQty] = [RAI_COORD_PRECISION, RAI_COORD_PRECISION, RAI_ANGLE_PRECISION, RAI_ANGLE_PRECISION, RAI_RADIUS_PRECISION,
                                                RAI_RADIUS_PRECISION];
        new zoneParams[zoneParamsQty];
        new len;
        for (new i = 0; i < zoneParamsQty; i++)
        {
            len = atofi(str, strPos, strLength, RAI_FRAC_SEPARATOR_SYMBOL, precisions[i], zoneParams[i]);
            if (!len)
                return false;

            strPos += len + RAI_PARAMS_SEPARATOR_SIZE;
        }
        if (!InZone(zoneParams[0], zoneParams[1], zoneParams[2], zoneParams[3], zoneParams[4], zoneParams[5]))
        {
            filePos += readSize;
            Delay(1);
            continue;
        }
        strPos = searchLinearStr(str, strLength, RAI_PARAMS_SEPARATOR_SYMBOL, strPos);
        if (strPos >= 0)
            strncpy(currentStation, 0, currentStationMaxSize, str, strPos + RAI_PARAMS_SEPARATOR_SIZE);

        nextStation{0} = 0;
        nextStationFilePos = -1;
        filePos += readSize;
        if (filePos >= fileSize)
            return true;

        readSize = fileReadLine(route.busLineFilePath, str, RAI_STRING_LENGTH_MAX, filePos);
        if (!readSize)
            return true;

        strPos = 0;
        strLength = strLen(str);
        const skipParamsQty = zoneParamsQty + 1;
        for (new i = 0; i < skipParamsQty; i++, strPos += RAI_PARAMS_SEPARATOR_SIZE)
        {
            strPos = searchLinearStr(str, strLength, RAI_PARAMS_SEPARATOR_SYMBOL, strPos);
            if (strPos < 0)
                return true;
        }
        nextStationFilePos = filePos + strPos;
        strncpy(nextStation, 0, nextStationMaxSize, str, strPos);
        return true;
    }
    return false;
}

stock raiGetAdvertisment(const route[RAI_ROUTE_DATA], filePos, advertisment{}, advertismentMaxSize, &nextPos)
{
    new readSize = fileReadLine(route.advertismentFilePath, advertisment, advertismentMaxSize, filePos);
    if (!readSize || !advertisment{0})
        return false;

    nextPos = filePos + readSize;
    return true;
}

stock raiGetNextStation(const route[RAI_ROUTE_DATA], filePos, station{}, stationMaxSize)
{
    return (filePos > 0) && (fileReadLine(route.busLineFilePath, station, stationMaxSize, filePos) > 0) && station{0};
}

stock raiSetRouteNameInUserArray(const route[RAI_ROUTE_DATA])
{
    setTextInUserArray(route.name);
}

//! @privatesection

stock rai_generateFilePath(const route[RAI_ROUTE_DATA], const fileName{}, filePath{})
{
    new pos = 0;
    filePath{0} = 0;
    new routeNameLength = strLen(route.name);
    new fileNameLength = strLen(fileName);
    if ((routeNameLength <= 0) || (fileNameLength <= 0) || ((routeNameLength + fileNameLength + 1) > RAI_FILE_PATH_LENGTH_MAX))
        return false;

    pos += insertArrayStr(filePath, pos, RAI_FILE_PATH_LENGTH_MAX, route.name, routeNameLength);
    pos += insertArrayStr(filePath, pos, RAI_FILE_PATH_LENGTH_MAX, PATH_SEPARATOR_PRIME, strLen(PATH_SEPARATOR_PRIME));
    strncpy(filePath, pos, RAI_FILE_PATH_LENGTH_MAX, fileName);
    return true;
}

stock rai_restoreRouteName(route[RAI_ROUTE_DATA])
{
    new len = FileRead(RAI_CURRENT_ROUTE_FILE_PATH, route.name, RAI_FILE_PATH_LENGTH_MAX);
    route.name{len <= 0 ? 0 : ((len < RAI_FILE_PATH_LENGTH_MAX) ? len : RAI_FILE_PATH_LENGTH_MAX)} = 0;
    return len > 0;
}

stock rai_getFinalStationsFilePath(route[RAI_ROUTE_DATA])
{
    return route.name{0}
            && rai_generateFilePath(route, RAI_FINAL_STATIONS_FILE_NAME, route.finalStationsFilePath)
            && (FileSize(route.finalStationsFilePath) >= 0);
}

stock rai_getFilePaths(route[RAI_ROUTE_DATA])
{
    if (!rai_getFinalStationsFilePath(route))
        return false;
    
    rai_generateFilePath(route, RAI_AUDIO_FILE_NAME, route.audioFilePath);
    rai_generateFilePath(route, RAI_BUSLINE_FILE_NAME, route.busLineFilePath);
    rai_generateFilePath(route, RAI_ADVERTISMENT_FILE_NAME, route.advertismentFilePath);
    return true;
}
