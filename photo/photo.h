//! @file
//! @brief Заголовок библиотеки работы с фотографиями

#if defined PHOTO_H
#endinput
#endif
#define PHOTO_H

#include "..\jpeg\jpeg.h"
#include "..\jpeg\jfif\jfif.h"
#include "..\jpeg\exif\exif.h"

//! @defgroup savingResults Результаты сохранения фрагмента фото с метаинформацией
//! @{
#define PHOTO_METACHUNK_OK 1
#define PHOTO_METACHUNK_ERROR_SIZE -1
#define PHOTO_METACHUNK_ERROR_FORMAT -2
#define PHOTO_METACHUNK_ERROR_SAVE -3
//! @}

//! @defgroup ports Порты
//! @{
#define PHOTO_PORT_RS485_INDEX 2

#define PHOTO_PORT_MIN 0
#define PHOTO_PORT_MAX PHOTO_PORT_RS485_INDEX
//! @}

//! @defgroup path Структура пути файла
//! @{
#define PHOTO_PATH_BEGIN "Pic/RS"
#define PHOTO_PATH_BEGIN_LENGTH 6
#define PHOTO_PATH_PORT_232_TEXT "232"
#define PHOTO_PATH_PORT_485_TEXT "485"
#define PHOTO_PATH_PORT_TEXT_LENGTH 3
#define PHOTO_PATH_PORT_232_INDEX_LENGTH 1
#define PHOTO_PATH_DELIMETER_LENGTH 1
#define PHOTO_PATH_TIME_COMP_LENGTH 2
#define PHOTO_PATH_END ".jpg"
#define PHOTO_PATH_END_LENGTH 4
#define PHOTO_PATH_LENGTH (PHOTO_PATH_BEGIN_LENGTH + PHOTO_PATH_PORT_TEXT_LENGTH + PHOTO_PATH_PORT_232_INDEX_LENGTH + (2 * PHOTO_PATH_DELIMETER_LENGTH) + \
                            (6 * PHOTO_PATH_TIME_COMP_LENGTH) + PHOTO_PATH_END_LENGTH)
#define PHOTO_PATH_SIZE_W0 (PHOTO_PATH_LENGTH + 1)                        
//! @}

//! @brief Сгенерировать полное имя файла по порту и времени для сохранения фотографии
//! @param[in] portNum номер последовательного порта источника фото @ref ports
//! @param[out] path сгенерированный путь
//! @param[in] pathMaxSize максимальный размер пути файла, не менее PHOTO_PATH_SIZE_W0
//! @param[in] needGetUptime признак необходимости получения текущего времени
//! @param[in] unixTime метка времени Unix при needGetUptime == false
//! @return true - успешно, false - ошибка (недостаточная длина для пути или неправильное время)
forward bool:stock generatePhotoPath(portNum, path{}, pathMaxSize, bool:needGetUptime = true, unixTime = 0);

//! @brief Сохранить первую часть изображения с генерацией метаданных для Wialon
//! @param[inout] chunk исходные данные части изображения, изменяются при сохранении (используется в качестве временного буфера)
//! @param[in] chunkSizeIn исходная длина части изображения
//! @param[in] fileName имя файла для сохранения, должно оканчиваться \0
//! @param[in] maker производитель камеры, должно оканчиваться \0, не более PHOTO_METACHUNK_EXIF_MAKER_SIZE
//! @param[in] model модель камеры, должно оканчиваться \0, не более PHOTO_METACHUNK_EXIF_MODEL_SIZE
//! @param[out] chunkSizeOut общий размер сохраненных данных
//! @param[in] needGetTime признак необходимости получения текущего времени для фото
//! @param[in] unixTime метка времени Unix при needGetTime == false
//! @return @ref savingResults
forward stock photoSaveMetaChunk(chunk{}, chunkSizeIn, const fileName{}, const maker{} = "", const model{} = "", &chunkSizeOut = 0, bool:needGetTime = true,
                                unixTime = 0);
