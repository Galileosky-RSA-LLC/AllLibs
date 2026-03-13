//! @file
//! @brief Общие определения

#if defined GDEFINES_H
#endinput
#endif
#define GDEFINES_H

//! @defgroup tagIds Идентификаторы тегов

//! @defgroup canTagIds CAN теги
//! @ingroup tagIds
//! @{
#define TAG_CAN8BITR_0 0x02
#define TAG_CAN8BITR_1 0x03
#define TAG_CAN8BITR_2 0x04
#define TAG_CAN8BITR_3 0x05
#define TAG_CAN8BITR_4 0x06
#define TAG_CAN8BITR_5 0x07
#define TAG_CAN8BITR_6 0x08
#define TAG_CAN8BITR_7 0x09
#define TAG_CAN8BITR_8 0x0A
#define TAG_CAN8BITR_9 0x0B

#define TAG_CAN8BITR_10 0x0C
#define TAG_CAN8BITR_11 0x0D
#define TAG_CAN8BITR_12 0x0E
#define TAG_CAN8BITR_13 0x0F
#define TAG_CAN8BITR_14 0x10
#define TAG_CAN8BITR_15 0x3D
#define TAG_CAN8BITR_16 0x3E
#define TAG_CAN8BITR_17 0x3F
#define TAG_CAN8BITR_18 0x40
#define TAG_CAN8BITR_19 0x41

#define TAG_CAN8BITR_20 0x42
#define TAG_CAN8BITR_21 0x43
#define TAG_CAN8BITR_22 0x44
#define TAG_CAN8BITR_23 0x45
#define TAG_CAN8BITR_24 0x46
#define TAG_CAN8BITR_25 0x47
#define TAG_CAN8BITR_26 0x48
#define TAG_CAN8BITR_27 0x49
#define TAG_CAN8BITR_28 0x4A
#define TAG_CAN8BITR_29 0x4B
#define TAG_CAN8BITR_30 0x4C

#define TAG_CAN16BITR_0 0x11
#define TAG_CAN16BITR_1 0x12
#define TAG_CAN16BITR_2 0x13
#define TAG_CAN16BITR_3 0x14
#define TAG_CAN16BITR_4 0x15
#define TAG_CAN16BITR_5 0x4D
#define TAG_CAN16BITR_6 0x4E
#define TAG_CAN16BITR_7 0x4F
#define TAG_CAN16BITR_8 0x50
#define TAG_CAN16BITR_9 0x51
#define TAG_CAN16BITR_10 0x52
#define TAG_CAN16BITR_11 0x53
#define TAG_CAN16BITR_12 0x54
#define TAG_CAN16BITR_13 0x55
#define TAG_CAN16BITR_14 0x56

#define TAG_CAN32BITR_0 0x16
#define TAG_CAN32BITR_1 0x17
#define TAG_CAN32BITR_2 0x18
#define TAG_CAN32BITR_3 0x19
#define TAG_CAN32BITR_4 0x1A
#define TAG_CAN32BITR_5 0x57
#define TAG_CAN32BITR_6 0x58
#define TAG_CAN32BITR_7 0x59
#define TAG_CAN32BITR_8 0x5A
#define TAG_CAN32BITR_9 0x5B
#define TAG_CAN32BITR_10 0x5C
#define TAG_CAN32BITR_11 0x5D
#define TAG_CAN32BITR_12 0x5E
#define TAG_CAN32BITR_13 0x5F
#define TAG_CAN32BITR_14 0x60

#define TAG_CAN_A_0 0x6C
#define TAG_CAN_A_1 0x6D
#define TAG_CAN_B_0 0x6E
#define TAG_CAN_B_1 0x6F
//! @}

//! @defgroup userTagIds Числовые пользовательские теги
//! @ingroup tagIds
//! @{
#define TAG_USER_0 0x62
#define TAG_USER_1 0x63
#define TAG_USER_2 0x64
#define TAG_USER_3 0x65
#define TAG_USER_4 0x66
#define TAG_USER_5 0x67
#define TAG_USER_6 0x68
#define TAG_USER_7 0x69
//! @}

//! @defgroup btTagIds Bluetooth теги
//! @ingroup tagIds
//! @{
#define TAG_BLUETOOTH_0 0x70
#define TAG_BLUETOOTH_1 0x71
#define TAG_BLUETOOTH_2 0x72
#define TAG_BLUETOOTH_3 0x73
#define TAG_BLUETOOTH_4 0x74
#define TAG_BLUETOOTH_5 0x75
#define TAG_BLUETOOTH_6 0x76
#define TAG_BLUETOOTH_7 0x77
#define TAG_BLUETOOTH_8 0x78
#define TAG_BLUETOOTH_9 0x79

#define TAG_BLUETOOTH_10 0x7A
#define TAG_BLUETOOTH_11 0x7B
#define TAG_BLUETOOTH_12 0x7C
#define TAG_BLUETOOTH_13 0x7D
#define TAG_BLUETOOTH_14 0x7E
#define TAG_BLUETOOTH_15 0x7F
#define TAG_BLUETOOTH_16 0x80
#define TAG_BLUETOOTH_17 0x81
#define TAG_BLUETOOTH_18 0x82
#define TAG_BLUETOOTH_19 0x83

#define TAG_BLUETOOTH_20 0x84
#define TAG_BLUETOOTH_21 0x85
#define TAG_BLUETOOTH_22 0x86
#define TAG_BLUETOOTH_23 0x87
#define TAG_BLUETOOTH_24 0x88
#define TAG_BLUETOOTH_25 0x89
#define TAG_BLUETOOTH_26 0x8A
#define TAG_BLUETOOTH_27 0x8B
#define TAG_BLUETOOTH_28 0x8C
#define TAG_BLUETOOTH_29 0x8D

#define TAG_BLUETOOTH_30 0x8E
#define TAG_BLUETOOTH_31 0x8F
#define TAG_BLUETOOTH_32 0x90
#define TAG_BLUETOOTH_33 0x91
#define TAG_BLUETOOTH_34 0x92
#define TAG_BLUETOOTH_35 0x93
#define TAG_BLUETOOTH_36 0x94
#define TAG_BLUETOOTH_37 0x95
#define TAG_BLUETOOTH_38 0x96
#define TAG_BLUETOOTH_39 0x97

#define TAG_BLUETOOTH_40 0x98
#define TAG_BLUETOOTH_41 0x99
#define TAG_BLUETOOTH_42 0x9A
#define TAG_BLUETOOTH_43 0x9B
#define TAG_BLUETOOTH_44 0x9C
#define TAG_BLUETOOTH_45 0x9D
#define TAG_BLUETOOTH_46 0x9E
#define TAG_BLUETOOTH_47 0x9F
#define TAG_BLUETOOTH_48 0xA0
#define TAG_BLUETOOTH_49 0xA1

#define TAG_BLUETOOTH_50 0xA2
#define TAG_BLUETOOTH_51 0xA3
#define TAG_BLUETOOTH_52 0xA4
#define TAG_BLUETOOTH_53 0xA5
#define TAG_BLUETOOTH_54 0xA6
#define TAG_BLUETOOTH_55 0xA7
#define TAG_BLUETOOTH_56 0xA8
#define TAG_BLUETOOTH_57 0xA9
#define TAG_BLUETOOTH_58 0xAA
#define TAG_BLUETOOTH_59 0xAB

#define TAG_BLUETOOTH_60 0xAC
#define TAG_BLUETOOTH_61 0xAD
#define TAG_BLUETOOTH_62 0xAE
#define TAG_BLUETOOTH_63 0xAF
//! @}

//! @defgroup rs232tagIds Теги портов RS-232
//! @ingroup tagIds
//! @{
#define TAG_RS232_0_VALUE 0x00
#define TAG_RS232_1_VALUE 0x01

#define TAG_RS232_0_TEMP 0x2B
#define TAG_RS232_1_TEMP 0x2C
//! @}

//! @defgroup rs485tagIds Теги порта RS-485
//! @ingroup tagIds
//! @{
#define TAG_RS485_FLS0_VALUE 0x1B
#define TAG_RS485_FLS1_VALUE 0x1C
#define TAG_RS485_FLS2_VALUE 0x1D
#define TAG_RS485_FLS3_VALUE 0x1E
#define TAG_RS485_FLS4_VALUE 0x1F
#define TAG_RS485_FLS5_VALUE 0x20
#define TAG_RS485_FLS6_VALUE 0x21
#define TAG_RS485_FLS7_VALUE 0x22
#define TAG_RS485_FLS8_VALUE 0x23
#define TAG_RS485_FLS9_VALUE 0x24
#define TAG_RS485_FLS10_VALUE 0x25
#define TAG_RS485_FLS11_VALUE 0x26
#define TAG_RS485_FLS12_VALUE 0x27
#define TAG_RS485_FLS13_VALUE 0x28
#define TAG_RS485_FLS14_VALUE 0x29
#define TAG_RS485_FLS15_VALUE 0x2A

#define TAG_RS485_FLS0_TEMP 0x2D
#define TAG_RS485_FLS1_TEMP 0x2E
#define TAG_RS485_FLS2_TEMP 0x2F
#define TAG_RS485_FLS3_TEMP 0x30
#define TAG_RS485_FLS4_TEMP 0x31
#define TAG_RS485_FLS5_TEMP 0x32
#define TAG_RS485_FLS6_TEMP 0x33
#define TAG_RS485_FLS7_TEMP 0x34
#define TAG_RS485_FLS8_TEMP 0x35
#define TAG_RS485_FLS9_TEMP 0x36
#define TAG_RS485_FLS10_TEMP 0x37
#define TAG_RS485_FLS11_TEMP 0x38
#define TAG_RS485_FLS12_TEMP 0x39
#define TAG_RS485_FLS13_TEMP 0x3A
#define TAG_RS485_FLS14_TEMP 0x3B
#define TAG_RS485_FLS15_TEMP 0x3C
//! @}

//! @defgroup arrayTagIds Теги массивов
//! @ingroup tagIds
//! @{
#define TAG_TPMS 0x61
#define TAG_USER_ARRAY 0x6A
#define TAG_REFRIGERATOR 0x6B
//! @}

//! @defgroup modbusTagIds Теги Modbus
//! @ingroup tagIds
//! @{
#define TAG_MODBUS_0 0xB0
#define TAG_MODBUS_1 0xB1
#define TAG_MODBUS_2 0xB2
#define TAG_MODBUS_3 0xB3
#define TAG_MODBUS_4 0xB4
#define TAG_MODBUS_5 0xB5
#define TAG_MODBUS_6 0xB6
#define TAG_MODBUS_7 0xB7
#define TAG_MODBUS_8 0xB8
#define TAG_MODBUS_9 0xB9

#define TAG_MODBUS_10 0xBA
#define TAG_MODBUS_11 0xBB
#define TAG_MODBUS_12 0xBC
#define TAG_MODBUS_13 0xBD
#define TAG_MODBUS_14 0xBE
#define TAG_MODBUS_15 0xBF
#define TAG_MODBUS_16 0xC0
#define TAG_MODBUS_17 0xC1
#define TAG_MODBUS_18 0xC2
#define TAG_MODBUS_19 0xC3

#define TAG_MODBUS_20 0xC4
#define TAG_MODBUS_21 0xC5
#define TAG_MODBUS_22 0xC6
#define TAG_MODBUS_23 0xC7
#define TAG_MODBUS_24 0xC8
#define TAG_MODBUS_25 0xC9
#define TAG_MODBUS_26 0xCA
#define TAG_MODBUS_27 0xCB
#define TAG_MODBUS_28 0xCC
#define TAG_MODBUS_29 0xCD

#define TAG_MODBUS_30 0xCE
#define TAG_MODBUS_31 0xCF
#define TAG_MODBUS_32 0xD0
#define TAG_MODBUS_33 0xD1
#define TAG_MODBUS_34 0xD2
#define TAG_MODBUS_35 0xD3
#define TAG_MODBUS_36 0xD4
#define TAG_MODBUS_37 0xD5
#define TAG_MODBUS_38 0xD6
#define TAG_MODBUS_39 0xD7

#define TAG_MODBUS_40 0xD8
#define TAG_MODBUS_41 0xD9
#define TAG_MODBUS_42 0xDA
#define TAG_MODBUS_43 0xDB
#define TAG_MODBUS_44 0xDC
#define TAG_MODBUS_45 0xDD
#define TAG_MODBUS_46 0xDE
#define TAG_MODBUS_47 0xDF
#define TAG_MODBUS_48 0xE0
#define TAG_MODBUS_49 0xE1

#define TAG_MODBUS_50 0xE2
#define TAG_MODBUS_51 0xE3
#define TAG_MODBUS_52 0xE4
#define TAG_MODBUS_53 0xE5
#define TAG_MODBUS_54 0xE6
#define TAG_MODBUS_55 0xE7
#define TAG_MODBUS_56 0xE8
#define TAG_MODBUS_57 0xE9
#define TAG_MODBUS_58 0xEA
#define TAG_MODBUS_59 0xEB

#define TAG_MODBUS_60 0xEC
#define TAG_MODBUS_61 0xED
#define TAG_MODBUS_62 0xEE
#define TAG_MODBUS_63 0xEF
//! @}

//! @defgroup wheelTagIds Колесные теги
//! @ingroup tagIds
//! @{
#define TAG_WHEEL_0 0x100
#define TAG_WHEEL_1 0x101
#define TAG_WHEEL_2 0x102
#define TAG_WHEEL_3 0x103
#define TAG_WHEEL_4 0x104
#define TAG_WHEEL_5 0x105
#define TAG_WHEEL_6 0x106
#define TAG_WHEEL_7 0x107
#define TAG_WHEEL_8 0x108
#define TAG_WHEEL_9 0x109

#define TAG_WHEEL_10 0x10A
#define TAG_WHEEL_11 0x10B
#define TAG_WHEEL_12 0x10C
#define TAG_WHEEL_13 0x10D
#define TAG_WHEEL_14 0x10E
#define TAG_WHEEL_15 0x10F
#define TAG_WHEEL_16 0x110
#define TAG_WHEEL_17 0x111
#define TAG_WHEEL_18 0x112
#define TAG_WHEEL_19 0x113

#define TAG_WHEEL_20 0x114
#define TAG_WHEEL_21 0x115
#define TAG_WHEEL_22 0x116
#define TAG_WHEEL_23 0x117
#define TAG_WHEEL_24 0x118
#define TAG_WHEEL_25 0x119
#define TAG_WHEEL_26 0x11A
#define TAG_WHEEL_27 0x11B
#define TAG_WHEEL_28 0x11C
#define TAG_WHEEL_29 0x11D

#define TAG_WHEEL_30 0x11E
#define TAG_WHEEL_31 0x11F
#define TAG_WHEEL_32 0x120
#define TAG_WHEEL_33 0x121
//! @}

//! @defgroup iButtonTagIds Теги iButton
//! @ingroup tagIds
//! @{
#define TAG_IBUTTON_0 0xF0
#define TAG_IBUTTON_1 0xF1
//! @}

//! @defgroup tagSizes Размеры тегов
//! @{
#define TAG_TPMS_SIZE 72

#define TAG_USER_ARRAY_MAX_SIZE 180

#define TAG_REFRIGERATOR_MIN_SIZE 7
#define TAG_REFRIGERATOR_MAX_SIZE 117
//! @}

//! @defgroup inputs Дискретно-аналоговые входы
//! @{
#define ADC_INPUTS_AMOUNT_MAX 10
#define ADC_INPUT_IDX_MIN 0
#define ADC_INPUT_IDX_MAX (ADC_INPUTS_AMOUNT_MAX - 1)
//! @}
