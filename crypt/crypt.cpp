#ifndef CRYPT_LIB
#define CRYPT_LIB
// Библиотека шифрования

#include "crypt.h"
#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"
#include "..\array\array.h"
#include "..\array\array.cpp"

xtea3encrypt(const dataIn[XTEA3_DATA_CELLS_AMOUNT], const key[XTEA3_KEY_CELLS_AMOUNT], dataOut[XTEA3_DATA_CELLS_AMOUNT], numRounds = XTEA3_NUM_ROUNDS_DEFAULT)
{
	new abcd[XTEA3_DATA_CELLS_AMOUNT];
    for (new i = 0; (i < XTEA3_DATA_CELLS_AMOUNT) && (i < XTEA3_KEY_CELLS_AMOUNT); i++)
        abcd[i] = dataIn[i] + key[i];

    new sum;
	for (new i = 0; i < numRounds; i++)
    {
		abcd[0] += xtea3crypt_roundCalcModA(abcd[1], abcd[3], sum, key);
		sum += XTEA3_DELTA;
	    abcd[2] += xtea3crypt_roundCalcModC(abcd[1], abcd[3], sum, key);
        arrayRingShift(abcd, XTEA3_DATA_CELLS_AMOUNT, false);
	}
	for (new i = 0; (i < XTEA3_DATA_CELLS_AMOUNT) && (i < XTEA3_KEY_HALF_CELLS_AMOUNT); i++)
        dataOut[i] = abcd[i] ^ key[XTEA3_KEY_HALF_CELLS_AMOUNT + i];
}
 
xtea3decrypt(const dataIn[XTEA3_DATA_CELLS_AMOUNT], const key[XTEA3_KEY_CELLS_AMOUNT], dataOut[XTEA3_DATA_CELLS_AMOUNT], numRounds = XTEA3_NUM_ROUNDS_DEFAULT)
{
    new abcd[XTEA3_DATA_CELLS_AMOUNT];
    for (new i = 0; (i < XTEA3_DATA_CELLS_AMOUNT) && (i < XTEA3_KEY_HALF_CELLS_AMOUNT); i++)
        abcd[i] = dataIn[i] ^ key[XTEA3_KEY_HALF_CELLS_AMOUNT + i];

    new sum = XTEA3_DELTA * numRounds;
	for (new i = 0; i < numRounds; i++)
    {
		arrayRingShift(abcd, XTEA3_DATA_CELLS_AMOUNT, true);
		abcd[2] -= xtea3crypt_roundCalcModC(abcd[1], abcd[3], sum, key);
        sum -= XTEA3_DELTA;
		abcd[0] -= xtea3crypt_roundCalcModA(abcd[1], abcd[3], sum, key);
    }
	for (new i = 0; (i < XTEA3_DATA_CELLS_AMOUNT) && (i < XTEA3_KEY_CELLS_AMOUNT); i++)
        dataOut[i] = abcd[i] - key[i];
}

// Приватные функции

xtea3crypt_roundCalcModA(b, d, sum, const key[XTEA3_KEY_CELLS_AMOUNT])
{
    return ((b<<4) + rol(key[(sum % 4) + 4], b)) ^ (d + sum) ^ ((b>>>5) + rol(key[sum % 4], b>>>27));
}

xtea3crypt_roundCalcModC(b, d, sum, const key[XTEA3_KEY_CELLS_AMOUNT])
{
    return ((d<<4) + rol(key[((sum>>>11) % 4) + 4], d)) ^ (b + sum) ^ ((d>>>5) + rol(key[(sum>>>11) % 4], d>>>27));
}

#endif
