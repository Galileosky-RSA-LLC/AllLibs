#if defined BLUETOOTH_H
#endinput
#endif
#define BLUETOOTH_H

//! @file
//! @brief Заголовок библиотеки для работы с Bluetooth

#define BLUETOOTH_MAC_SIZE 6

#define BLUETOOTH_RSSI_MASK 0xFFFFFF00

#define BLUETOOTH_ADV_DATA_SIZE_MAX 32

//! @defgroup adStruct AD структура
//! @{
#define BLUETOOTH_ADSTRUCT_LEN_POS 0
#define BLUETOOTH_ADSTRUCT_LEN_SIZE 1
#define BLUETOOTH_ADSTRUCT_LEN_MIN 2
#define BLUETOOTH_ADSTRUCT_TYPE_POS (BLUETOOTH_ADSTRUCT_LEN_POS + BLUETOOTH_ADSTRUCT_LEN_SIZE)
#define BLUETOOTH_ADSTRUCT_TYPE_SIZE 1
#define BLUETOOTH_ADSTRUCT_PAYLOAD_POS (BLUETOOTH_ADSTRUCT_TYPE_POS + BLUETOOTH_ADSTRUCT_TYPE_SIZE)
#define BLUETOOTH_ADSTRUCT_DATA_SIZE_MIN 1
#define BLUETOOTH_ADSTRUCT_SIZE_MIN (BLUETOOTH_ADSTRUCT_LEN_MIN + BLUETOOTH_ADSTRUCT_LEN_SIZE)
#define BLUETOOTH_ADSTRUCT_DATA_SIZE_MAX (BLUETOOTH_ADV_DATA_SIZE_MAX - BLUETOOTH_ADSTRUCT_SIZE_MIN)
#define BLUETOOTH_ADSTRUCT [\
    .type,\
    .dataSize,\
    .data{BLUETOOTH_ADSTRUCT_DATA_SIZE_MAX}\
]
//! @}

//! @defgroup adStructTypes Типы данных AD структур
//! @ingroup adStruct
//! @{
#define BLUETOOTH_ADTYPE_SHORT_LOCAL_NAME 0x08
#define BLUETOOTH_ADTYPE_COMPL_LOCAL_NAME 0x09
#define BLUETOOTH_ADTYPE_SRVCDATA_UUID16BIT 0x16
#define BLUETOOTH_ADTYPE_MFRSPECDATA 0xFF
//! @}

//! @defgroup adStructs AD структуры
//! @ingroup adStruct

//! @defgroup adStructsSrvcDataUuid16 Service Data 16 bit 
//! @ingroup adStructs
//! @{
#define BLUETOOTH_ADTYPE_SRVCDATA_UUID16BIT_UUID_POS 0
#define BLUETOOTH_ADTYPE_SRVCDATA_UUID16BIT_SIZE 2
//! @}

//! @defgroup adStructsMfrSpecData Manufactured specific data
//! @ingroup adStructs
//! @{
#define BLUETOOTH_MFRSPECDATA_COMP_ID_POS 0
#define BLUETOOTH_MFRSPECDATA_COMP_ID_SIZE 2
//! @}

//! @defgroup bleApiReturns Коды возврата BLE API
//! @{
#define BLE_SUCCESS 1 //!< >=1
#define BLE_ERROR_TIMEOUT 0
#define BLE_ERROR_ARGS -1
#define BLE_ERROR_STATE -2
#define BLE_ERROR_GENERAL -3
#define BLE_ERROR_REJECT -4
//! @}

//! @brief Получить RSSI устройства, передавшего сообщение
forward stock bluetoothGetRssi(const msg[BTMSG]);

//! @brief Получить AD Structure из рекламного сообщения
//! @param[in] msg принятое рекламное сообщение
//! @param[in] pos позиция начала структуры в данных рекламного сообщения
//! @param[out] adStruct AD структура
//! @return общий размер AD struct (0 - ошибка)
forward stock bluetoothGetAdStruct(const msg[BTMSG], pos, adStruct[BLUETOOTH_ADSTRUCT]);

//! @brief Получить имя
//! @param[in] msg принятое рекламное сообщение
//! @param[in] nameMaxSize максимальная длина для имени
//! @param[out] name полученное имя
//! @return true - успешно, false - ошибка
forward stock bluetoothGetLocalName(const msg[BTMSG], nameMaxSize, name{});

//! @brief Получить полное имя
//! @param[in] msg принятое рекламное сообщение
//! @param[in] nameMaxSize максимальная длина для имени
//! @param[out] name полученное имя
//! @return true - успешно, false - ошибка
forward stock bluetoothGetComplName(const msg[BTMSG], nameMaxSize, name{});

//! @brief Получить короткое имя
//! @param[in] msg принятое рекламное сообщение
//! @param[in] nameMaxSize максимальная длина для имени
//! @param[out] name полученное имя
//! @return true - успешно, false - ошибка
forward stock bluetoothGetShortlName(const msg[BTMSG], nameMaxSize, name{});

//! @brief Получить AD структуру заданного типа из рекламного сообщения
//! @param[in] msg принятое рекламное сообщение
//! @param[in] type искомый тип структуры
//! @param[out] adStruct найденная AD структура
//! @return true - найдена, false - не найдена
forward stock bluetoothGetAdStructType(const msg[BTMSG], type, adStruct[BLUETOOTH_ADSTRUCT]);

//! @brief Получить Company Id из AD Struct
//! @param[in] adStruct AD Struct c Manufacturer Specific Data
//! @param[out] result полученный идентификатор
//! @return true - успешно, false - ошибка
bluetoothGetCompanyId(const adStruct[BLUETOOTH_ADSTRUCT], &result);
