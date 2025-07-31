//! @file
//! @brief Заголовок библиотеки работы с числами

#ifdef NUMERIC_H
#endinput
#endif
#define NUMERIC_H

#define NUM_VALUE_MAX cellmax // 2 147 483 647, устаревшее определение, использовать cellmax
#define NUM_VALUE_MIN cellmin // -2 147 483 648, устаревшее определение, использовать cellmin

#ifndef MIN
#define MIN(%1,%2) ((%1) < (%2) ? (%1) : (%2)) // при многих вложенных таких же - ЗАВИСАЕТ КОМПИЛЯЦИЯ
#endif

#ifndef MAX
#define MAX(%1,%2) ((%1) > (%2) ? (%1) : (%2)) // при многих вложенных таких же - ЗАВИСАЕТ КОМПИЛЯЦИЯ
#endif

#ifndef COUNT_USED_CELLS
#define COUNT_USED_CELLS(%1,%2) ((%1) / (%2) + (((%1) % (%2)) != 0))
#endif

#define BYTE_BITS 8
#define CELL_BYTES (cellbits / BYTE_BITS)

//! @defgroup general Общие функции
//! @{

//! @brief Получить модуль числа
forward stock abs(num);

//! @brief Привести число к требуемому диапазону
//! @param[inout] num преобразуемое число
//! @param[in] rangeMin минимум диапазона
//! @param[in] rangeMax максимум диапазона
//! @return false - число не корректировалось, true - число корректировалось
forward stock coerce(&num, rangeMin, rangeMax);

//! @brief Получить количество занятых ячеек по актуальной длине параметра
forward stock countUsedCells(size, cellSize);

//! @brief Конвертировать в BCD
forward stock dec2bcd(dec);

//! @brief Получить количество разрядов в числе
forward stock digits(val);

//! @brief Возвести число в неотрицательную степень
//! @details без проверок границ диапазона
//! @param[in] num исходное число
//! @param[in] p степень
forward stock pow(num, p);

//! @brief Перевернуть байты числа
//! @details например, 0xAABBCCDD -> 0xDDCCBBAA
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

//! @brief Получить значение бита из байта
//! @param[in] byteValue исходный байт
//! @param[in] bit номер бита, 0..7
//! @return значение бита false/true
forward stock getBit(byteValue, bit);

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

//! @brief Установить значение бита в байте
//! @param[in] byteValue исходный байт
//! @param[in] bit номер изменяемого бита, 0..7
//! @param[in] val устанавливаемое значение бита false/true
//! @return измененный байт
forward stock setBit(byteValue, bit, val);

//! @}
