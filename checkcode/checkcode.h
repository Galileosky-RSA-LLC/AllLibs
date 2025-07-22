#ifndef CHECKCODE_H
#define CHECKCODE_H
// Заголовок библиотеки проверочных кодов (CRC, CS, etc.)

//// 8 бит

//! CRC8
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
forward stock crc8(const data{}, length, pos = 0)

//! LRC8
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
forward stock lrc8(const data{}, length, pos = 0)

//! CRC-8-Dallas/Maxim
//! Poly: 0x31 (x^8 + x^5 + x^4 + 1)
//! init: 0xFF
//! Check: 0xF7 ("123456789")
//! \param[in] data массив с данными для расчета
//! \param[in] dataSize размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение инициализации
forward stock crc8dallasMaxim(const data{}, dataSize, pos = 0, init = 0xFF)


//// 16 бит

//! CRC-KERMIT
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение при инициализации
forward stock crc16Kermit(const data{}, length, pos = 0, init = 0)

//! Контрольная сумма Флетчера 
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
forward stock fletcher16(const data{}, length, pos = 0)

//! Контрольная сумма Флетчера, оптимизированный вариант 
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
forward stock fletcher16opt(const data{}, length, pos = 0)

//! Poly  : 0x1021    x^16 + x^12 + x^5 + 1
//!  Revert: false
//!  XorOut: 0x0000
//!  Check : 0x29B1 ("123456789")
//! MaxLen: 4095 байт (32767 бит) - обнаружение одинарных, двойных, тройных и всех нечетных ошибок
//! по окончании расчета взять младшие 2 байта
//! \param[in] data исходный массив
//! \param[in] length размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение при инициализации
forward stock crc16ccitt(const data{}, length, pos = 0, init = 0xFFFF)

//! Poly: 0x8005 (x^16 + x^15 + x^2 + 1)
//! Init  : 0x0000
//! RefIn: false
//! RefOut: false
//! XorOut: 0x0000
//! Check : 0xFEE8 ("123456789")
//! \param[in] data массив с данными для расчета
//! \param[in] dataSize размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение инициализации
forward stock crc16buypass(const data{}, dataSize, pos = 0, init = 0x0000)


//// 32 бит

//! Poly: 0x04C11DB7 (x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1)
//! Init  : 0xFFFFFFFF
//! Revert: true
//! XorOut: 0xFFFFFFFF
//! Check : 0xCBF43926 ("123456789")
//! MaxLen: 268 435 455 байт (2 147 483 647 бит) - обнаружение одинарных, двойных, пакетных и всех нечетных ошибок
//! \param[in] data массив с данными для расчета
//! \param[in] dataSize размер массива
//! \param[in] pos стартовый индекс подмассива для вычисления
//! \param[in] init значение инициализации
forward stock crc32(const data{}, dataSize, pos = 0, init = 0xFFFFFFFF)

#endif // CHECKCODE_H