#ifndef CHECKCODE_H
#define CHECKCODE_H
// Функции вычисления проверочных кодов (CRC, CS, etc.)

// 8 бит
crc8(const data{}, length, pos = 0);
lrc8(const data{}, length, pos = 0);
crc8dallasMaxim(const data{}, dataSize, pos = 0, init = 0xFF);

// 16 бит
crc16Kermit(const data{}, length, pos = 0, init = 0);
fletcher16(const data{}, length, pos = 0);
fletcher16opt(const data{}, length, pos = 0);
crc16ccitt(const data{}, length, pos = 0, init = 0xFFFF);
stock crc16buypass(const data{}, dataSize, pos = 0, init = 0x0000)

// 32 бит
crc32(const data{}, dataSize, pos = 0, init = 0xFFFFFFFF);

#endif // CHECKCODE_H