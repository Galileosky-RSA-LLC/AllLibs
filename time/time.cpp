#ifndef TIME_LIB
#define TIME_LIB
// Библиотека функций времени

#include "time.h"
#include "..\numeric\numeric.h"

//! Истек ли таймер с зафиксированного времени UPTIME
//! \param[in] fixedTime зафиксированное время от запуска операционной системы в мс
//! \param[in] timer таймер в мс
isTimerExpired(fixedTime, timer)
{
    new gUpt = GetVar(UPTIME);
    if (timer < 0)
        timer = 0;

	if (gUpt == fixedTime)
		return timer == 0;
    
    return (gUpt - fixedTime) >= 0 ? (gUpt - fixedTime) >= timer : true;
}

//! Задержать выполнение, 
//! более точно, чем Delay(), шаг 10 мс
wait(timer)
{
    waitFrom(GetVar(UPTIME), timer);
}

//! Задержать выполнение с зафиксированного времени
waitFrom(fixedTime, timer)
{
    while (!isTimerExpired(fixedTime, timer))
        Delay(10);
}

//! Разделить метку времени Unixtime на дату и время в отдельные переменные
//! \param[in] time метка времени Unixtime
//! \param[out] day день
//! \param[out] month месяц
//! \param[out] year год
//! \param[out] hour час
//! \param[out] minute минута
//! \param[out] second секунда
unixTime2dateTime(time, &year, &month, &day, &hour, &minute, &second)
{
    new i = 0;
    new thisYearSeconds = NONLEAP_YEAR_SECONDS;
    while (time >= thisYearSeconds)
    {
        i++;
        time -= thisYearSeconds;
        thisYearSeconds = NONLEAP_YEAR_SECONDS + ((i % 4 == 2) ? SECONDS_PER_DAY : 0);
    }
    year = UNIX_EPOCH_YEAR_START + i;
    new monthDays[] = [JANUARY_DAYS, FEBRUARY_NONLEAP_DAYS, MARCH_DAYS, APRIL_DAYS, MAY_DAYS, JUNE_DAYS, JULY_DAYS, AUGUST_DAYS, SEPTEMBER_DAYS, OCTOBER_DAYS,
                        NOVEMBER_DAYS, DECEMBER_DAYS];
    if (isLeapYear(year))
        monthDays[1] = FEBRUARY_LEAP_DAYS;

    new daysElapsed = time / SECONDS_PER_DAY;
    new day1 = daysElapsed;
    for (new i = 0; i <= sizeof monthDays - 1; i++)
    {
        if (day1 <= (monthDays[i] - 1))
        {
            month = i + 1;
            day = day1 + 1;
            break;
        }
        day1 -= monthDays[i];
    }
    new secondsFromMidnight = time - daysElapsed * SECONDS_PER_DAY;
    hour = secondsFromMidnight / SECONDS_PER_HOUR;
    new secondsFromHour = secondsFromMidnight - hour * SECONDS_PER_HOUR;
    minute = secondsFromHour / SECONDS_PER_MINUTE;
    second = secondsFromHour - minute * SECONDS_PER_MINUTE;
}

//! Проверить, что uptime1 < uptime2
uptimeLess(uptime1, uptime2)
{
    if (uptime1 == uptime2)
        return false;

    if (((uptime1 >= 0) && (uptime2 >= 0)) || ((uptime1 < 0) && (uptime2 < 0)))
        return uptime1 < uptime2;

    return (uptime1 >= 0) && (uptime2 < 0);
}

//! Преобразовать компоненты времени (UTC без смещения) в Unixtime
//! \param[in] day день
//! \param[in] month месяц
//! \param[in] year год
//! \param[in] hour час
//! \param[in] minute минута
//! \param[in] second секунда
//! \param[out] unixtime метка времени Unixtime
//! \return true - успешно, false - ошибка входных данных
dateTime2unixTime(year, month, day, hour, minute, second, &unixtime)
{
    new monthDays[] = [JANUARY_DAYS, FEBRUARY_NONLEAP_DAYS, MARCH_DAYS, APRIL_DAYS, MAY_DAYS, JUNE_DAYS, JULY_DAYS, AUGUST_DAYS, SEPTEMBER_DAYS, OCTOBER_DAYS,
                        NOVEMBER_DAYS, DECEMBER_DAYS];
    new leapYear = isLeapYear(year);
    if (leapYear)
        monthDays[1] = FEBRUARY_LEAP_DAYS;

    if ((hour < HOUR_MIN) || (hour > HOUR_MAX)
        || (minute < MINUTE_MIN) || (minute > MINUTE_MAX)
        || (second < SECOND_MIN) || (second > SECOND_MAX)
        || (month < MONTH_MIN) || (month > MONTH_MAX)
        || (day < DAY_MIN) || (day > monthDays[month - 1])
        || (year < UNIX_EPOCH_YEAR_START) || (year > UNIX_EPOCH_YEAR_END_32)
        || ((year == UNIX_EPOCH_YEAR_END_32) && ((month > UNIX_EPOCH_MONTH_END_32)
                                    || ((month == UNIX_EPOCH_MONTH_END_32) && ((day > UNIX_EPOCH_DAY_END_32)
                                                                    || ((day == UNIX_EPOCH_DAY_END_32) && ((hour > UNIX_EPOCH_HOUR_END_32)
                                                                                    || ((hour == UNIX_EPOCH_HOUR_END_32) && ((minute > UNIX_EPOCH_MINUTE_END_32)
                                                                                                            || ((minute == UNIX_EPOCH_MINUTE_END_32)
                                                                                                                && (second > UNIX_EPOCH_SECOND_END_32)))))))))))
        return false;

    new i;
    for (i = UNIX_EPOCH_YEAR_START; i < year; i++)
        unixtime += isLeapYear(i) ? LEAP_YEAR_DAYS : NONLEAP_YEAR_DAYS;

    for (i = 0; i < (month - 1); i++)
        unixtime += monthDays[i];
    
    unixtime += day - 1;
    unixtime *= HOURS_PER_DAY;
    unixtime += hour;
    unixtime *= MINUTES_PER_HOUR;
    unixtime += minute;
    unixtime *= SECONDS_PER_MINUTE;
    unixtime += second;
    return true;
}

//! Проверка года на високосность
isLeapYear(year)
{
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

//! Определить длительность в мс 
//! \param[in] uptimeStart время старта
//! \param[in] uptimeEnd время окончания
//! \return длительность в мс
duration(uptimeStart, uptimeEnd)
{
    return uptimeStart <= uptimeEnd ? uptimeEnd - uptimeStart : NUM_VALUE_MAX - uptimeStart + uptimeEnd;
}

#endif // TIME_LIB