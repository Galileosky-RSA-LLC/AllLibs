#ifdef SETTING_H
#endinput
#endif
#define SETTING_H

//! @file
//! @brief Заголовок библиотеки файловых настроек

#include "..\numeric\numeric.h"
#include "..\file\file.h"

//#define SETTING_DEBUG // включить для отладки

#define SETTING_CONF_CMD_WAIT_MS 5000 //!< время ожидания конфигурационных команд при отсутствии доступа к файловой системе

#define SETTING_PARAM_SIZE CELL_BYTES
#ifndef SETTING_PARAMS_AMOUNT
#define SETTING_PARAMS_AMOUNT 0 //!< общее количество сохраняемых параметров, определить на верхнем уровне
#endif
#if (SETTING_PARAMS_AMOUNT < 1)
#error "SETTING_PARAMS_AMOUNT wrong"
#elseif (SETTING_PARAMS_AMOUNT == 1)
#define SETTING_PARAMS_ADD_AMOUNT 2
#else
#define SETTING_PARAMS_ADD_AMOUNT SETTING_PARAMS_AMOUNT
#endif
#define SETTING_BUF_SIZE (SETTING_PARAMS_AMOUNT * SETTING_PARAM_SIZE)

#define SETTING_FILENAME_SIZE_MAX FILE_FULL_PATH_SIZE_MAX
#define SETTING_FILENAME_SIZE_MAX_WITH_0 (SETTING_FILENAME_SIZE_MAX + 1)

#define SETTING_DATA [\
    .fileName{SETTING_FILENAME_SIZE_MAX_WITH_0},\
    .varAddr[SETTING_PARAMS_ADD_AMOUNT],\
    .oldValue[SETTING_PARAMS_ADD_AMOUNT],\
]

#define SETTING_SINGLE_DATA [\
    .fileName{SETTING_FILENAME_SIZE_MAX_WITH_0},\
    .varAddr,\
    .oldValue,\
]

//! @brief Инициализировать объект настроек
//! @param[out] obj инициализируемая структура данных
//! @param[in] fileName имя файла с настройками, должно оканчиваться \0
//! @param[in] varAddresses адреса глобальных переменных с настройками
//! @param[in] varAddressesSize количество адресов
forward stock settingInit(obj[SETTING_DATA], const fileName{}, const varAddresses[], varAddressesSize);

//! @brief Загрузить сохраненные параметры из файла в глобальные переменные
forward stock settingRestoreParams(obj[SETTING_DATA]);

//! @brief Сохранить параметры по их изменению
//! @details Сначала проверяется изменение текущих значений глобальных переменных по отношению к предыдущему вызову
forward stock settingStoreParamsByChange(obj[SETTING_DATA]);

//! @brief Инициализировать изолированный параметр, хранящийся в отдельном файле
//! @param[out] obj инициализируемая структура данных
//! @param[in] fileName имя файла с настройкой, должно оканчиваться \0
//! @param[in] varAddresses адрес глобальной переменной для данной настройки
forward stock settingSingleInit(obj[SETTING_SINGLE_DATA], const fileName{}, varAddress);

//! @brief Восстановить значение изолированного параметра из файла
forward stock settingSingleRestoreParam(obj[SETTING_SINGLE_DATA]);

//! @brief Сохранить параметр при его изменении
//! @details Текущее значение сравнивается со значением с предыдущего вызова
forward stock settingSingleStoreParamByChange(obj[SETTING_SINGLE_DATA]);
