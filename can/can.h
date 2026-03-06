//! @file
//! @brief Заголовок библиотеки работы с CAN

#if defined CAN_H
#endinput
#endif
#define CAN_H

//! @defgroup portIndexes Индексы порта
//! @{
#define CAN_PORT_NUM_MIN 0
#define CAN_PORT_NUM_MAX 2
//! @}

//! @defgroup dataSizes Размеры данных
//! @{
#define CAN_DATASIZE_GENERAL_MAX 8
#define CAN_DATASIZE_FD_MAX 64
//! @}

//! @defgroup portModes Режимы порта
//! @{
#define CAN_MODE_OFF 0
#define CAN_MODE_SCAN 1
#define CAN_MODE_FMS_STD 2
#define CAN_MODE_J1939_CUSTOM 3
#define CAN_MODE_J1979_STD_29BIT 5
#define CAN_MODE_J1979_STD_11BIT 6
#define CAN_MODE_J1979_CUSTOM_29BIT 8
#define CAN_MODE_J1979_CUSTOM_11BIT 9
#define CAN_MODE_J1979_CHECK_PIDS 10
#define CAN_MODE_EASY_LOGIC 11
#define CAN_MODE_CAN_SCANNER 12
#define CAN_MODE_J1939DA_SENDING 13
#define CAN_MODE_J1939DA_RECEIVING 14
//! @}

//! @defgroup recLogStruct Структура лога формата .rec

// Структура в Конфигураторе:
// struct attribute((packed)) ConfRecord
// {
// uint32_t timestamp;
// uint32_t id;
// uint8_t is29bit;
// uint32_t msg_length;
// uint8_t msg[8];
// uint8_t clr_spec;
// uint16_t clr[5];
// };

//! @defgroup recStruct Структура записи формата .rec
//! @ingroup recLogStruct
//! @{
#define CAN_REC_TIMESTAMP_POS 0
#define CAN_REC_TIMESTAMP_SIZE 4
#define CAN_REC_ID_POS (CAN_REC_TIMESTAMP_POS + CAN_REC_TIMESTAMP_SIZE)
#define CAN_REC_ID_SIZE 4
#define CAN_REC_ISLONG_POS (CAN_REC_ID_POS + CAN_REC_ID_SIZE)
#define CAN_REC_ISLONG_SIZE 1
#define CAN_REC_MSGLEN_POS (CAN_REC_ISLONG_POS + CAN_REC_ISLONG_SIZE)
#define CAN_REC_MSGLEN_SIZE 4
#define CAN_REC_MSG_POS (CAN_REC_MSGLEN_POS + CAN_REC_MSGLEN_SIZE)
#define CAN_REC_MSG_SIZE_MIN 0
#define CAN_REC_MSG_SIZE_MAX 8
#define CAN_REC_COLORSPEC_ID1 1
#define CAN_REC_COLORSPEC_ID2 2
#define CAN_REC_COLORSPEC_SIZE 1
#define CAN_REC_COLOR_SIZE 10
#define CAN_REC_SIZE_MIN (CAN_REC_TIMESTAMP_SIZE + CAN_REC_ID_SIZE + CAN_REC_ISLONG_SIZE + CAN_REC_MSGLEN_SIZE + CAN_REC_MSG_SIZE_MIN + \
                            CAN_REC_COLORSPEC_SIZE + CAN_REC_COLOR_SIZE)
#define CAN_REC_SIZE_MAX (CAN_REC_SIZE_MIN - CAN_REC_MSG_SIZE_MIN + CAN_REC_MSG_SIZE_MAX)
//! @}

//! @addtogroup recLogStruct
//! @{
#define CAN_REC_FILE_EXTENSION ".rec"
#define CAN_REC_FILE_EXTENSION_SIZE 4
//! @}

//! @defgroup rec2logStruct Структура лога формата .rec2

// Структура в Конфигураторе:
// Первая запись - заголовок
// Структура заголовка:
// 2 байта - magic = 0xBEC2
// 2 байта - версия
// 4 байта - размер данных
// 120 байт - комментарий
// 2 байта - crc16 данных
// Вторая и последующие записи - пакеты с данными
// Структура пакета:
// 4 байта - временнАя метка в миллисекундах от 00:00
// 1 байт - номер шины
// 1 байт - флаги (0 - 29bit, 1 - FD, 2 - brs, 3 - esi, 4..7 - reserved)
// 4 байт - PID
// 1 байт - размер данных N
// N байт - сырые данные
// 4 байта - информация по цвету

//! @defgroup rec2struct Структура записи формата .rec2
//! @ingroup rec2logStruct
//! @{
#define CAN_REC2_HDR_MAGIC_POS 0
#define CAN_REC2_HDR_MAGIC_SIZE 2
#define CAN_REC2_HDR_MAGIC "\xBE\xC2"
#define CAN_REC2_HDR_VER_POS (CAN_REC2_HDR_MAGIC_POS + CAN_REC2_HDR_MAGIC_SIZE)
#define CAN_REC2_HDR_VER_SIZE 2
#define CAN_REC2_HDR_DATALEN_POS (CAN_REC2_HDR_VER_POS + CAN_REC2_HDR_VER_SIZE)
#define CAN_REC2_HDR_DATALEN_SIZE 4
#define CAN_REC2_HDR_COMMENT_POS (CAN_REC2_HDR_DATALEN_POS + CAN_REC2_HDR_DATALEN_SIZE)
#define CAN_REC2_HDR_COMMENT_SIZE 120
#define CAN_REC2_HDR_CC_POS (CAN_REC2_HDR_COMMENT_POS + CAN_REC2_HDR_COMMENT_SIZE)
#define CAN_REC2_HDR_CC_SIZE 2
#define CAN_REC2_HDR_SIZE (CAN_REC2_HDR_MAGIC_SIZE + CAN_REC2_HDR_VER_SIZE + CAN_REC2_HDR_DATALEN_SIZE + CAN_REC2_HDR_COMMENT_SIZE + CAN_REC2_HDR_CC_SIZE)
#define CAN_REC2_PACK_POS CAN_REC2_HDR_SIZE
#define CAN_REC2_PACK_TIMESTAMP_POS 0
#define CAN_REC2_PACK_TIMESTAMP_SIZE 4
#define CAN_REC2_PACK_BUS_POS (CAN_REC2_PACK_TIMESTAMP_POS + CAN_REC2_PACK_TIMESTAMP_SIZE)
#define CAN_REC2_PACK_BUS_SIZE 1
#define CAN_REC2_PACK_FLAGS_POS (CAN_REC2_PACK_BUS_POS + CAN_REC2_PACK_BUS_SIZE)
#define CAN_REC2_PACK_FLAGS_SIZE 1
#define CAN_REC2_PACK_FLAG_ID_29BIT_MASK 0b1
#define CAN_REC2_PACK_FLAG_FD_MASK 0b10
#define CAN_REC2_PACK_FLAG_BRS_MASK 0b100
#define CAN_REC2_PACK_FLAG_ESI_MASK 0b1000
#define CAN_REC2_PACK_ID_POS (CAN_REC2_PACK_FLAGS_POS + CAN_REC2_PACK_FLAGS_SIZE)
#define CAN_REC2_PACK_ID_SIZE 4
#define CAN_REC2_PACK_DATALEN_POS (CAN_REC2_PACK_ID_POS + CAN_REC2_PACK_ID_SIZE)
#define CAN_REC2_PACK_DATALEN_SIZE 1
#define CAN_REC2_PACK_DATALEN_MIN 0
#define CAN_REC2_PACK_DATA_POS (CAN_REC2_PACK_DATALEN_POS + CAN_REC2_PACK_DATALEN_SIZE)
#define CAN_REC2_PACK_COLOR_SIZE 4
#define CAN_REC2_PACK_SIZE_MIN (CAN_REC2_PACK_TIMESTAMP_SIZE + CAN_REC2_PACK_COLOR_SIZE + CAN_REC2_PACK_BUS_SIZE + CAN_REC2_PACK_FLAGS_SIZE + \
                                CAN_REC2_PACK_ID_SIZE + CAN_REC2_PACK_DATALEN_SIZE + CAN_REC2_PACK_DATALEN_MIN)
//! @}

//! @addtogroup rec2logStruct
//! @{                                
#define CAN_REC2_FILE_EXTENSION ".rec2"
#define CAN_REC2_FILE_EXTENSION_SIZE 5
//! @}

//! @brief Вывести в диагностику сообщение CAN
//! @param[in] mes сообщение CAN
//! @param[in] portNum индекс порта
//! @param[in] send признак отправки: 0 - принятое сообщение, 1 - отправляемое сообщение
forward stock canDiagMessage(const mes[CANMSG], portNum, bool:isSend);

//! @brief Вывести в диагностику отправляемое сообщение CAN
//! @param[in] mes сообщение CAN
//! @param[in] portNum индекс порта
forward stock canDiagSentMessage(const mes[CANMSG], portNum);

//! @brief Вывести в диагностику принятое сообщение CAN
//! @param[in] mes сообщение CAN
//! @param[in] portNum индекс порта
forward stock canDiagReceivedMessage(const mes[CANMSG], portNum);

//! @brief Определить текущие параметры CAN порта
//! @param[in] portNum индекс порта
//! @param[out] mode режим порта @ref portModes при успешном возврате
//! @param[out] baudRate установленная скорость передачи данных при успешном возврате, бит/с
//! @return true - успешно, false - ошибка
forward bool:stock getCanRegime(portNum, &mode, &baudRate);
