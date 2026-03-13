//! @file
//! @brief Заголовок библиотеки работы с числами

#if defined NUMERIC_H
#endinput
#endif
#define NUMERIC_H

#if !defined MIN
#define MIN(%1,%2) ((%1) < (%2) ? (%1) : (%2)) // при многих вложенных таких же - ЗАВИСАЕТ КОМПИЛЯЦИЯ
#endif

#if !defined MAX
#define MAX(%1,%2) ((%1) > (%2) ? (%1) : (%2)) // при многих вложенных таких же - ЗАВИСАЕТ КОМПИЛЯЦИЯ
#endif

#if !defined COUNT_USED_CELLS
#define COUNT_USED_CELLS(%1,%2) (((%1) / (%2)) + ((((%1) % (%2)) != 0) ? 1 : 0))
#endif

#define BYTE_BITS 8
#define CELL_BYTES (cellbits / BYTE_BITS)
#define CELL_LAST_BYTE_INDEX (CELL_BYTES - 1)
#define CELL_LAST_BIT_INDEX (cellbits - 1)

//! @defgroup general Общие функции
//! @{

//! @brief Получить модуль числа
forward stock abs(num);

//! @brief Привести число к требуемому диапазону
//! @param[inout] num преобразуемое число
//! @param[in] rangeMin минимум диапазона
//! @param[in] rangeMax максимум диапазона
//! @return false - число не корректировалось, true - число корректировалось
forward bool:stock coerce(&num, rangeMin, rangeMax);

//! @brief Получить количество занятых ячеек по актуальной длине параметра
//! @details Например: .totalSize=5,.cellSize=2 -> 3
forward stock countUsedCells(totalSize, cellSize);

//! @brief Конвертировать в BCD
forward stock dec2bcd(dec);

//! @brief Получить количество разрядов в числе
forward stock digits(val);

//! @brief Получить байт из числа
//! @param[in] number исходное число
//! @param[in] byteId позиция байта в числе, 0..3
forward stock getByte(number, byteId);

//! @brief Возвести число в неотрицательную степень
//! @details без проверок границ диапазона
//! @param[in] num исходное число
//! @param[in] p степень
forward stock pow(num, p);

//! @brief Перевернуть байты числа
//! @details Например: 0xAABBCCDD -> 0xDDCCBBAA
forward stock reverse(value);

//! @brief Генерировать псевдослучайное число из заданного диапазона
//! @param[in] atLeast нижняя граница (от), 0..2147483647
//! @param[in] noGreater верхняя граница (до), 0..2147483647
forward stock rnd(atLeast, noGreater);

//! @brief Установить байт в числе
//! @param[in] number исходное число
//! @param[in] byteId номер изменяемого байта в числе, 0..3
//! @param[in] newByteValue новое значение байта
//! @return измененное число
forward stock setByte(number, byteId, newByteValue);
//! @}

//! @defgroup bits Битовые операции
//! @{

//! @brief Получить значение бита
//! @param[in] num число
//! @param[in] bit индекс бита, 0..31
//! @return значение бита
forward bool:stock getBit(num, bit);

//! @brief Инвертировать биты в байте
forward stock invertBitsInByte(byte);

//! @brief Циклический сдвиг влево
//! @param[in] value число
//! @param[in] bits кол-во разрядов
forward stock rol(value, bits);

//! @brief Циклический сдвиг вправо
//! @param[in] value число
//! @param[in] bits кол-во разрядов
forward stock ror(value, bits);

//! @brief Развернуть биты числа
//! @param[in] value исходное число
//! @param[in] bits количество бит для разворота
//! @return измененное число
forward stock reverseBits(value, bits);

//! @brief Установить значение бита
//! @param[in] num исходное число
//! @param[in] bit изменяемый бит, 0..31
//! @param[in] bitValue устанавливаемое значение бита
//! @return измененное число
forward stock setBit(num, bit, bool:bitValue);
//! @}
