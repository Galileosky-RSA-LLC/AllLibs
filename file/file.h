//! @file
//! @brief Заголовок библиотеки для работы с файлами

#ifdef FILE_H
#endinput
#endif
#define FILE_H

#define FILE_FULL_PATH_SIZE_MAX 255
#define FILE_FULL_PATH_SIZE_MAX_W0 (FILE_FULL_PATH_SIZE_MAX + 1)

//! @defgroup reservedFiles Зарезервированные файлы
//! @{

#define FILE_TIME_ZONE "time_zone" // файл с текущим часовым поясом в строковом виде: "-5" -> UTC-5

//! @}

//! @brief Переименовать файл 
//! @details с использованием вызова команды FSMOVE
//! @param[in] src исходное имя файла, должно оканчиваться \0
//! @param[in] dest требуемое имя файла, должно оканчиваться \0
//! @return false - ошибка, true - успешно
forward stock fileRename(const src{}, const dest{});

//! @brief Записать в файл буфер с произвольной позиции
//! @param[in] fileName имя файла, должно оканчиваться \0
//! @param[in] buf записываемый буфер
//! @param[in] bufSize размер буфера
//! @param[in] fileOffset смещение в файле (-1 - запись в конец)
//! @param[in] bufPos начало записываемой части буфера
//! @return количество записанных байт
forward stock fileWriteWrap(const fileName{}, const buf{}, bufSize, fileOffset, bufPos = 0);
