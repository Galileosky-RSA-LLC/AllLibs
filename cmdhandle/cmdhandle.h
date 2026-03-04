//! @file
//! @brief Заголовок библиотеки обработчика команд

#if defined CMDHANDLE_H
#endinput
#endif
#define CMDHANDLE_H

#define CMD_LENGTH_MAX 255
#define CMD_LENGTH_MAX_W0 (CMD_LENGTH_MAX + 1)

stock const CMD_TEXT_SEPARATOR{} = " ";
stock const CMD_PARAM_SEPARATOR{} = ",";

#define CMDHANDLE_ANSWER_SIZE_MAX 254
#define CMDHANDLE_ANSWER_SIZE_MAX_W0 (CMDHANDLE_ANSWER_SIZE_MAX + 1)

#define CMDHANDLE_DATA [\
    .answer{CMDHANDLE_ANSWER_SIZE_MAX_W0},\
    .answerSize,\
]

//! @brief Добавить текст к ответу
forward stock cmdHandlerAddTextToAnswer(obj[CMDHANDLE_DATA], const text{}, textSize = 0);

//! @brief Добавить текстовое представление числа к ответу
forward stock cmdHandlerAddNumToAnswer(obj[CMDHANDLE_DATA], num);

//! @brief Добавить к ответу имя параметра и его числовое значение
forward stock cmdHandlerAddParValueToAnswer(obj[CMDHANDLE_DATA], const parName{}, parValue, parNameSize = 0);

//! @brief Добавить к ответу hex данные в строковом виде
forward stock cmdHandlerAddHexAsciiToAnswer(obj[CMDHANDLE_DATA], const data{}, dataSize);

//! @brief Послать подготовленный ранее ответ на команду
//! @param[inout] obj объект обработчика, ответ может быть изменен в зависимости от типа соединения источника команды
forward stock cmdHandlerSendPreparedAnswer(obj[CMDHANDLE_DATA]);
