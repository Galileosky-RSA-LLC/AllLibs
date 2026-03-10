//! @file
//! @brief Заголовок библиотеки массива пользователя

#if defined USERARRAY_H
#endinput
#endif
#define USERARRAY_H

#include "..\gdefines.h"

//! @defgroup general Общая структура
//! @{
#define USERARRAY_MAX_SIZE TAG_USER_ARRAY_MAX_SIZE

#define USERARRAY_TYPE_POS 0
//! @}

//! @defgroup energyMeterMercury Счетчик электроэнергии "Меркурий"
//! @{
#define USERARRAY_EMETER_MERCURY_TYPE 0x02
#define USERARRAY_EMETER_MERCURY_TYPE_SIZE 1
//! @}

//! @defgroup tachograph Тахограф
//! @{
#define USERARRAY_TACHOGRAPH_TYPE 0x03
#define USERARRAY_TACHOGRAPH_TYPE_SIZE 1
//! @}

//! @defgroup sens Устройство "Сенс"
//! @{
#define USERARRAY_SENS_TYPE 0x04
#define USERARRAY_SENS_TYPE_SIZE 1
//! @}

//! @defgroup skvtm Устройство "СКВТ-М"
//! @{
#define USERARRAY_SKVTM_TYPE 0x05
#define USERARRAY_SKVTM_TYPE_SIZE 1
//! @}

//! @defgroup tpmsTd5300ax04 Система контроля давления в шинах "TD5300A-X-04"
//! @{
#define USERARRAY_TD5300AX04_TYPE 0x06
#define USERARRAY_TD5300AX04_TYPE_SIZE 1
//! @}

//! @defgroup flowMeterEurosensDelta Расходомер "Eurosens Delta"
//! @{
#define USERARRAY_FMETER_EUROSENS_DELTA_TYPE 0x07
#define USERARRAY_FMETER_EUROSENS_DELTA_TYPE_SIZE 1
//! @}

//! @defgroup llsFrequency Частота с ДУТ
//! @{
#define USERARRAY_LLSFREQ_TYPE 0x08
#define USERARRAY_LLSFREQ_TYPE_SIZE 1
//! @}

//! @defgroup passengersCounting Подсчет пассажиров
//! @{
#define USERARRAY_PASCOUNT_TYPE 0x0A
#define USERARRAY_PASCOUNT_TYPE_SIZE 1
#define USERARRAY_PASCOUNT_SENSOR_ADDR_POS 0
#define USERARRAY_PASCOUNT_SENSOR_ADDR_SIZE 3
#define USERARRAY_PASCOUNT_SENSOR_ADDR_MIN 0x00
#define USERARRAY_PASCOUNT_SENSOR_ADDR_MAX 0xFFFFFF
#define USERARRAY_PASCOUNT_SENSOR_STATE_POS (USERARRAY_PASCOUNT_SENSOR_ADDR_POS + USERARRAY_PASCOUNT_SENSOR_ADDR_SIZE)
#define USERARRAY_PASCOUNT_SENSOR_STATE_SIZE 1
#define USERARRAY_PASCOUNT_SENSOR_DATA_SIZE 2
#define USERARRAY_PASCOUNT_SENSOR_IN_POS (USERARRAY_PASCOUNT_SENSOR_STATE_POS + USERARRAY_PASCOUNT_SENSOR_STATE_SIZE)
#define USERARRAY_PASCOUNT_SENSOR_IN_SIZE USERARRAY_PASCOUNT_SENSOR_DATA_SIZE
#define USERARRAY_PASCOUNT_SENSOR_OUT_POS (USERARRAY_PASCOUNT_SENSOR_IN_POS + USERARRAY_PASCOUNT_SENSOR_IN_SIZE)
#define USERARRAY_PASCOUNT_SENSOR_OUT_SIZE USERARRAY_PASCOUNT_SENSOR_DATA_SIZE
#define USERARRAY_PASCOUNT_SENSOR_DATA_MIN 0
#define USERARRAY_PASCOUNT_SENSOR_DATA_MAX 65535
#define USERARRAY_PASCOUNT_SENSOR_SIZE (USERARRAY_PASCOUNT_SENSOR_OUT_POS + USERARRAY_PASCOUNT_SENSOR_OUT_SIZE)
#define USERARRAY_PASCOUNT_SENSORS_QTY_MAX 8
#define USERARRAY_PASCOUNT_SIZE_MAX (USERARRAY_PASCOUNT_TYPE_SIZE + (USERARRAY_PASCOUNT_SENSORS_QTY_MAX * USERARRAY_PASCOUNT_SENSOR_SIZE))
#define USERARRAY_PASCOUNT_DATA_POS_MIN USERARRAY_PASCOUNT_TYPE_SIZE
#define USERARRAY_PASCOUNT_DATA_POS_MAX (USERARRAY_PASCOUNT_SIZE_MAX - USERARRAY_PASCOUNT_SENSOR_SIZE)

#define USERARRAY_PASCOUNT_SENSOR_DATA [\
    .addr,\
    .state,\
    .incoming,\
    .outgoing,\
]
//! @}

//! @defgroup ctrlPanelMerkator Пульт "Меркатор"
//! @{
#define USERARRAY_CTRLPANEL_MERKATOR_TYPE 0x0B
#define USERARRAY_CTRLPANEL_MERKATOR_TYPE_SIZE 1
//! @}

//! @defgroup vims VIMS
//! @{
#define USERARRAY_VIMS_TYPE 0x0C
#define USERARRAY_VIMS_TYPE_SIZE 1
//! @}

//! @defgroup ela Датчики "ELA"
//! @{
#define USERARRAY_ELA_TYPE 0x0E
#define USERARRAY_ELA_TYPE_SIZE 1
//! @}

//! @defgroup ustruct Универсальная структура
//! @{
#define USERARRAY_USTRUCT_TYPE 0x7F
#define USERARRAY_USTRUCT_TYPE_SIZE 1
#define USERARRAY_USTRUCT_DESCR_SIZE 1
#define USERARRAY_USTRUCT_DESCR_SYMBOLS_MAX 127
#define USERARRAY_USTRUCT_DESCR_NUMBERS_MAX 8
//! @}

//! @defgroup file Передача файла
//! @{
#define USERARRAY_FILE_TYPE 0x80
#define USERARRAY_FILE_TYPE_LEN 1
#define USERARRAY_FILE_UID_POS (USERARRAY_TYPE_POS + USERARRAY_FILE_TYPE_LEN)
#define USERARRAY_FILE_UID_LEN 4
#define USERARRAY_FILE_NCHUNK_POS (USERARRAY_FILE_UID_POS + USERARRAY_FILE_UID_LEN)
#define USERARRAY_FILE_NCHUNK_LEN 3
#define USERARRAY_FILE_SIZE_POS (USERARRAY_FILE_NCHUNK_POS + USERARRAY_FILE_NCHUNK_LEN)
#define USERARRAY_FILE_SIZE_LEN 1
#define USERARRAY_FILE_HEADER_LEN (USERARRAY_FILE_TYPE_LEN + USERARRAY_FILE_UID_LEN + USERARRAY_FILE_NCHUNK_LEN + USERARRAY_FILE_SIZE_LEN)
#define USERARRAY_FILE_DATA_POS (USERARRAY_FILE_SIZE_POS + USERARRAY_FILE_SIZE_LEN)
#define USERARRAY_FILE_CRC_LEN 1
#define USERARRAY_FILE_FRAME_LEN_MIN (USERARRAY_FILE_HEADER_LEN + USERARRAY_FILE_CRC_LEN)
#define USERARRAY_FILE_CHUNK_LEN_MAX (USERARRAY_MAX_SIZE - USERARRAY_FILE_FRAME_LEN_MIN)
//! @}

//! @defgroup text Текст
//! @{
#define USERARRAY_TEXT_TYPE 0xEE
#define USERARRAY_TEXT_TYPE_LEN 1
#define USERARRAY_TEXT_PAYLOAD_POS USERARRAY_TEXT_TYPE_LEN
#define USERARRAY_TEXT_PAYLOAD_SIZE_MAX (USERARRAY_MAX_SIZE - USERARRAY_TEXT_TYPE_LEN)
//! @}

//! @addtogroup ustruct
//! @{

//! @brief Сформировать описатель для группы параметров массива пользователя универсальной структуры
//! @param[in] isNumbers признак числового типа данных (!=0 - числа, =0 - строка)
//! @param[in] amount количество элементов: для строки - длина в байтах (от 0 до 127), для чисел - от 1 до 8
//! @param[in] size размер памяти числа: 0, 1, 2, 4 или 8 (для строкового типа данных не учитывается)
forward stock ustructMakeDescriptor(isNumbers, amount, size);

//! @brief Сформировать описатель группы чисел в массиве пользователя универсальной структуры
//! @param[in] amount количество чисел, 1..8
//! @param[in] size размер памяти числа: 1, 2, 4 или 8 байт
forward stock ustructMakeDescriptorNum(amount, size);

//! @brief Сформировать описатель строки в массиве пользователя универсальной структуры
//! @param[in] strSize длина строки в байтах, 0..127
forward stock ustructMakeDescriptorStr(strSize);

//! @brief Пропустить неиспользуемые параметры в массиве пользователя универсальной структуры
//! @param[inout] userArray массив пользователя
//! @param[in] userArrayMaxSize предельный размер массива пользователя
//! @param[inout] pos позиция в массиве пользователя
//! @param[in] emptyCount количество пропускаемых параметров
//! @return количество вставленных байт (0 - ошибка)
forward stock ustructInsertEmpty(userArray{}, userArrayMaxSize, &pos, emptyCount);

//! @brief Вставить числа в массив пользователя универсальной структуры
//! @param[inout] userArray массив пользователя
//! @param[in] userArrayMaxSize предельный размер массива пользователя
//! @param[inout] pos позиция в массиве пользователя
//! @param[in] dimension размерность вставляемых чисел: 1,2,4
//! @param[in] numbers вставляемые числа
//! @param[in] numbersSize количество вставляемых чисел
//! @return количество вставленных байт (0 - ошибка)
forward stock ustructInsertNumbers(userArray{}, userArrayMaxSize, &pos, dimension, const numbers[], numbersSize = 1);
//! @}

//! @addtogroup file
//! @{

//! @brief Отправить файл с помощью массива пользователя
//! @param[in] fileName имя файла, должно оканчиваться \0
//! @param[inout] lastFileId идентификатор последнего отправленного файла
//! @return true - успешно, false - ошибка файловой системы: файл пустой или не прочитался
forward bool:stock sendFileInUserArray(const fileName{}, &lastFileId);
//! @}

//! @addtogroup passengersCounting
//! @{

//! @brief Вставить данные датчика пассажиропотока в массив пользователя
//! @param[inout] userArray массив пользователя
//! @param[in] userArrayMaxSize предельный размер массива пользователя
//! @param[in] sensor данные датчика
//! @param[inout] pos позиция в массиве пользователя
//! @return количество вставленных байт (0 - ошибка входных данных)
forward stock userArrayAddPasCounting(userArray{}, userArrayMaxSize, const sensor[USERARRAY_PASCOUNT_SENSOR_DATA], &pos);
//! @}

//! @addtogroup text
//! @{

//! @brief Записать текст в массив пользователя
//! @param[in] text записываемый текст
forward stock setTextInUserArray(const text{});
//! @}
