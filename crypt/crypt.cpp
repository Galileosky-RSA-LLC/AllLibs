#if defined CRYPT_LIB
#endinput
#endif
#define CRYPT_LIB

//! @file
//! @brief Реализация библиотеки шифрования

#include "..\numeric\numeric.h"
#include "..\numeric\numeric.cpp"
#include "..\array\array.h"
#include "..\array\array.cpp"
#include "crypt.h"

#define XTEA3_DELTA 0x9E3779B9
#define XTEA3_KEY_HALF_CELLS_AMOUNT (XTEA3_KEY_CELLS_AMOUNT / 2)

//! @publicsection

stock xtea3encrypt(const dataIn[XTEA3_DATA_CELLS_AMOUNT], const key[XTEA3_KEY_CELLS_AMOUNT], dataOut[XTEA3_DATA_CELLS_AMOUNT], numRounds = XTEA3_NUM_ROUNDS_DEFAULT)
{
    new keyMod[XTEA3_KEY_CELLS_AMOUNT];
    for (new i = 0; i < XTEA3_KEY_CELLS_AMOUNT; i++)
        keyMod[i] = reverse(key[i]);
    
    new abcd[XTEA3_DATA_CELLS_AMOUNT];
    for (new i = 0; (i < XTEA3_DATA_CELLS_AMOUNT) && (i < XTEA3_KEY_CELLS_AMOUNT); i++)
        abcd[i] = reverse(dataIn[i]) + keyMod[i];

    new sum;
    for (new i = 0; i < numRounds; i++)
    {
        abcd[0] += xtea3crypt_roundCalcModA(abcd[1], abcd[3], sum, keyMod);
		sum += XTEA3_DELTA;
	    abcd[2] += xtea3crypt_roundCalcModC(abcd[1], abcd[3], sum, keyMod);
        arrayRingShift(abcd, XTEA3_DATA_CELLS_AMOUNT, false);
	}
	for (new i = 0; (i < XTEA3_DATA_CELLS_AMOUNT) && (i < XTEA3_KEY_HALF_CELLS_AMOUNT); i++)
        dataOut[i] = reverse(abcd[i] ^ keyMod[XTEA3_KEY_HALF_CELLS_AMOUNT + i]);
}

stock xtea3decrypt(const dataIn[XTEA3_DATA_CELLS_AMOUNT], const key[XTEA3_KEY_CELLS_AMOUNT], dataOut[XTEA3_DATA_CELLS_AMOUNT], numRounds = XTEA3_NUM_ROUNDS_DEFAULT)
{
    new keyMod[XTEA3_KEY_CELLS_AMOUNT];
    for (new i = 0; i < XTEA3_KEY_CELLS_AMOUNT; i++)
        keyMod[i] = reverse(key[i]);

    new abcd[XTEA3_DATA_CELLS_AMOUNT];
    for (new i = 0; (i < XTEA3_DATA_CELLS_AMOUNT) && (i < XTEA3_KEY_HALF_CELLS_AMOUNT); i++)
        abcd[i] = reverse(dataIn[i]) ^ keyMod[XTEA3_KEY_HALF_CELLS_AMOUNT + i];

    new sum = XTEA3_DELTA * numRounds;
	for (new i = 0; i < numRounds; i++)
    {
		arrayRingShift(abcd, XTEA3_DATA_CELLS_AMOUNT, true);
		abcd[2] -= xtea3crypt_roundCalcModC(abcd[1], abcd[3], sum, keyMod);
        sum -= XTEA3_DELTA;
		abcd[0] -= xtea3crypt_roundCalcModA(abcd[1], abcd[3], sum, keyMod);
    }
	for (new i = 0; (i < XTEA3_DATA_CELLS_AMOUNT) && (i < XTEA3_KEY_CELLS_AMOUNT); i++)
        dataOut[i] = reverse(abcd[i] - keyMod[i]);
}

//! @privatesection

stock xtea3crypt_roundCalcModA(b, d, sum, const key[XTEA3_KEY_CELLS_AMOUNT])
{
    return ((b<<4) + rol(key[(sum % 4) + 4], b)) ^ (d + sum) ^ ((b>>>5) + rol(key[sum % 4], b>>>27));
}

stock xtea3crypt_roundCalcModC(b, d, sum, const key[XTEA3_KEY_CELLS_AMOUNT])
{
    return ((d<<4) + rol(key[((sum>>>11) % 4) + 4], d)) ^ (b + sum) ^ ((d>>>5) + rol(key[(sum>>>11) % 4], d>>>27));
}
