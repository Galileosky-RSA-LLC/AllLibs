#ifndef ASYNC_H
#define ASYNC_H

// Библиотека асинхронных функций

// коды возврата
#define ASYNC_RET_OK 1 // успешное завершение, значение получено
#define ASYNC_RET_BUSY 2 // не завершено, необходим повторный вызов
#define ASYNC_RET_NODATA 0 // завершено, нет данных
#define ASYNC_RET_ERROR -1 // завершено с ошибкой

#endif