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

#endif // DEVINFO_H
