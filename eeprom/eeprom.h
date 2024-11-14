#ifndef EEPROM_H
#define EEPROM_H
//{ ============================================================================
//{ Заголовок библиотеки работы с EEPROM =======================================
//{ ============================================================================

//#define EEPROM_DEBUG // режим отладки


#define EEPROM_CONF_CMD_WAIT_MS 5000 // время ожидания конфигурационных команд при отсутствии доступа к ROM

#ifndef EEPROM_PARAMS_COUNT
#define EEPROM_PARAMS_COUNT 1 // общее количество сохраняемых параметров
#endif

#define EEPROM_PARAM_SIZE 4
#define EEPROM_KEY_BUFFER_MAX_SIZE 16
#define EEPROM_PARAMS_PER_KEY (EEPROM_KEY_BUFFER_MAX_SIZE / EEPROM_PARAM_SIZE)
#define EEPROM_KEYS_COUNT_MAX 32
#define EEPROM_PARAMS_COUNT_MAX (EEPROM_KEYS_COUNT_MAX * EEPROM_PARAMS_PER_KEY)
#if (EEPROM_PARAMS_COUNT > EEPROM_PARAMS_COUNT_MAX)
#undef EEPROM_PARAMS_COUNT
#define EEPROM_PARAMS_COUNT EEPROM_PARAMS_COUNT_MAX
#endif

#define EEPROM_KEYS_COUNT (EEPROM_PARAMS_COUNT / EEPROM_PARAMS_PER_KEY + (EEPROM_PARAMS_COUNT % EEPROM_PARAMS_PER_KEY != 0))
#define EEPROM_KEY_LAST (EEPROM_KEYS_COUNT - 1)

#define EEPROM_DATA [\
    .keyBuf[EEPROM_PARAMS_PER_KEY],\
    .keysVarAddr[EEPROM_PARAMS_COUNT],\
    .keysOldValue[EEPROM_PARAMS_COUNT],\
]

eepromInitParamValues(obj[EEPROM_DATA], const addr[], addrSize);
eepromRestoreParams(obj[EEPROM_DATA]);
eepromStoreParamsByChange(obj[EEPROM_DATA]);

// Пример использования
// mainT();

//!} Конец заголовка библиотеки работы с EEPROM ================================
//!} ===========================================================================
#endif // EEPROM_H