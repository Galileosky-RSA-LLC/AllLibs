#ifndef FILE_H
#define FILE_H
// Заголовок библиотеки работы с файлами

#define FILE_TIME_ZONE "time_zone"
#define FILE_FULL_PATH_SIZE_MAX 255
#define FILE_FULL_PATH_SIZE_MAX_W0 (FILE_FULL_PATH_SIZE_MAX + 1)

fileRename(const src{}, const dest{});
fileWriteWrap(const fileName{}, const buf{}, bufSize, fileOffset, bufPos = 0);

#endif // FILE_H