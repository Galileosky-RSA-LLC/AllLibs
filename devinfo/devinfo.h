#ifndef DEVINFO_H
#define DEVINFO_H
// Определение информации о приборе

#define DEVINFO_MODEL_UNKNOWN 0
#define DEVINFO_MODEL_7X 1
#define DEVINFO_MODEL_70 2
#define DEVINFO_MODEL_BB 3
#define DEVINFO_MODEL_10 4

#define DEVINFO_DEBUGLEVEL_UNKNOWN -1
#define DEVINFO_DEBUGLEVEL_OFF 0
#define DEVINFO_DEBUGLEVEL_MIN 1
#define DEVINFO_DEBUGLEVEL_STD 2
#define DEVINFO_DEBUGLEVEL_MAX 3

#define DEVINFO_STATUS_MOTION_BIT 0
#define DEVINFO_STATUS_INCLINE_BIT 1
#define DEVINFO_STATUS_IBUTTON_BIT 2
#define DEVINFO_STATUS_NOSIM_BIT 3
#define DEVINFO_STATUS_GEOFENCE_BIT 4
#define DEVINFO_STATUS_UINTLOW_BIT 5
#define DEVINFO_STATUS_GPSANTOFF_BIT 6
#define DEVINFO_STATUS_UINTBUSFAIL_BIT 7
#define DEVINFO_STATUS_EXTPOWERFAIL_BIT 8
#define DEVINFO_STATUS_ENGINEON_BIT 9
#define DEVINFO_STATUS_SHOCK_BIT 10
#define DEVINFO_STATUS_COORDS_BIT 11
#define DEVINFO_STATUS_SIGNALING_BIT 14
#define DEVINFO_STATUS_ALARM_BIT 15
#define DEVINFO_STATUS_MAINSRV_BIT 16
#define DEVINFO_STATUS_GPRS_BIT 17
#define DEVINFO_STATUS_GSMJAMMING_BIT 18
#define DEVINFO_STATUS_SECONDSRV_BIT 19
#define DEVINFO_STATUS_GNSSJAMMING_BIT 20
#define DEVINFO_STATUS_USB_BIT 21
#define DEVINFO_STATUS_SD_BIT 22

getModel();
getSoftVersion(&softMaj, &softMin);
getDebugLevel();
isPortInitHasResult(devModel, softMaj, softMin);
isRomAvailable(devModel, softMaj, softMin);
isTagWriteBeginAvailable(devModel, softMaj, softMin);
isWheelTagsAvailable(devModel, softMaj, softMin);
getFreeRam(&firmware, &zip = 0, &easyLogic = 0);
getTagMaxSize(tagId);
getTagValue(tagId, &value);
hasExtPower(devStatus);
isEngineOn(devStatus);
getInStatus(index);

#endif // DEVINFO_H
