//! @file
//! @brief Реализация библиотеки определения информации о приборе

#if defined DEVINFO_LIB
#endinput
#endif
#define DEVINFO_LIB

#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\string\string.h"
#include "..\string\string.cpp"
#include "..\gdefines.h"
#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"
#include "devinfo.h"

stock getModel()
{
    const bufLength = 64;
    new buf{bufLength} = "SPECS";
    ExecCommand(buf);
    GetBinaryDataFromCommand(buf, bufLength);
    toLowerCase(buf, bufLength);
    if ((searchStr(buf, "galileosky 10") >= 0) || (searchStr(buf, "galileosky v10") >= 0))
        return DEVINFO_MODEL_10;

    if (searchStr(buf, "galileosky v7x") >= 0)
        return DEVINFO_MODEL_7X;
    
    if (searchStr(buf, "galileosky v7.0") >= 0)
        return DEVINFO_MODEL_70;
    
    return (searchStr(buf, "galileosky baseblock") >= 0) ? DEVINFO_MODEL_BB : DEVINFO_MODEL_UNKNOWN;
}

stock bool:getSoftVersion(&softMaj, &softMin)
{
    softMaj = 0;
    softMin = 0;
    const bufLength = 32;
    new buf{bufLength} = "STATUS";
    ExecCommand(buf);
    GetBinaryDataFromCommand(buf, bufLength);
    toLowerCase(buf, bufLength);
    new const prefix{} = "soft=";
    new const prefixLength = strLen(prefix);
    new pos = searchStr(buf, prefix);
    if (pos < 0)
        return false;

    pos += prefixLength;
    new len = atoi(buf, softMaj, bufLength, pos);
    if (!len)
        return false;

    pos += 1 + len;
    return atoi(buf, softMin, bufLength, pos);
}

stock getDebugLevel()
{
    const bufLength = 16;
    new buf{bufLength} = "DEBUGLEVEL";
    ExecCommand(buf);
    GetBinaryDataFromCommand(buf, bufLength);
    for (new i = 0; i < bufLength; i++)
    {
        if (!isDigit(buf{i}))
            continue;

        new const res = getDigit(buf{i});
        return res > DEVINFO_DEBUGLEVEL_MAX ? DEVINFO_DEBUGLEVEL_MAX : res;
    }
    return DEVINFO_DEBUGLEVEL_UNKNOWN;
}

stock bool:isPortInitHasResult(devModel, softMaj, softMin)
{
    return ((devModel == DEVINFO_MODEL_7X)
            && (((softMaj == 32) && (softMin >= 9)) || ((softMaj == 37) && (softMin >= 3)) || ((softMaj == 38) && (softMin >= 2)) || (softMaj >= 39)))
            ||
            (((devModel == DEVINFO_MODEL_70) || (devModel == DEVINFO_MODEL_BB))
            && (((softMaj == 23) && (softMin >= 25)) || ((softMaj == 25) && (softMin >= 10)) || (softMaj >= 26)))
            || (devModel >= DEVINFO_MODEL_10);
}

stock bool:isRomAvailable(devModel, softMaj, softMin)
{
    return ((devModel == DEVINFO_MODEL_7X) && (((softMaj == 43) && (softMin >= 3)) || (softMaj > 43)))
            || (devModel >= DEVINFO_MODEL_10);
}

stock bool:isTagWriteBeginAvailable(devModel, softMaj, softMin)
{
    return ((devModel == DEVINFO_MODEL_7X) && (((softMaj == 47) && (softMin >= 13)) || (softMaj > 47)))
            || ((devModel == DEVINFO_MODEL_10) && (((softMaj == 2) && (softMin >= 7)) || (softMaj > 2)))
            || (devModel > DEVINFO_MODEL_10);
}

stock bool:isWheelTagsAvailable(devModel, softMaj, softMin)
{
    return ((devModel == DEVINFO_MODEL_7X) && (((softMaj == 43) && (softMin >= 3)) || (softMaj > 43)))
            || (devModel >= DEVINFO_MODEL_10);
}

stock getFreeRam(&firmware, &zip = 0, &easyLogic = 0)
{
    ExecCommand("FREEMEM");
    const bufMaxSize = 56;
    new buf{bufMaxSize};
    const valuesMaxSize = 3;
    new values[valuesMaxSize];
    new valuesActualSize;
    strSplitNums(buf, ',', values, valuesMaxSize, valuesActualSize, GetBinaryDataFromCommand(buf, bufMaxSize), 8);
    if (valuesActualSize > 0)
        firmware = values[0];

    if (valuesActualSize > 1)
        zip = values[1];

    if (valuesActualSize > 2)
        easyLogic = values[2];

    return valuesActualSize;
}

stock getTagMaxSize(tagId)
{
    if (((tagId >= TAG_CAN32BITR_0) && (tagId <= TAG_CAN32BITR_4))
        || ((tagId >= TAG_CAN32BITR_5) && (tagId <= TAG_CAN32BITR_14))
        || ((tagId >= TAG_USER_0) && (tagId <= TAG_USER_7))
        || ((tagId >= TAG_CAN_A_0) && (tagId <= TAG_CAN_A_1))
        || ((tagId >= TAG_CAN_B_0) && (tagId <= TAG_CAN_B_1))
        || ((tagId >= TAG_BLUETOOTH_0) && (tagId <= TAG_BLUETOOTH_63))
        || ((tagId >= TAG_MODBUS_0) && (tagId <= TAG_MODBUS_63)))
        return 4;
    
    if (((tagId >= TAG_RS232_0_VALUE) && (tagId <= TAG_RS232_1_VALUE))
        || ((tagId >= TAG_CAN16BITR_0) && (tagId <= TAG_CAN16BITR_4))
        || ((tagId >= TAG_RS485_FLS0_VALUE) && (tagId <= TAG_RS485_FLS15_VALUE))
        || ((tagId >= TAG_CAN16BITR_5) && (tagId <= TAG_CAN16BITR_14)))
        return 2;
    
    if (((tagId >= TAG_CAN8BITR_0) && (tagId <= TAG_CAN8BITR_14))
        || ((tagId >= TAG_RS232_0_TEMP) && (tagId <= TAG_RS485_FLS15_TEMP))
        || ((tagId >= TAG_CAN8BITR_15) && (tagId <= TAG_CAN8BITR_30)))
        return 1;

    if ((tagId >= TAG_IBUTTON_0) && (tagId <= TAG_IBUTTON_1))
        return 8;

    if ((tagId >= TAG_WHEEL_0) && (tagId <= TAG_WHEEL_33))
        return 3;

    if (tagId == TAG_USER_ARRAY)
        return TAG_USER_ARRAY_MAX_SIZE;

    if (tagId == TAG_TPMS)
        return TAG_TPMS_SIZE;

    if (tagId == TAG_REFRIGERATOR)
        return TAG_REFRIGERATOR_MAX_SIZE;

    return 0;
}

stock bool:getTagValue(tagId, &value)
{
    switch (tagId)
    {
        case TAG_BLUETOOTH_0: value = GetVar(BLUETOOTH_0);
        case TAG_BLUETOOTH_1: value = GetVar(BLUETOOTH_1);
        case TAG_BLUETOOTH_2: value = GetVar(BLUETOOTH_2);
        case TAG_BLUETOOTH_3: value = GetVar(BLUETOOTH_3);
        case TAG_BLUETOOTH_4: value = GetVar(BLUETOOTH_4);
        case TAG_BLUETOOTH_5: value = GetVar(BLUETOOTH_5);
        case TAG_BLUETOOTH_6: value = GetVar(BLUETOOTH_6);
        case TAG_BLUETOOTH_7: value = GetVar(BLUETOOTH_7);
        case TAG_BLUETOOTH_8: value = GetVar(BLUETOOTH_8);
        case TAG_BLUETOOTH_9: value = GetVar(BLUETOOTH_9);
        case TAG_BLUETOOTH_10: value = GetVar(BLUETOOTH_10);
        case TAG_BLUETOOTH_11: value = GetVar(BLUETOOTH_11);
        case TAG_BLUETOOTH_12: value = GetVar(BLUETOOTH_12);
        case TAG_BLUETOOTH_13: value = GetVar(BLUETOOTH_13);
        case TAG_BLUETOOTH_14: value = GetVar(BLUETOOTH_14);
        case TAG_BLUETOOTH_15: value = GetVar(BLUETOOTH_15);
        case TAG_BLUETOOTH_16: value = GetVar(BLUETOOTH_16);
        case TAG_BLUETOOTH_17: value = GetVar(BLUETOOTH_17);
        case TAG_BLUETOOTH_18: value = GetVar(BLUETOOTH_18);
        case TAG_BLUETOOTH_19: value = GetVar(BLUETOOTH_19);
        case TAG_BLUETOOTH_20: value = GetVar(BLUETOOTH_20);
        case TAG_BLUETOOTH_21: value = GetVar(BLUETOOTH_21);
        case TAG_BLUETOOTH_22: value = GetVar(BLUETOOTH_22);
        case TAG_BLUETOOTH_23: value = GetVar(BLUETOOTH_23);
        case TAG_BLUETOOTH_24: value = GetVar(BLUETOOTH_24);
        case TAG_BLUETOOTH_25: value = GetVar(BLUETOOTH_25);
        case TAG_BLUETOOTH_26: value = GetVar(BLUETOOTH_26);
        case TAG_BLUETOOTH_27: value = GetVar(BLUETOOTH_27);
        case TAG_BLUETOOTH_28: value = GetVar(BLUETOOTH_28);
        case TAG_BLUETOOTH_29: value = GetVar(BLUETOOTH_29);
        case TAG_BLUETOOTH_30: value = GetVar(BLUETOOTH_30);
        case TAG_BLUETOOTH_31: value = GetVar(BLUETOOTH_31);
        case TAG_BLUETOOTH_32: value = GetVar(BLUETOOTH_32);
        case TAG_BLUETOOTH_33: value = GetVar(BLUETOOTH_33);
        case TAG_BLUETOOTH_34: value = GetVar(BLUETOOTH_34);
        case TAG_BLUETOOTH_35: value = GetVar(BLUETOOTH_35);
        case TAG_BLUETOOTH_36: value = GetVar(BLUETOOTH_36);
        case TAG_BLUETOOTH_37: value = GetVar(BLUETOOTH_37);
        case TAG_BLUETOOTH_38: value = GetVar(BLUETOOTH_38);
        case TAG_BLUETOOTH_39: value = GetVar(BLUETOOTH_39);
        case TAG_BLUETOOTH_40: value = GetVar(BLUETOOTH_40);
        case TAG_BLUETOOTH_41: value = GetVar(BLUETOOTH_41);
        case TAG_BLUETOOTH_42: value = GetVar(BLUETOOTH_42);
        case TAG_BLUETOOTH_43: value = GetVar(BLUETOOTH_43);
        case TAG_BLUETOOTH_44: value = GetVar(BLUETOOTH_44);
        case TAG_BLUETOOTH_45: value = GetVar(BLUETOOTH_45);
        case TAG_BLUETOOTH_46: value = GetVar(BLUETOOTH_46);
        case TAG_BLUETOOTH_47: value = GetVar(BLUETOOTH_47);
        case TAG_BLUETOOTH_48: value = GetVar(BLUETOOTH_48);
        case TAG_BLUETOOTH_49: value = GetVar(BLUETOOTH_49);
        case TAG_BLUETOOTH_50: value = GetVar(BLUETOOTH_50);
        case TAG_BLUETOOTH_51: value = GetVar(BLUETOOTH_51);
        case TAG_BLUETOOTH_52: value = GetVar(BLUETOOTH_52);
        case TAG_BLUETOOTH_53: value = GetVar(BLUETOOTH_53);
        case TAG_BLUETOOTH_54: value = GetVar(BLUETOOTH_54);
        case TAG_BLUETOOTH_55: value = GetVar(BLUETOOTH_55);
        case TAG_BLUETOOTH_56: value = GetVar(BLUETOOTH_56);
        case TAG_BLUETOOTH_57: value = GetVar(BLUETOOTH_57);
        case TAG_BLUETOOTH_58: value = GetVar(BLUETOOTH_58);
        case TAG_BLUETOOTH_59: value = GetVar(BLUETOOTH_59);
        case TAG_BLUETOOTH_60: value = GetVar(BLUETOOTH_60);
        case TAG_BLUETOOTH_61: value = GetVar(BLUETOOTH_61);
        case TAG_BLUETOOTH_62: value = GetVar(BLUETOOTH_62);
        case TAG_BLUETOOTH_63: value = GetVar(BLUETOOTH_63);
        
        case TAG_CAN8BITR_0: value = GetVar(CAN8BITR0);
        case TAG_CAN8BITR_1: value = GetVar(CAN8BITR1);
        case TAG_CAN8BITR_2: value = GetVar(CAN8BITR2);
        case TAG_CAN8BITR_3: value = GetVar(CAN8BITR3);
        case TAG_CAN8BITR_4: value = GetVar(CAN8BITR4);
        case TAG_CAN8BITR_5: value = GetVar(CAN8BITR5);
        case TAG_CAN8BITR_6: value = GetVar(CAN8BITR6);
        case TAG_CAN8BITR_7: value = GetVar(CAN8BITR7);
        case TAG_CAN8BITR_8: value = GetVar(CAN8BITR8);
        case TAG_CAN8BITR_9: value = GetVar(CAN8BITR9);
        case TAG_CAN8BITR_10: value = GetVar(CAN8BITR10);
        case TAG_CAN8BITR_11: value = GetVar(CAN8BITR11);
        case TAG_CAN8BITR_12: value = GetVar(CAN8BITR12);
        case TAG_CAN8BITR_13: value = GetVar(CAN8BITR13);
        case TAG_CAN8BITR_14: value = GetVar(CAN8BITR14);
        case TAG_CAN8BITR_15: value = GetVar(CAN8BITR15);
        case TAG_CAN8BITR_16: value = GetVar(CAN8BITR16);
        case TAG_CAN8BITR_17: value = GetVar(CAN8BITR17);
        case TAG_CAN8BITR_18: value = GetVar(CAN8BITR18);
        case TAG_CAN8BITR_19: value = GetVar(CAN8BITR19);
        case TAG_CAN8BITR_20: value = GetVar(CAN8BITR20);
        case TAG_CAN8BITR_21: value = GetVar(CAN8BITR21);
        case TAG_CAN8BITR_22: value = GetVar(CAN8BITR22);
        case TAG_CAN8BITR_23: value = GetVar(CAN8BITR23);
        case TAG_CAN8BITR_24: value = GetVar(CAN8BITR24);
        case TAG_CAN8BITR_25: value = GetVar(CAN8BITR25);
        case TAG_CAN8BITR_26: value = GetVar(CAN8BITR26);
        case TAG_CAN8BITR_27: value = GetVar(CAN8BITR27);
        case TAG_CAN8BITR_28: value = GetVar(CAN8BITR28);
        case TAG_CAN8BITR_29: value = GetVar(CAN8BITR29);
        case TAG_CAN8BITR_30: value = GetVar(CAN8BITR30);
        
        case TAG_CAN16BITR_0: value = GetVar(CAN16BITR0);
        case TAG_CAN16BITR_1: value = GetVar(CAN16BITR1);
        case TAG_CAN16BITR_2: value = GetVar(CAN16BITR2);
        case TAG_CAN16BITR_3: value = GetVar(CAN16BITR3);
        case TAG_CAN16BITR_4: value = GetVar(CAN16BITR4);
        case TAG_CAN16BITR_5: value = GetVar(CAN16BITR5);
        case TAG_CAN16BITR_6: value = GetVar(CAN16BITR6);
        case TAG_CAN16BITR_7: value = GetVar(CAN16BITR7);
        case TAG_CAN16BITR_8: value = GetVar(CAN16BITR8);
        case TAG_CAN16BITR_9: value = GetVar(CAN16BITR9);
        case TAG_CAN16BITR_10: value = GetVar(CAN16BITR10);
        case TAG_CAN16BITR_11: value = GetVar(CAN16BITR11);
        case TAG_CAN16BITR_12: value = GetVar(CAN16BITR12);
        case TAG_CAN16BITR_13: value = GetVar(CAN16BITR13);
        case TAG_CAN16BITR_14: value = GetVar(CAN16BITR14);

        case TAG_CAN32BITR_0: value = GetVar(CAN32BITR0);
        case TAG_CAN32BITR_1: value = GetVar(CAN32BITR1);
        case TAG_CAN32BITR_2: value = GetVar(CAN32BITR2);
        case TAG_CAN32BITR_3: value = GetVar(CAN32BITR3);
        case TAG_CAN32BITR_4: value = GetVar(CAN32BITR4);
        case TAG_CAN32BITR_5: value = GetVar(CAN32BITR5);
        case TAG_CAN32BITR_6: value = GetVar(CAN32BITR6);
        case TAG_CAN32BITR_7: value = GetVar(CAN32BITR7);
        case TAG_CAN32BITR_8: value = GetVar(CAN32BITR8);
        case TAG_CAN32BITR_9: value = GetVar(CAN32BITR9);
        case TAG_CAN32BITR_10: value = GetVar(CAN32BITR10);
        case TAG_CAN32BITR_11: value = GetVar(CAN32BITR11);
        case TAG_CAN32BITR_12: value = GetVar(CAN32BITR12);
        case TAG_CAN32BITR_13: value = GetVar(CAN32BITR13);
        case TAG_CAN32BITR_14: value = GetVar(CAN32BITR14);

        case TAG_MODBUS_0: value = GetVar(MODBUS_0);
        case TAG_MODBUS_1: value = GetVar(MODBUS_1);
        case TAG_MODBUS_2: value = GetVar(MODBUS_2);
        case TAG_MODBUS_3: value = GetVar(MODBUS_3);
        case TAG_MODBUS_4: value = GetVar(MODBUS_4);
        case TAG_MODBUS_5: value = GetVar(MODBUS_5);
        case TAG_MODBUS_6: value = GetVar(MODBUS_6);
        case TAG_MODBUS_7: value = GetVar(MODBUS_7);
        case TAG_MODBUS_8: value = GetVar(MODBUS_8);
        case TAG_MODBUS_9: value = GetVar(MODBUS_9);
        case TAG_MODBUS_10: value = GetVar(MODBUS_10);
        case TAG_MODBUS_11: value = GetVar(MODBUS_11);
        case TAG_MODBUS_12: value = GetVar(MODBUS_12);
        case TAG_MODBUS_13: value = GetVar(MODBUS_13);
        case TAG_MODBUS_14: value = GetVar(MODBUS_14);
        case TAG_MODBUS_15: value = GetVar(MODBUS_15);
        case TAG_MODBUS_16: value = GetVar(MODBUS_16);
        case TAG_MODBUS_17: value = GetVar(MODBUS_17);
        case TAG_MODBUS_18: value = GetVar(MODBUS_18);
        case TAG_MODBUS_19: value = GetVar(MODBUS_19);
        case TAG_MODBUS_20: value = GetVar(MODBUS_20);
        case TAG_MODBUS_21: value = GetVar(MODBUS_21);
        case TAG_MODBUS_22: value = GetVar(MODBUS_22);
        case TAG_MODBUS_23: value = GetVar(MODBUS_23);
        case TAG_MODBUS_24: value = GetVar(MODBUS_24);
        case TAG_MODBUS_25: value = GetVar(MODBUS_25);
        case TAG_MODBUS_26: value = GetVar(MODBUS_26);
        case TAG_MODBUS_27: value = GetVar(MODBUS_27);
        case TAG_MODBUS_28: value = GetVar(MODBUS_28);
        case TAG_MODBUS_29: value = GetVar(MODBUS_29);
        case TAG_MODBUS_30: value = GetVar(MODBUS_30);
        case TAG_MODBUS_31: value = GetVar(MODBUS_31);
        case TAG_MODBUS_32: value = GetVar(MODBUS_32);
        case TAG_MODBUS_33: value = GetVar(MODBUS_33);
        case TAG_MODBUS_34: value = GetVar(MODBUS_34);
        case TAG_MODBUS_35: value = GetVar(MODBUS_35);
        case TAG_MODBUS_36: value = GetVar(MODBUS_36);
        case TAG_MODBUS_37: value = GetVar(MODBUS_37);
        case TAG_MODBUS_38: value = GetVar(MODBUS_38);
        case TAG_MODBUS_39: value = GetVar(MODBUS_39);
        case TAG_MODBUS_40: value = GetVar(MODBUS_40);
        case TAG_MODBUS_41: value = GetVar(MODBUS_41);
        case TAG_MODBUS_42: value = GetVar(MODBUS_42);
        case TAG_MODBUS_43: value = GetVar(MODBUS_43);
        case TAG_MODBUS_44: value = GetVar(MODBUS_44);
        case TAG_MODBUS_45: value = GetVar(MODBUS_45);
        case TAG_MODBUS_46: value = GetVar(MODBUS_46);
        case TAG_MODBUS_47: value = GetVar(MODBUS_47);
        case TAG_MODBUS_48: value = GetVar(MODBUS_48);
        case TAG_MODBUS_49: value = GetVar(MODBUS_49);
        case TAG_MODBUS_50: value = GetVar(MODBUS_50);
        case TAG_MODBUS_51: value = GetVar(MODBUS_51);
        case TAG_MODBUS_52: value = GetVar(MODBUS_52);
        case TAG_MODBUS_53: value = GetVar(MODBUS_53);
        case TAG_MODBUS_54: value = GetVar(MODBUS_54);
        case TAG_MODBUS_55: value = GetVar(MODBUS_55);
        case TAG_MODBUS_56: value = GetVar(MODBUS_56);
        case TAG_MODBUS_57: value = GetVar(MODBUS_57);
        case TAG_MODBUS_58: value = GetVar(MODBUS_58);
        case TAG_MODBUS_59: value = GetVar(MODBUS_59);
        case TAG_MODBUS_60: value = GetVar(MODBUS_60);
        case TAG_MODBUS_61: value = GetVar(MODBUS_61);
        case TAG_MODBUS_62: value = GetVar(MODBUS_62);
        case TAG_MODBUS_63: value = GetVar(MODBUS_63);

        case TAG_USER_0: value = GetVar(USER_TAG0);
        case TAG_USER_1: value = GetVar(USER_TAG1);
        case TAG_USER_2: value = GetVar(USER_TAG2);
        case TAG_USER_3: value = GetVar(USER_TAG3);
        case TAG_USER_4: value = GetVar(USER_TAG4);
        case TAG_USER_5: value = GetVar(USER_TAG5);
        case TAG_USER_6: value = GetVar(USER_TAG6);
        case TAG_USER_7: value = GetVar(USER_TAG7);
        
        default: return false;
    }
    return true;
}

stock bool:hasExtPower(devStatus)
{
    return !((devStatus >>> DEVINFO_STATUS_BIT_EXTPOWERFAIL) & 1);
}

stock bool:isEngineOn(devStatus)
{
    return (devStatus >>> DEVINFO_STATUS_BIT_ENGINEON) & 1;
}

stock bool:getInStatus(index)
{
    switch (index)
    {
        case 0: return GetVar(STATUS_OF_IN0);
        case 1: return GetVar(STATUS_OF_IN1);
        case 2: return GetVar(STATUS_OF_IN2);
        case 3: return GetVar(STATUS_OF_IN3);
        case 4: return GetVar(STATUS_OF_IN4);
        case 5: return GetVar(STATUS_OF_IN5);
        case 6: return GetVar(STATUS_OF_IN6);
        case 7: return GetVar(STATUS_OF_IN7);
        case 8: return GetVar(STATUS_OF_IN8);
        case 9: return GetVar(STATUS_OF_IN9);
        default: return false;
    }
    return false;
}

stock bool:hasUserSpec(devModel, softMaj, softMin)
{
    return ((devModel == DEVINFO_MODEL_7X) && (((softMaj == 38) && (softMin >= 20)) || ((softMaj == 44) && (softMin >= 2)) || (softMaj > 44)))
            || (devModel >= DEVINFO_MODEL_10);
}

stock bool:getUserSpec(&userSpec)
{
    new cmdText{} = "USERSPEC";
    ExecCommand(cmdText);
    const bufMaxSize = 44;
    new buf{bufMaxSize};
    new const bufSize = GetBinaryDataFromCommand(buf, bufMaxSize);
    new pos = searchLinearStr(buf, bufSize, 'b', .fromEnd = true);
    new const cmdTextLength = strLen(cmdText);
    if (!isArraysEqualStr(buf, 0, min(cmdTextLength, bufSize), cmdText, 0, cmdTextLength) || (pos < 0) || (pos >= bufMaxSize))
        return false;

    userSpec = 0;
    pos--;
    for (new i = 0; pos >= 0; i++, pos--)
    {
        new const moduleOn = buf{pos} == SYMBOL_DIGIT_1;
        if (!moduleOn && (buf{pos} != SYMBOL_DIGIT_0))
            break;

        userSpec += moduleOn << i;
    }
    return true;
}

stock bool:isModuleOn(userSpec, userSpecBit)
{
    coerce(userSpecBit, 0, CELL_LAST_BIT_INDEX);
    return (userSpec >>> userSpecBit) & 1;
}
