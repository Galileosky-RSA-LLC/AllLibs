//! @file
//! @brief Заголовок библиотеки обработчика команд

#if defined CMDHANDLE_H
#endinput
#endif
#define CMDHANDLE_H

#define CMD_LENGTH_MAX 255
#define CMD_LENGTH_MAX_W0 (CMD_LENGTH_MAX + 1)

//! @defgroup cmdStruct Структура команды
//! @{
stock const CMD_TEXT_SEPARATOR{} = " ";
stock const CMD_PARAM_SEPARATOR{} = ",";
//! @}

#define CMDHANDLE_ANSWER_SIZE_MAX (CMD_LENGTH_MAX - 1)
#define CMDHANDLE_ANSWER_SIZE_MAX_W0 (CMDHANDLE_ANSWER_SIZE_MAX + 1)

#define CMDHANDLE_DATA [\
    .answer{CMDHANDLE_ANSWER_SIZE_MAX_W0},\
    .answerSize,\
]

//! @defgroup connectionTypes Типы соединений
//! @{
#define CMDHANDLE_CONNECTION_TYPE_UNKNOWN 0
#define CMDHANDLE_CONNECTION_TYPE_USB 1
#define CMDHANDLE_CONNECTION_TYPE_SMS 2
#define CMDHANDLE_CONNECTION_TYPE_SERVER 3
#define CMDHANDLE_CONNECTION_TYPE_BLE 4
//! @}

//! @brief Добавить текст к ответу
//! @param[inout] obj структура обработчика команд
//! @param[in] text добавляемый текст
//! @return true - успешно, false - ошибка (превышена длина ответа)
forward bool:stock cmdHandlerAddTextToAnswer(obj[CMDHANDLE_DATA], const text{});

//! @brief Добавить текстовое представление числа к ответу
//! @param[inout] obj структура обработчика команд
//! @param[in] num добавляемое число
//! @return true - успешно, false - ошибка (превышена длина ответа)
forward bool:stock cmdHandlerAddNumToAnswer(obj[CMDHANDLE_DATA], num);

//! @brief Добавить к ответу имя параметра и его числовое значение
//! @param[inout] obj структура обработчика команд
//! @param[in] parName наименование параметра
//! @param[in] parValue значение параметра
//! @return true - успешно, false - ошибка (превышена длина ответа)
forward bool:stock cmdHandlerAddParValueToAnswer(obj[CMDHANDLE_DATA], const parName{}, parValue);

//! @brief Добавить к ответу hex данные в строковом виде
//! @param[inout] obj структура обработчика команд
//! @param[in] data добавляемые данные
//! @param[in] dataSize размер добавляемых данных
//! @return true - успешно, false - ошибка (превышена длина ответа)
forward bool:stock cmdHandlerAddHexAsciiToAnswer(obj[CMDHANDLE_DATA], const data{}, dataSize);

//! @brief Послать подготовленный ранее ответ на команду с дополнительными бинарными данными
//! @param[inout] obj структура обработчика команд
//! @param[in] binaryData бинарные данные
//! @param[in] binaryDataSize размер бинарных данных
//! @param[in] useThreadConnection признак использования соединения текущего потока для отправки ответа на команду
//! @param[in] connectionType тип соединения при useThreadConnection==false
//! @param[in] connectionId идентификатор соединения при useThreadConnection==false
//! @param[in] commandNumber номер команды при useThreadConnection==false
forward stock cmdHandlerSendPreparedAnswer(obj[CMDHANDLE_DATA], const binaryData{} = "", binaryDataSize = 0, bool:useThreadConnection = true,
                                            connectionType = CMDHANDLE_CONNECTION_TYPE_UNKNOWN, connectionId = 0, commandNumber = 0);
