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

stock unixTime2dateTime(unixtime, &year, &month, &day, &hour, &minute, &second)
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
    year = UNIX_EPOCH_YEAR_START + yearsFromStart;
    new monthDays[] = [JANUARY_DAYS, FEBRUARY_NONLEAP_DAYS, MARCH_DAYS, APRIL_DAYS, MAY_DAYS, JUNE_DAYS, JULY_DAYS, AUGUST_DAYS, SEPTEMBER_DAYS, OCTOBER_DAYS,
                        NOVEMBER_DAYS, DECEMBER_DAYS];
    if (isLeapYear(year))
        monthDays[1] = FEBRUARY_LEAP_DAYS;

    new const daysElapsed = unixtime / SECONDS_PER_DAY;
    new totalDays = daysElapsed;
    for (new i = 0; i < sizeof(monthDays); i++)
    {
        if (totalDays < monthDays[i])
        {
            month = i + 1;
            day = totalDays + 1;
            break;
        }
        totalDays -= monthDays[i];
    }
    new const secondsFromMidnight = unixtime - (daysElapsed * SECONDS_PER_DAY);
    hour = secondsFromMidnight / SECONDS_PER_HOUR;
    new const secondsFromHour = secondsFromMidnight - (hour * SECONDS_PER_HOUR);
    minute = secondsFromHour / SECONDS_PER_MINUTE;
    second = secondsFromHour - (minute * SECONDS_PER_MINUTE);
    return true;
}

stock bool:uptimeLess(uptime1, uptime2)
{
    if (uptime1 == uptime2)
        return false;

    if (((uptime1 >= 0) && (uptime2 >= 0)) || ((uptime1 < 0) && (uptime2 < 0)))
        return uptime1 < uptime2;

    return (uptime1 >= 0) && (uptime2 < 0);
}

stock bool:dateTime2unixTime(year, month, day, hour, minute, second, &unixtime)
{
    new monthDays[] = [JANUARY_DAYS, FEBRUARY_NONLEAP_DAYS, MARCH_DAYS, APRIL_DAYS, MAY_DAYS, JUNE_DAYS, JULY_DAYS, AUGUST_DAYS, SEPTEMBER_DAYS, OCTOBER_DAYS,
                        NOVEMBER_DAYS, DECEMBER_DAYS];
    if (isLeapYear(year))
        monthDays[1] = FEBRUARY_LEAP_DAYS;

    if ((hour < HOUR_MIN) || (hour > HOUR_MAX)
        || (minute < MINUTE_MIN) || (minute > MINUTE_MAX)
        || (second < SECOND_MIN) || (second > SECOND_MAX)
        || (month < MONTH_MIN) || (month > MONTH_MAX)
        || (day < DAY_MIN) || (day > monthDays[month - 1])
        || (year < UNIX_EPOCH_YEAR_START) || (year > UNIX_EPOCH_YEAR_END_32BIT))
        return false;

    if (year == UNIX_EPOCH_YEAR_END_32BIT)
    {
        if (month > UNIX_EPOCH_MONTH_END_32BIT)
            return false;

        if (month == UNIX_EPOCH_MONTH_END_32BIT)
        {
            if (day > UNIX_EPOCH_DAY_END_32BIT)
                return false;
            
            if (day == UNIX_EPOCH_DAY_END_32BIT)
            {
                if (hour > UNIX_EPOCH_HOUR_END_32BIT)
                    return false;
                
                if (hour == UNIX_EPOCH_HOUR_END_32BIT)
                {
                    if (minute > UNIX_EPOCH_MINUTE_END_32BIT)
                        return false;

                    if ((minute == UNIX_EPOCH_MINUTE_END_32BIT) && (second > UNIX_EPOCH_SECOND_END_32BIT))
                        return false;
                }
            }
        }
    }
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

stock bool:isLeapYear(year)
{
    return (((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0);
}

stock duration(uptimeStart, uptimeEnd)
{
    return uptimeStart <= uptimeEnd ? uptimeEnd - uptimeStart : cellmax - uptimeStart + uptimeEnd;
}
