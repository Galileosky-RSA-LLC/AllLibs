#ifndef SETTING_H
#define SETTING_H
//{ ============================================================================
//{ Заголовок библиотеки работы с настройками ==================================
//{ ============================================================================

//#define SETTING_DEBUG // режим отладки

#define SETTING_CONF_CMD_WAIT_MS 5000 // время ожидания конфигурационных команд при отсутствии доступа к файловой системе

#ifndef SETTING_PARAMS_AMOUNT
#define SETTING_PARAMS_AMOUNT 1 // общее количество сохраняемых параметров
#endif

#define SETTING_FILENAME_SIZE_MAX 255
#define SETTING_FILENAME_SIZE_MAX_WITH_0 (SETTING_FILENAME_SIZE_MAX + 1)

#define SETTING_PARAM_SIZE 4
#define SETTING_BUF_SIZE (SETTING_PARAMS_AMOUNT * SETTING_PARAM_SIZE)

#define SETTING_DATA [\
    .fileName{SETTING_FILENAME_SIZE_MAX_WITH_0},\
    .varAddr{SETTING_BUF_SIZE},\
    .oldValue{SETTING_BUF_SIZE},\
]

#define SETTING_SINGLE_DATA [\
    .fileName{SETTING_FILENAME_SIZE_MAX_WITH_0},\
    .varAddr,\
    .oldValue,\
]

settingInit(obj[SETTING_DATA], const fileName{}, const varAddresses[], varAddressesSize);
settingRestoreParams(obj[SETTING_DATA]);
settingStoreParamsByChange(obj[SETTING_DATA]);
settingSingleInit(obj[SETTING_SINGLE_DATA], const fileName{}, varAddress);
settingSingleRestoreParam(obj[SETTING_SINGLE_DATA]);
settingSingleStoreParamByChange(obj[SETTING_SINGLE_DATA]);

//!} Конец заголовка библиотеки работы с настройками ===========================
//!} ===========================================================================
#endif // SETTING_H