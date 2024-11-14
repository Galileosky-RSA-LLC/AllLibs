#ifndef FILE_H
#define FILE_H
// Заголовок библиотеки работы с файлами

#define FILE_TIME_ZONE "time_zone"

fileRename(const src{}, const dest{});
fileWriteWrap(const fileName{}, const buf{}, bufSize, fileOffset, bufPos = 0);

#endif // FILE_H