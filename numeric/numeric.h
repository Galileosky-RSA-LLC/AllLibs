#ifndef NUMERIC_H
#define NUMERIC_H
// Библиотека работы с числами

#define NUM_VALUE_MAX 0x7FFFFFFF // 2 147 483 647
#define NUM_VALUE_MIN 0x80000000 // -2 147 483 648
#define BIT_DEPTH 32

#ifndef MIN
#define MIN(%1,%2) ((%1) < (%2) ? (%1) : (%2)) // при многих вложенных таких же - ЗАВИСАЕТ КОМПИЛЯЦИЯ
#endif

#ifndef MAX
#define MAX(%1,%2) ((%1) > (%2) ? (%1) : (%2)) // при многих вложенных таких же - ЗАВИСАЕТ КОМПИЛЯЦИЯ
#endif

#ifndef COUNT_USED_CELLS
#define COUNT_USED_CELLS(%1,%2) ((%1) / (%2) + (((%1) % (%2)) != 0))
#endif

abs(num);
coerce(&num, rangeMin, rangeMax);
dec2bcd(dec);
digits(val);
getBit(byteValue, bit);
invertBitsInByte(byte);
pow(num, p);
rnd(atLeast, noGreater);
setBit(byteValue, bit, val);
countUsedCells(size, cellSize);
rol(value, bits);
ror(value, bits);
reverse(value);

#endif // NUMERIC_H
