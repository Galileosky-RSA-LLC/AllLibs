//! @file
//! @brief Реализация библиотеки времени

#if defined TIME_LIB
#endinput
#endif
#define TIME_LIB

#include "time.h"

stock bool:isTimerExpired(fixedUptime, timerMs)
{
    new const gUpt = GetVar(UPTIME);
    if (timerMs < 0)
        timerMs = 0;

	new const elapsed = gUpt - fixedUptime;
    if (elapsed == 0)
		return timerMs == 0;
    
    return (elapsed < 0) || (elapsed >= timerMs);
}

stock wait(timerMs)
{
    waitFrom(GetVar(UPTIME), timerMs);
}

stock waitFrom(fixedUptime, timerMs)
{
    while (!isTimerExpired(fixedUptime, timerMs))
        Delay(10);
}

stock bool:unixTime2dateTime(unixtime, datetime[DATETIME])
{
    if (unixtime < 0)
        return false;

    new yearsFromStart = 0;
    new thisYearSeconds = NONLEAP_YEAR_SECONDS;
    while (unixtime >= thisYearSeconds)
    {
        yearsFromStart++;
        unixtime -= thisYearSeconds;
        thisYearSeconds = NONLEAP_YEAR_SECONDS + (((yearsFromStart % 4) == 2) ? SECONDS_PER_DAY : 0);
    }
    datetime.year = UNIX_EPOCH_YEAR_START + yearsFromStart;
    new monthDays[] = [JANUARY_DAYS, FEBRUARY_NONLEAP_DAYS, MARCH_DAYS, APRIL_DAYS, MAY_DAYS, JUNE_DAYS, JULY_DAYS, AUGUST_DAYS, SEPTEMBER_DAYS, OCTOBER_DAYS,
                        NOVEMBER_DAYS, DECEMBER_DAYS];
    if (isLeapYear(datetime.year))
        monthDays[1] = FEBRUARY_LEAP_DAYS;

    new const daysElapsed = unixtime / SECONDS_PER_DAY;
    new totalDays = daysElapsed;
    for (new i = 0; i < sizeof(monthDays); i++)
    {
        if (totalDays < monthDays[i])
        {
            datetime.month = i + 1;
            datetime.day = totalDays + 1;
            break;
        }
        totalDays -= monthDays[i];
    }
    new const secondsFromMidnight = unixtime - (daysElapsed * SECONDS_PER_DAY);
    datetime.hour = secondsFromMidnight / SECONDS_PER_HOUR;
    new const secondsFromHour = secondsFromMidnight - (datetime.hour * SECONDS_PER_HOUR);
    datetime.minute = secondsFromHour / SECONDS_PER_MINUTE;
    datetime.second = secondsFromHour - (datetime.minute * SECONDS_PER_MINUTE);
    return true;
}

stock bool:dateTime2unixTime(const datetime[DATETIME], &unixtime)
{
    new monthDays[] = [JANUARY_DAYS, FEBRUARY_NONLEAP_DAYS, MARCH_DAYS, APRIL_DAYS, MAY_DAYS, JUNE_DAYS, JULY_DAYS, AUGUST_DAYS, SEPTEMBER_DAYS, OCTOBER_DAYS,
                        NOVEMBER_DAYS, DECEMBER_DAYS];
    if (isLeapYear(datetime.year))
        monthDays[1] = FEBRUARY_LEAP_DAYS;

    if ((datetime.hour < HOUR_MIN) || (datetime.hour > HOUR_MAX)
        || (datetime.minute < MINUTE_MIN) || (datetime.minute > MINUTE_MAX)
        || (datetime.second < SECOND_MIN) || (datetime.second > SECOND_MAX)
        || (datetime.month < MONTH_MIN) || (datetime.month > MONTH_MAX)
        || (datetime.day < DAY_MIN) || (datetime.day > monthDays[datetime.month - 1])
        || (datetime.year < UNIX_EPOCH_YEAR_START) || (datetime.year > UNIX_EPOCH_YEAR_END_32BIT))
        return false;

    if (datetime.year == UNIX_EPOCH_YEAR_END_32BIT)
    {
        if (datetime.month > UNIX_EPOCH_MONTH_END_32BIT)
            return false;

        if (datetime.month == UNIX_EPOCH_MONTH_END_32BIT)
        {
            if (datetime.day > UNIX_EPOCH_DAY_END_32BIT)
                return false;
            
            if (datetime.day == UNIX_EPOCH_DAY_END_32BIT)
            {
                if (datetime.hour > UNIX_EPOCH_HOUR_END_32BIT)
                    return false;
                
                if (datetime.hour == UNIX_EPOCH_HOUR_END_32BIT)
                {
                    if (datetime.minute > UNIX_EPOCH_MINUTE_END_32BIT)
                        return false;

                    if ((datetime.minute == UNIX_EPOCH_MINUTE_END_32BIT) && (datetime.second > UNIX_EPOCH_SECOND_END_32BIT))
                        return false;
                }
            }
        }
    }
    new i;
    for (i = UNIX_EPOCH_YEAR_START; i < datetime.year; i++)
        unixtime += isLeapYear(i) ? LEAP_YEAR_DAYS : NONLEAP_YEAR_DAYS;

    for (i = 0; i < (datetime.month - 1); i++)
        unixtime += monthDays[i];
    
    unixtime += datetime.day - 1;
    unixtime *= HOURS_PER_DAY;
    unixtime += datetime.hour;
    unixtime *= MINUTES_PER_HOUR;
    unixtime += datetime.minute;
    unixtime *= SECONDS_PER_MINUTE;
    unixtime += datetime.second;
    return true;
}

stock bool:isLeapYear(year)
{
    return (((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0);
}

stock durationMs(uptimeStart, uptimeEnd, &overflows = 0)
{
    new dur = uptimeEnd - uptimeStart;
    if (dur >= 0)
    {    
        overflows = 0;
        return dur;
    }
    if (dur == -1)
    {
        overflows = 2;
        return 1;
    }
    overflows = 1;
    return dur - cellmax;
}
