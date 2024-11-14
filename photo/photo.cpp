#ifndef PHOTO_LIB
#define PHOTO_LIB

#include "photo.h"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "..\time\time.h"
#include "..\time\time.cpp"
#include "..\jpeg\jpeg.h"
#include "..\jpeg\jfif\jfif.h"
#include "..\jpeg\exif\exif.h"
#include "..\string\string.h"
#include "..\string\string.cpp"

//! Сгенерировать полное имя файла по порту и времени для сохранения фотографии
//! \param[in] portNum номер последовательного порта источника фото
//! \param[out] path сгенерированный путь
//! \param[in] pathMaxSize максимальный размер пути файла, не менее PHOTO_PATH_SIZE
//! \return true - успешно, false - ошибка (недостаточная длина для пути)
generatePhotoPath(portNum, path{}, pathMaxSize)
{
    if (pathMaxSize < PHOTO_PATH_SIZE)
        return false;

    if (portNum < PHOTO_PORT_MIN)
        portNum = PHOTO_PORT_MIN;

    if (portNum > PHOTO_PORT_MAX)
        portNum = PHOTO_PORT_MAX;

    new pos = insertArrayStr(path, 0, pathMaxSize, PHOTO_PATH_BEGIN, PHOTO_PATH_BEGIN_SIZE);
    pos += insertArrayStr(path, pos, pathMaxSize, portNum == PHOTO_PORT_RS485_INDEX ? PHOTO_PATH_PORT_485_TEXT : PHOTO_PATH_PORT_232_TEXT, PHOTO_PATH_PORT_TEXT_SIZE);
    if (portNum != PHOTO_PORT_RS485_INDEX)
        path{pos++} = '0' + portNum;

    path{pos++} = PHOTO_PATH_DIR_DELIMETER;
    new years;
    new months;
    new days;
    new hours;
    new mins;
    new secs;
    unixTime2dateTime(GetVar(UNIX_TIME), years, months, days, hours, mins, secs);
    pos += itoaw(years % 100, path, pos, pathMaxSize, PHOTO_PATH_TIME_COMP_SIZE);
    pos += itoaw(months, path, pos, pathMaxSize, PHOTO_PATH_TIME_COMP_SIZE);
    pos += itoaw(days, path, pos, pathMaxSize, PHOTO_PATH_TIME_COMP_SIZE);
    path{pos++} = PHOTO_PATH_DIR_DELIMETER;
    pos += itoaw(hours, path, pos, pathMaxSize, PHOTO_PATH_TIME_COMP_SIZE);
    pos += itoaw(mins, path, pos, pathMaxSize, PHOTO_PATH_TIME_COMP_SIZE);
    pos += itoaw(secs, path, pos, pathMaxSize, PHOTO_PATH_TIME_COMP_SIZE);
    insertArrayStr(path, pos, pathMaxSize, PHOTO_PATH_END, PHOTO_PATH_END_SIZE);
    return true;
}

//! Сохранить первую часть изображения с генерацией метаданных для Wialon
//! \param[inout] chunk исходные данные части изображения, изменяются при сохранении (используется в качестве временного буфера)
//! \param[in] chunkSizeIn исходная длина части изображения
//! \param[in] fileName имя файла для сохранения, должно оканчиваться \0
//! \param[in] maker производитель камеры, должно оканчиваться \0, не более PHOTO_METACHUNK_EXIF_MAKER_SIZE
//! \param[in] model модель камеры, должно оканчиваться \0, не более PHOTO_METACHUNK_EXIF_MODEL_SIZE
//! \param[out] chunkSizeOut общий размер сохраненных данных
//! \param[in] isNeedGetTime признак необходимости получения текущего времени для фото
//! \param[in] unixTime метка времени Unix при isNeedGetTime == false
//! \return PHOTO_METACHUNK_OK - успешно, PHOTO_METACHUNK_ERROR_ при ошибке
photoSaveMetaChunk(chunk{}, chunkSizeIn, const fileName{}, const maker{} = "", const model{} = "", &chunkSizeOut = 0, isNeedGetTime = true, unixTime = 0)
{
    chunkSizeOut = 0;
    if (chunkSizeIn < PHOTO_METACHUNK_SIZE_MIN)
        return PHOTO_METACHUNK_ERROR_SIZE;

    if (countIdenticalStr(chunk, 0, chunkSizeIn, JPEG_SEGMENT_SOI_MARKER, 0, JPEG_SEGMENT_MARKER_SIZE) != JPEG_SEGMENT_MARKER_SIZE)
        return PHOTO_METACHUNK_ERROR_FORMAT;

    new pos = PHOTO_METACHUNK_FILE_POS;
    new size;
    new res = photoSaveMetaChunk_getSegSize(chunk, chunkSizeIn, pos, JFIF_SEGMENT_MARKER, size);
    if (res != PHOTO_METACHUNK_OK)
        return res;
    
    if (size)
    {
        pos += size;
        if (chunkSizeIn < (pos + JFXX_SEGMENT_MARKER_SIZE))
            return PHOTO_METACHUNK_ERROR_SIZE;

        res = photoSaveMetaChunk_getSegSize(chunk, chunkSizeIn, pos, JFXX_SEGMENT_MARKER, size);
        if (res != PHOTO_METACHUNK_OK)
            return res;

        pos += size;
    }
    if (chunkSizeIn < (pos + EXIF_SEGMENT_MARKER_SIZE))
        return PHOTO_METACHUNK_ERROR_SIZE;

    res = photoSaveMetaChunk_getSegSize(chunk, chunkSizeIn, pos, EXIF_SEGMENT_MARKER, size);
    if (res != PHOTO_METACHUNK_OK)
        return res;

    pos += size;
    
    photoSaveMetaChunk_insertImei(photoMeta);
    insertArrayStr(photoMeta, PHOTO_METACHUNK_EXIF_MAKER_POS, PHOTO_METACHUNK_META_SIZE, maker, strLen(maker, PHOTO_METACHUNK_EXIF_MAKER_SIZE));
    insertArrayStr(photoMeta, PHOTO_METACHUNK_EXIF_MODEL_POS, PHOTO_METACHUNK_META_SIZE, model, strLen(model, PHOTO_METACHUNK_EXIF_MODEL_SIZE));
    if (isNeedGetTime)
        unixTime = GetVar(UNIX_TIME);

    photoSaveMetaChunk_insertTime(photoMeta, unixTime);
    photoSaveMetaChunk_insertCoords(photoMeta);
    if (FileWrite(fileName, photoMeta, PHOTO_METACHUNK_META_SIZE, 0) != PHOTO_METACHUNK_META_SIZE)
        return PHOTO_METACHUNK_ERROR_SAVE;
    
    size = insertArrayStr(chunk, 0, chunkSizeIn, chunk, chunkSizeIn, pos);
    if (FileWrite(fileName, chunk, size, PHOTO_METACHUNK_REST_POS) != size)
        return PHOTO_METACHUNK_ERROR_SAVE;

    chunkSizeOut = PHOTO_METACHUNK_META_SIZE + size;
    return PHOTO_METACHUNK_OK;
}

// Приватные функции

photoSaveMetaChunk_getSegSize(const chunk{}, chunkSize, pos, const segmentMarker{}, &segmentSize)
{
    new size;
    if (countIdenticalStr(chunk, pos, chunkSize, segmentMarker, 0, JPEG_SEGMENT_MARKER_SIZE) == JPEG_SEGMENT_MARKER_SIZE)
    {
        new len;
        if (!array2num16beUnSign(chunk, pos + JPEG_SEGMENT_MARKER_SIZE, chunkSize, len))
            return PHOTO_METACHUNK_ERROR_SIZE;

        size = len + JPEG_SEGMENT_MARKER_SIZE;
        if ((size + pos) > chunkSize)
            return PHOTO_METACHUNK_ERROR_SIZE;

        if (size < JFIF_SIZE_MIN)
            return PHOTO_METACHUNK_ERROR_FORMAT;
    }
    segmentSize = size;
    return PHOTO_METACHUNK_OK;
}

photoSaveMetaChunk_insertImei(photoMeta{PHOTO_METACHUNK_META_SIZE})
{
    new imei{PHOTO_METACHUNK_EXIF_IMEI_VALUE_SIZE};
    GetIMEI(imei, PHOTO_METACHUNK_EXIF_IMEI_VALUE_SIZE);
    insertArrayStr(photoMeta, PHOTO_METACHUNK_EXIF_IMEI_VALUE_POS, PHOTO_METACHUNK_META_SIZE, imei, PHOTO_METACHUNK_EXIF_IMEI_VALUE_SIZE);
}

photoSaveMetaChunk_insertTime(photoMeta{PHOTO_METACHUNK_META_SIZE}, unixTime)
{
    new years;
    new months;
    new days;
    new hours;
    new mins;
    new secs;
    unixTime2dateTime(unixTime, years, months, days, hours, mins, secs);
    itoaw(years, photoMeta, PHOTO_METACHUNK_EXIF_YEAR_POS, PHOTO_METACHUNK_META_SIZE, PHOTO_METACHUNK_EXIF_YEAR_SIZE);
    itoaw(months, photoMeta, PHOTO_METACHUNK_EXIF_MONTH_POS, PHOTO_METACHUNK_META_SIZE, PHOTO_METACHUNK_EXIF_MONTH_SIZE);
    itoaw(days, photoMeta, PHOTO_METACHUNK_EXIF_DAY_POS, PHOTO_METACHUNK_META_SIZE, PHOTO_METACHUNK_EXIF_DAY_SIZE);
    itoaw(hours, photoMeta, PHOTO_METACHUNK_EXIF_HOUR_POS, PHOTO_METACHUNK_META_SIZE, PHOTO_METACHUNK_EXIF_HOUR_SIZE);
    itoaw(mins, photoMeta, PHOTO_METACHUNK_EXIF_MINUTE_POS, PHOTO_METACHUNK_META_SIZE, PHOTO_METACHUNK_EXIF_MINUTE_SIZE);
    itoaw(secs, photoMeta, PHOTO_METACHUNK_EXIF_SEC_POS, PHOTO_METACHUNK_META_SIZE, PHOTO_METACHUNK_EXIF_SEC_SIZE);
}

photoSaveMetaChunk_insertCoords(photoMeta{PHOTO_METACHUNK_META_SIZE})
{
    new latitude = GetVar(LATITUDE);
    new longitude = GetVar(LONGITUDE);
    new altitude = GetVar(ALTITUDE);
    
    num32bit2arrayLe(latitude >= 0 ? latitude : -latitude, photoMeta, PHOTO_METACHUNK_EXIF_LATITUDE_POS, PHOTO_METACHUNK_META_SIZE);
    if (latitude < 0)
        photoMeta{PHOTO_METACHUNK_EXIF_LATITUDE_FLAG_POS} = PHOTO_METACHUNK_EXIF_SOUTH_FLAG;
    
    num32bit2arrayLe(longitude >= 0 ? longitude : -longitude, photoMeta, PHOTO_METACHUNK_EXIF_LONGITUDE_POS, PHOTO_METACHUNK_META_SIZE);
    if (longitude < 0)
        photoMeta{PHOTO_METACHUNK_EXIF_LONGITUDE_FLAG_POS} = PHOTO_METACHUNK_EXIF_WEST_FLAG;

    num32bit2arrayLe(altitude >= 0 ? altitude : -altitude, photoMeta, PHOTO_METACHUNK_EXIF_ALTITUDE_POS, PHOTO_METACHUNK_META_SIZE);
    if (altitude < 0)
        photoMeta{PHOTO_METACHUNK_EXIF_ALTITUDE_FLAG_POS} = PHOTO_METACHUNK_EXIF_ALTITUDE_NEG_FLAG;
}

#endif
