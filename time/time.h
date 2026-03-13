//! @file
//! @brief Заголовок библиотеки времени

#if defined TIME_H
#endinput
#endif
#define TIME_H

//! @defgroup timeIntervalDurations Длительности временных промежутков
//! @{
#define MS_PER_SECOND 1000
#define SECONDS_PER_MINUTE 60
#define MS_PER_MINUTE (SECONDS_PER_MINUTE * MS_PER_SECOND)
#define MINUTES_PER_HOUR 60
#define SECONDS_PER_HOUR (MINUTES_PER_HOUR * SECONDS_PER_MINUTE)
#define MS_PER_HOUR (SECONDS_PER_HOUR * MS_PER_SECOND)
#define HOURS_PER_DAY 24
#define MINUTES_PER_DAY (HOURS_PER_DAY * MINUTES_PER_HOUR)
#define SECONDS_PER_DAY (MINUTES_PER_DAY * SECONDS_PER_MINUTE)
#define MONTHS_PER_YEAR 12
#define MS_PER_DAY (HOURS_PER_DAY * MS_PER_HOUR)
//! @}

//! @defgroup timeStampParams Параметры временных меток
//! @{
#define HOUR_MIN 0
#define HOUR_MAX (HOURS_PER_DAY - 1)
#define MINUTE_MIN 0
#define MINUTE_MAX (MINUTES_PER_HOUR - 1)
#define SECOND_MIN 0
#define SECOND_MAX (SECONDS_PER_MINUTE - 1)
#define MONTH_MIN 1
#define MONTH_MAX MONTHS_PER_YEAR
#define DAY_MIN 1
//! @}

//! @defgroup monthYearDurations Длительности месяцев и лет
//! @{
#define LEAP_YEAR_ADD_DAY 1

#define JANUARY_DAYS 31
#define FEBRUARY_NONLEAP_DAYS 28
#define FEBRUARY_LEAP_DAYS (FEBRUARY_NONLEAP_DAYS + LEAP_YEAR_ADD_DAY)
#define MARCH_DAYS 31
#define APRIL_DAYS 30
#define MAY_DAYS 31
#define JUNE_DAYS 30
#define JULY_DAYS 31
#define AUGUST_DAYS 31
#define SEPTEMBER_DAYS 30
#define OCTOBER_DAYS 31
#define NOVEMBER_DAYS 30
#define DECEMBER_DAYS 31

#define NONLEAP_YEAR_DAYS (JANUARY_DAYS + FEBRUARY_NONLEAP_DAYS + MARCH_DAYS + APRIL_DAYS + MAY_DAYS + JUNE_DAYS + JULY_DAYS + AUGUST_DAYS + SEPTEMBER_DAYS + \
                            OCTOBER_DAYS + NOVEMBER_DAYS + DECEMBER_DAYS)
#define LEAP_YEAR_DAYS (NONLEAP_YEAR_DAYS + LEAP_YEAR_ADD_DAY)

#define NONLEAP_YEAR_SECONDS (NONLEAP_YEAR_DAYS * SECONDS_PER_DAY)
//! @}

//! @defgroup unixEpoch Эпоха Unix
//! @{
#define UNIX_EPOCH_YEAR_START 1970
#define UNIX_EPOCH_YEAR_END_32BIT 2038
#define UNIX_EPOCH_MONTH_END_32BIT 1
#define UNIX_EPOCH_DAY_END_32BIT 19
#define UNIX_EPOCH_HOUR_END_32BIT 3
#define UNIX_EPOCH_MINUTE_END_32BIT 14
#define UNIX_EPOCH_SECOND_END_32BIT 7
//! @}

#define DATETIME[\
    .year,\
    .month,\
    .day,\
    .hour,\
    .minute,\
    .second,\
]

//! @brief Проверить, истек ли таймер с зафиксированного времени
//! @param[in] fixedUptime зафиксированное время от старта терминала в мс
//! @param[in] timerMs таймер в мс
forward bool:stock isTimerExpired(fixedUptime, timerMs);

//! @brief Задержать выполнение
//! @details Более точно, чем Delay() - шаг 10 мс
forward stock wait(timerMs);

//! @brief Задержать выполнение с зафиксированного времени
forward stock waitFrom(fixedUptime, timerMs);

//! @brief Разделить метку времени Unixtime на компоненты даты и времени суток
//! @param[in] unixtime метка времени Unixtime
//! @param[out] datetime структура даты и времени
//! @return true - успешно, false - ошибка входных данных
forward bool:stock unixTime2dateTime(unixtime, datetime[DATETIME]);

//! @brief Преобразовать компоненты даты и времени суток (UTC без смещения) в метку времени Unixtime
//! @param[in] datetime структура даты и времени
//! @param[out] unixtime метка времени Unixtime
//! @return true - успешно, false - ошибка входных данных
forward bool:stock dateTime2unixTime(const datetime[DATETIME], &unixtime);

//! @brief Проверить год на високосность
forward bool:stock isLeapYear(year);

//! @brief Определить длительность в мс
//! @param[in] uptimeStart время старта
//! @param[in] uptimeEnd время окончания
//! @param[out] overflows количество переполнений значения cellmax
//! @return остаточная длительность после последнего переполнения, т.е. итоговая длительность: overflows * cellmax + return
forward stock durationMs(uptimeStart, uptimeEnd, &overflows = 0);

//! @brief Вычислить оставшееся время в мс до конца таймера
//! @param[in] startUptime время старта
//! @param[in] timerMs таймер в мс
//! @return оставшаяся длительность
forward stock remainingUptimeMs(startUptime, timerMs);
