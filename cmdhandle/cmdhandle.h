// Заголовок библиотеки обработчика ответов на команды

#ifndef CMDHANDLE_H
#define CMDHANDLE_H

#define CMDHANDLE_ANSWER_SIZE_MAX 254
#define CMDHANDLE_ANSWER_SIZE_MAX_W0 (CMDHANDLE_ANSWER_SIZE_MAX + 1)

#define CMDHANDLE_DATA [\
    .answer{CMDHANDLE_ANSWER_SIZE_MAX_W0},\
    .answerSize,\
]


cmdHandlerAddTextToAnswer(obj[CMDHANDLE_DATA], const text{}, textSize = 0);
cmdHandlerAddNumToAnswer(obj[CMDHANDLE_DATA], num);
cmdHandlerAddParValueToAnswer(obj[CMDHANDLE_DATA], const parName{}, parValue, parNameSize = 0);
cmdHandlerAddHexAsciiToAnswer(obj[CMDHANDLE_DATA], const data{}, dataSize);
cmdHandlerSendPreparedAnswer(obj[CMDHANDLE_DATA]);

#endif