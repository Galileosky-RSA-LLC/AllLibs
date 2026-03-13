//! @file
//! @brief Заголовок библиотеки работы с EXIF изображениями

#if defined EXIF_H
#endinput
#endif
#define EXIF_H

#include "..\jpeg.h"

#define EXIF_SEGMENT_ID JPEG_SEGMENT_APP1_ID
stock const EXIF_SEGMENT_MARKER{} = {JPEG_SEGMENT_MARKER_PREFIX, EXIF_SEGMENT_ID};
#define EXIF_SEGMENT_MARKER_SIZE JPEG_SEGMENT_MARKER_SIZE
#define EXIF_LEN_POS JPEG_SEGMENT_APP_LEN_POS
#define EXIF_LEN_SIZE JPEG_SEGMENT_APP_LEN_SIZE
