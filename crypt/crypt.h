#if defined CRYPT_H
#endinput
#endif
#define CRYPT_H

//! @file
//! @brief Заголовок библиотеки шифрования

#include "..\numeric\numeric.h"

//! @defgroup xtea3 XTEA-3
//! @{
#define XTEA3_NUM_ROUNDS_DEFAULT 64
#define XTEA3_DATA_CELLS_AMOUNT 4
#define XTEA3_DATA_SIZE (XTEA3_DATA_CELLS_AMOUNT * CELL_BYTES)
#define XTEA3_KEY_CELLS_AMOUNT (XTEA3_DATA_CELLS_AMOUNT * 2)
#define XTEA3_KEY_SIZE (XTEA3_KEY_CELLS_AMOUNT * CELL_BYTES)
//! @}

//! @brief Зашифровать методом XTEA-3
//! @param[in] dataIn исходные данные для шифрования
//! @param[in] key ключ шифрования
//! @param[out] dataOut зашифрованные данные
//! @param[in] numRounds количество раундов шифрования
forward stock xtea3encrypt(const dataIn[XTEA3_DATA_CELLS_AMOUNT], const key[XTEA3_KEY_CELLS_AMOUNT], dataOut[XTEA3_DATA_CELLS_AMOUNT],
                            numRounds = XTEA3_NUM_ROUNDS_DEFAULT);

//! @brief Расшифровать методом XTEA-3
//! @param[in] dataIn исходные зашифрованные данные
//! @param[in] key ключ шифрования
//! @param[out] dataOut расшифрованные данные
//! @param[in] numRounds количество раундов шифрования
forward stock xtea3decrypt(const dataIn[XTEA3_DATA_CELLS_AMOUNT], const key[XTEA3_KEY_CELLS_AMOUNT], dataOut[XTEA3_DATA_CELLS_AMOUNT],
                            numRounds = XTEA3_NUM_ROUNDS_DEFAULT);
