//! @file
//! @brief Заголовок библиотеки последовательных портов

#if defined RSPORT_H
#endinput
#endif
#define RSPORT_H

//! @defgroup portNums Номера портов
//! @{
#define RS232_0_PORT_NUM 0
#define RS232_1_PORT_NUM 1
#define RS485_PORT_NUM 2

#define RSPORT_NUM_MIN RS232_0_PORT_NUM
#define RSPORT_NUM_MAX RS485_PORT_NUM
//! @}

//! @defgroup baudRates Скорости передачи данных
//! @{
#define RSPORT_BAUD_RATE_MIN 1
//! @}

//! @defgroup buffer Буфер
//! @{
#define RSPORT_BUFFER_SIZE_MIN 1
//! @}

//! @defgroup stopBits Стоповые биты

//! @defgroup stopBitsPortInit Значения стоповых бит при инициализации порта
//! @ingroup stopBits
//! @{
#define PORTINIT_STOP_BITS_ONE 0
#define PORTINIT_STOP_BITS_TWO 1
//! @}

//! @defgroup stopBitsQuantity Количество стоповых бит
//! @ingroup stopBits
//! @{
#define RSPORT_STOP_BITS_QTY_MIN 1
#define RSPORT_STOP_BITS_QTY_MAX 2
//! @}

//! @defgroup parity Четность
//! @{
#define RSPORT_PARITY_NONE 0
#define RSPORT_PARITY_ODD 1
#define RSPORT_PARITY_EVEN 2

#define RSPORT_PARITY_MIN RSPORT_PARITY_NONE
#define RSPORT_PARITY_MAX RSPORT_PARITY_EVEN
//! @}
