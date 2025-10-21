#ifdef EEPROM_H
#endinput
#endif
#define EEPROM_H

//! @file
//! @brief Заголовок библиотеки работы с EEPROM

#include "..\numeric\numeric.h"

//#define EEPROM_DEBUG // включить для отладки

#define EEPROM_CONF_CMD_WAIT_MS 5000 //!< время ожидания конфигурационных команд при отсутствии доступа к ROM

#ifndef EEPROM_PARAMS_COUNT
#define EEPROM_PARAMS_COUNT 0 //!< общее количество сохраняемых параметров, определить на верхнем уровне
#endif

#define EEPROM_PARAM_SIZE CELL_BYTES
#define EEPROM_KEY_BUFFER_MAX_SIZE 16
#define EEPROM_PARAMS_PER_KEY (EEPROM_KEY_BUFFER_MAX_SIZE / EEPROM_PARAM_SIZE)
#define EEPROM_KEYS_COUNT_MAX 32
#define EEPROM_PARAMS_COUNT_MAX (EEPROM_KEYS_COUNT_MAX * EEPROM_PARAMS_PER_KEY)
#if ((EEPROM_PARAMS_COUNT < 1) || (EEPROM_PARAMS_COUNT > EEPROM_PARAMS_COUNT_MAX))
#error "EEPROM_PARAMS_COUNT wrong"
#endif
#if (EEPROM_PARAMS_COUNT == 1)
#define EEPROM_PARAMS_ADD_COUNT 2
#else
#define EEPROM_PARAMS_ADD_COUNT EEPROM_PARAMS_COUNT
#endif
#define EEPROM_KEYS_COUNT COUNT_USED_CELLS(EEPROM_PARAMS_COUNT, EEPROM_PARAMS_PER_KEY)
#define EEPROM_KEY_LAST (EEPROM_KEYS_COUNT - 1)

#define EEPROM_DATA [\
    .keyBuf[EEPROM_PARAMS_PER_KEY],\
    .keysVarAddr[EEPROM_PARAMS_ADD_COUNT],\
    .keysOldValue[EEPROM_PARAMS_ADD_COUNT],\
]

//! @brief Инициализировать из адресов глобальных переменных значениями по умолчанию
//! @param[out] obj инициализируемые данные
//! @param[in] addr адреса глобальных переменных
//! @param[in] addrSize количество адресов глобальных переменных
forward stock eepromInitParamValues(obj[EEPROM_DATA], const addr[], addrSize);

//! @brief Восстановить сохраненные параметры
forward stock eepromRestoreParams(obj[EEPROM_DATA]);

//! @brief Сохранить параметры по их изменению
forward stock eepromStoreParamsByChange(obj[EEPROM_DATA]);
