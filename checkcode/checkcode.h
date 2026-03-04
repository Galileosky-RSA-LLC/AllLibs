//! @file
//! @brief Заголовок библиотеки проверочных кодов (CRC, CS, etc.)

#if defined CHECKCODE_H
#endinput
#endif
#define CHECKCODE_H

//! @defgroup general Общие функции
//! @{

//! @brief Рассчитать CRC
//! @param[in] data исходный массив
//! @param[in] dataSize размер массива
//! @param[in] polyWidth разрядность полинома
//! @param[in] poly полином, без старшего бита
//! @param[in] pos стартовый индекс массива
//! @param[in] init начальное значение
//! @param[in] isRevert признак инверсии битов: true - инвертировать, false - нет; к выходному значению применяется только при isFinish == true
//! @param[in] xorOut величина для XOR с конечным значением, применяется только при isFinish == true
//! @param[in] isFinish признак завершения расчета: true - последний проход, false - не последний
//! @return рассчитанное значение
forward stock crc(const data{}, dataSize, polyWidth, poly, pos = 0, init = 0, bool:isRevert = false, xorOut = 0, bool:isFinish = true);

//! @}


//! @defgroup bits8 8 бит
//! @{

//! @brief CRC-8/MAXIM-DOW, CRC-8/MAXIM, DOW-CRC
//! @details Poly: 0x31 (x^8 + x^5 + x^4 + 1), 0x8C
//! RefIn: true
//! RefOut: true
//! XorOut: 0x00
//! Check: 0xA1 ("123456789")
//! @param[in] data исходный массив
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
//! @param[in] init значение инициализации
//! @param[in] isFinish признак завершения расчета: true - последний проход, false - не последний
forward stock crc8(const data{}, dataSize, pos = 0, init = 0x00, bool:isFinish = true);

//! @brief LRC8, CHKSUM-8/ModBus, Intel HEX
//! @param[in] data исходный массив
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
//! @param[in] init значение инициализации
forward stock lrc8(const data{}, dataSize, pos = 0, init = 0x00);

//! @brief CRC-8 Dallas/Maxim, CRC-8/NRSC-5
//! @details Poly: 0x31 (x^8 + x^5 + x^4 + 1), 0xC8
//! RefIn: false
//! RefOut: false
//! XorOut: 0x00
//! Check: 0xF7 ("123456789")
//! @param[in] data массив с данными для расчета
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
//! @param[in] init значение инициализации
//! @param[in] isFinish признак завершения расчета: true - последний проход, false - не последний
forward stock crc8dallasMaxim(const data{}, dataSize, pos = 0, init = 0xFF, bool:isFinish = true);

//! @}


//! @defgroup bits16 16 бит
//! @{

//! @brief CRC-16/KERMIT, CRC-16/BLUETOOTH, CRC-16/CCITT, CRC-16/CCITT-TRUE, CRC-16/V-41-LSB
//! @details Poly: 0x1021 (x^16 + x^12 + x^5 + 1), 0x8408
//! RefIn: true
//! RefOut: true
//! XorOut: 0x0000
//! Check: 0x2189 ("123456789")
//! @param[in] data исходный массив
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
//! @param[in] init значение инициализации
//! @param[in] isFinish признак завершения расчета: true - последний проход, false - не последний
forward stock crc16Kermit(const data{}, dataSize, pos = 0, init = 0, bool:isFinish = true);

//! @brief Контрольная сумма Флетчера 
//! @param[in] data исходный массив
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
forward stock fletcher16(const data{}, dataSize, pos = 0);

//! @brief Контрольная сумма Флетчера, оптимизированный вариант 
//! @param[in] data исходный массив
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
forward stock fletcher16opt(const data{}, dataSize, pos = 0);

//! @brief CRC-16/IBM-3740, CRC-16/AUTOSAR, CRC-16/CCITT-FALSE
//! @details Poly: 0x1021 (x^16 + x^12 + x^5 + 1)
//! Revert: false
//! XorOut: 0x0000
//! Check: 0x29B1 ("123456789")
//! MaxLen: 4095 байт (32767 бит) - обнаружение одинарных, двойных, тройных и всех нечетных ошибок
//! @param[in] data исходный массив
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
//! @param[in] init значение инициализации
//! @param[in] isFinish признак завершения расчета: true - последний проход, false - не последний
forward stock crc16ccitt(const data{}, dataSize, pos = 0, init = 0xFFFF, bool:isFinish = true);

//! @brief CRC-16/UMTS, CRC-16/BUYPASS, CRC-16/VERIFONE
//! @details Poly: 0x8005 (x^16 + x^15 + x^2 + 1)
//! RefIn: false
//! RefOut: false
//! XorOut: 0x0000
//! Check: 0xFEE8 ("123456789")
//! @param[in] data массив с данными для расчета
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
//! @param[in] init значение инициализации
//! @param[in] isFinish признак завершения расчета: true - последний проход, false - не последний
forward stock crc16buypass(const data{}, dataSize, pos = 0, init = 0x0000, bool:isFinish = true);

//! @}


//! @defgroup bits32 32 бит
//! @{

//! @brief CRC-32/zlib, CRC-32/ISO-HDLC, CRC-32, CRC-32/ADCCP, CRC-32/V-42, CRC-32/XZ, PKZIP
//! @details Poly: 0x04C11DB7 (x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1)
//! Revert: true
//! XorOut: 0xFFFFFFFF
//! Check: 0xCBF43926 ("123456789")
//! MaxLen: 268 435 455 байт (2 147 483 647 бит) - обнаружение одинарных, двойных, пакетных и всех нечетных ошибок
//! @param[in] data массив с данными для расчета
//! @param[in] dataSize размер массива
//! @param[in] pos стартовый индекс подмассива для вычисления
//! @param[in] init значение инициализации
//! @param[in] isFinish признак завершения расчета: true - последний проход, false - не последний
forward stock crc32(const data{}, dataSize, pos = 0, init = 0xFFFFFFFF, bool:isFinish = true);

//! @}
