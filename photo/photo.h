#if !defined PHOTO_H
#define PHOTO_H

#include "..\jpeg\jpeg.h"
#include "..\jpeg\jfif\jfif.h"
#include "..\jpeg\exif\exif.h"

#define PHOTO_PATH_BEGIN "Pic/RS"
#define PHOTO_PATH_BEGIN_SIZE 6
#define PHOTO_PATH_PORT_TEXT_SIZE 3
#define PHOTO_PATH_PORT_232_TEXT "232"
#define PHOTO_PATH_PORT_485_TEXT "485"
#define PHOTO_PATH_PORT_232_INDEX_SIZE 1
#define PHOTO_PATH_DELIMETER_SIZE 1
#define PHOTO_PATH_DIR_DELIMETER '/'
#define PHOTO_PORT_RS485_INDEX 2
#define PHOTO_PATH_TIME_COMP_SIZE 2
#define PHOTO_PATH_END ".jpg\0"
#define PHOTO_PATH_END_SIZE 5
#define PHOTO_PATH_SIZE (PHOTO_PATH_BEGIN_SIZE + PHOTO_PATH_PORT_TEXT_SIZE + PHOTO_PATH_PORT_232_INDEX_SIZE + (2 * PHOTO_PATH_DELIMETER_SIZE) + \
                        (6 * PHOTO_PATH_TIME_COMP_SIZE) + PHOTO_PATH_END_SIZE)
#define PHOTO_PORT_MIN 0
#define PHOTO_PORT_MAX PHOTO_PORT_RS485_INDEX

// Метаданные фотографий
#define PHOTO_METACHUNK_SIZE_MIN (JPEG_SEGMENT_SOI_SIZE + JPEG_SEGMENT_MARKER_SIZE)
#define PHOTO_METACHUNK_FILE_POS JPEG_SEGMENT_SOI_SIZE

#define PHOTO_METACHUNK_EXIF_IMEI_VALUE_POS 110
#define PHOTO_METACHUNK_EXIF_IMEI_VALUE_SIZE 15

#define PHOTO_METACHUNK_EXIF_MAKER_POS 125
#define PHOTO_METACHUNK_EXIF_MAKER_SIZE 10

#define PHOTO_METACHUNK_EXIF_MODEL_POS 135
#define PHOTO_METACHUNK_EXIF_MODEL_SIZE 10

#define PHOTO_METACHUNK_EXIF_YEAR_POS 145
#define PHOTO_METACHUNK_EXIF_YEAR_SIZE 4
#define PHOTO_METACHUNK_EXIF_MONTH_POS 150
#define PHOTO_METACHUNK_EXIF_MONTH_SIZE 2
#define PHOTO_METACHUNK_EXIF_DAY_POS 153
#define PHOTO_METACHUNK_EXIF_DAY_SIZE 2
#define PHOTO_METACHUNK_EXIF_HOUR_POS 156
#define PHOTO_METACHUNK_EXIF_HOUR_SIZE 2
#define PHOTO_METACHUNK_EXIF_MINUTE_POS 159
#define PHOTO_METACHUNK_EXIF_MINUTE_SIZE 2
#define PHOTO_METACHUNK_EXIF_SEC_POS 162
#define PHOTO_METACHUNK_EXIF_SEC_SIZE 2

#define PHOTO_METACHUNK_EXIF_LATITUDE_FLAG_POS 175
#define PHOTO_METACHUNK_EXIF_SOUTH_FLAG 'S'

#define PHOTO_METACHUNK_EXIF_LONGITUDE_FLAG_POS 199
#define PHOTO_METACHUNK_EXIF_WEST_FLAG 'W'

#define PHOTO_METACHUNK_EXIF_ALTITUDE_FLAG_POS 223
#define PHOTO_METACHUNK_EXIF_ALTITUDE_NEG_FLAG 0x01

#define PHOTO_METACHUNK_EXIF_LATITUDE_POS 243
#define PHOTO_METACHUNK_EXIF_LONGITUDE_POS 267
#define PHOTO_METACHUNK_EXIF_ALTITUDE_POS 291

#define PHOTO_METACHUNK_JFIF_SIZE 19
#define PHOTO_METACHUNK_EXIF_SIZE 278
#define PHOTO_METACHUNK_META_SIZE (JPEG_SEGMENT_SOI_SIZE + PHOTO_METACHUNK_JFIF_SIZE + PHOTO_METACHUNK_EXIF_SIZE)
#define PHOTO_METACHUNK_EXIF_LENGTH (PHOTO_METACHUNK_EXIF_SIZE - JPEG_SEGMENT_MARKER_SIZE)
#define PHOTO_METACHUNK_EXIF_LENGTH_HI ((PHOTO_METACHUNK_EXIF_LENGTH & 0xFF00) >> 8)
#define PHOTO_METACHUNK_EXIF_LENGTH_LO (PHOTO_METACHUNK_EXIF_LENGTH & 0xFF)

new photoMeta{PHOTO_METACHUNK_META_SIZE} =
{
    // SOI
    JPEG_SEGMENT_MARKER_PREFIX, JPEG_SEGMENT_SOI_ID, // offset 0;
    
    // APP0 (JFIF) - тут не все по стандарту, но так надо для Wialon
    JPEG_SEGMENT_MARKER_PREFIX, JFIF_SEGMENT_ID, // offset 2; Marker
    0x00, 0x11, //  offset 4; segment length
    'J', 'F',  'I',  'F',  '\0', // offset 6
    0x01, 0x01, // offset 11; version 1.01
    0x01, // offset 13; density units - Pixels per inch
    0x00, 0x00, // offset 14; Xdensity
    0x00, 0x00, // offset 16; Ydensity
    0x00, // offset 18; Xthumbnail
    0x00, // offset 19; Ythumbnail
    0x0A, // offset 20; Thumbnail data

    // APP1 (Exif)
    JPEG_SEGMENT_MARKER_PREFIX, EXIF_SEGMENT_ID, // offset 21; Marker
    PHOTO_METACHUNK_EXIF_LENGTH_HI, PHOTO_METACHUNK_EXIF_LENGTH_LO, // offset 23; segment length
    'E', 'x', 'i', 'f', '\0', // offset 25; ExifID
    0x00, // offset 30; padding
    // TIFF header
    'I', 'I', // offset 31; порядок байт ("II" - Intel, little-endian)
    0x2A, 0x00, // offset 33; TIFF file (42)
    0x08, 0x00, 0x00, 0x00, // offset 35; 0th IFD Offset (8)
    // 0th IFD
    0x05, 0x00, // offset 39; кол-во записей (5)
    // описатель 1-й записи
    0x0E, 0x01, // offset 41; tagId (ImageDescription)
    0x02, 0x00, // offset 43; type (строка ASCII)
    0x14, 0x00, 0x00, 0x00, // offset 45; count (20)
    0x4A, 0x00, 0x00, 0x00, // offset 49; значение (если count <= 4) или смещение значения - смещение относительно заголовка TIFF (74)
    // описатель 2-й записи
    0x0F, 0x01, // offset 53; tagId (Make)
    0x02, 0x00, // offset 55; type (строка ASCII)
    0x0A, 0x00, 0x00, 0x00, // offset 57; count (10)
    0x5E, 0x00, 0x00, 0x00, // offset 61; смещение (94)
    // описатель 3-й записи
    0x10, 0x01, // offset 65; tagId (Model)
    0x02, 0x00, // offset 67; type (строка ASCII)
    0x0A, 0x00, 0x00, 0x00, // offset 69; count (10)
    0x68, 0x00, 0x00, 0x00, // offset 73; смещение (104)
    // описатель 4-й записи
    0x32, 0x01, // offset 77; tagId (DateTime)
    0x02, 0x00, // offset 79; type (строка ASCII)
    0x14, 0x00, 0x00, 0x00, // offset 81; count (20)
    0x72, 0x00, 0x00, 0x00, // offset 85; смещение (114)
    // описатель 5-й записи
    0x25, 0x88, // offset 89; tagId (GPS Info IFD Pointer)
    0x04, 0x00, // offset 91; type (uint32)
    0x01, 0x00, 0x00, 0x00, // offset 93; count (1)
    0x86, 0x00, 0x00, 0x00, // offset 97; смещение (134)
    // смещение следующего IFD
    0x00, 0x00, 0x00, 0x00, // offset 101; нет следующего
    // Value longer than 4Byte of 0th IFD
    'I', 'M', 'E', 'I', ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // offset 105; ImageDescription
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // offset 125; Maker
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // offset 135; Model
    'Y', 'Y', 'Y', 'Y', ':', 'M', 'M', ':', 'D', 'D', ' ', 'h', 'h', ':', 'm', 'm', ':', 's', 's', '\0', // offset 145; дата и время
    // GPS IFD
    0x06, 0x00, // offset 165; кол-во записей (6)
    // описатель 1-й записи
    0x01, 0x00, // offset 167; tagId (GPSLatitudeRef)
    0x02, 0x00, // offset 169; type (строка ASCII)
    0x02, 0x00, 0x00, 0x00, // offset 171; count (2)
    // флаг широты
    'N', 0x00, 0x00, 0x00, // offset 175; значение (по умолчанию север)
    // описатель 2 записи
    0x02, 0x00, // offset 179; tagId (GPSLatitude)
    0x05, 0x00, // offset 181; type (RATIONAL - 2 LONG)
    0x03, 0x00, 0x00, 0x00, // offset 183; count (3)
    0xD4, 0x00, 0x00, 0x00, // offset 187; смещение (212)
    // описатель 3 записи
    0x03, 0x00, // offset 191; tagId (GPSLongitudeRef)
    0x02, 0x00, // offset 193; type (строка ASCII)
    0x02, 0x00, 0x00, 0x00, // offset 195; count (2)
    'E', 0x00, 0x00, 0x00, // offset 199; значение (по умолчанию запад)
    // описатель 4 записи
    0x04, 0x00, // offset 203; tagId (GPSLongitude)
    0x05, 0x00, // offset 205; type (RATIONAL - 2 LONG)
    0x03, 0x00, 0x00, 0x00, // offset 207; count (3)
    0xEC, 0x00, 0x00, 0x00, // offset 211; смещение (236)
    // описатель 5 записи
    0x05, 0x00, // offset 215; tagId (GPSAltitudeRef)
    0x01, 0x00, // offset 217; type (uint8)
    0x01, 0x00, 0x00, 0x00, // offset 219; count (1)
    0x00, 0x00, 0x00, 0x00, // offset 223; значение (по умолчанию неотрицательная эллипсоидальная высота от эллипсоидальной поверхности)
    // описатель 6 записи
    0x06, 0x00, // offset 227; tagId (GPSAltitude)
    0x05, 0x00, // offset 229; type (RATIONAL - 2 LONG)
    0x01, 0x00, 0x00, 0x00, // offset 231; count (1)
    0x04, 0x01, 0x00, 0x00, // offset 235; смещение (260)
    // смещение следующего IFD
    0x00, 0x00, 0x00, 0x00, // offset 239; нет следующего
    // Value longer than 4Byte of GPS IFD
    0x00, 0x00, 0x00, 0x00, // offset 243; Latitude градусы числитель (по умолчанию 0)
    0x40, 0x42, 0x0F, 0x00, // offset 247; Latitude градусы знаменатель (1 000 000)
    0x00, 0x00, 0x00, 0x00, // offset 251; Latitude минуты числитель (0)
    0x01, 0x00, 0x00, 0x00, // offset 255; Latitude минуты знаменатель (1)
    0x00, 0x00, 0x00, 0x00, // offset 259; Latitude секунды числитель (по умолчанию 0)
    0x01, 0x00, 0x00, 0x00, // offset 263; Latitude секунды знаменатель (1)
    0x00, 0x00, 0x00, 0x00, // offset 267; Longitude градусы числитель (по умолчанию 0)
    0x40, 0x42, 0x0F, 0x00, // offset 271; Longitude градусы знаменатель (1 000 000)
    0x00, 0x00, 0x00, 0x00, // offset 275; Longitude минуты числитель (0)
    0x01, 0x00, 0x00, 0x00, // offset 279; Longitude минуты знаменатель (1)
    0x00, 0x00, 0x00, 0x00, // offset 283; Longitude секунды числитель (0)
    0x01, 0x00, 0x00, 0x00, // offset 287; Longitude секунды знаменатель (1)
    0x00, 0x00, 0x00, 0x00, // offset 291; Altitude м, числитель (по умолчанию 0)
    0x01, 0x00, 0x00, 0x00, // offset 295; Altitude м, знаменатель (1)
};

#define PHOTO_METACHUNK_REST_POS PHOTO_METACHUNK_META_SIZE

// результаты сохранения фрагмента фото с метаинформацией
#define PHOTO_METACHUNK_OK 1
#define PHOTO_METACHUNK_ERROR_SIZE -1
#define PHOTO_METACHUNK_ERROR_FORMAT -2
#define PHOTO_METACHUNK_ERROR_SAVE -3


generatePhotoPath(portNum, path{}, pathMaxSize);
photoSaveMetaChunk(chunk{}, chunkSizeIn, const fileName{}, const maker{} = "", const model{} = "", &chunkSizeOut = 0, isNeedGetTime = true, unixTime = 0);

#endif
