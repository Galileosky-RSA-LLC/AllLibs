#if defined TIME_LIB
#endinput
#endif
#define TIME_LIB

//! @file
//! @brief Реализация библиотеки времени

#include "time.h"
#include "..\numeric\numeric.h"

stock isTimerExpired(fixedUptime, timer)
{
    new gUpt = GetVar(UPTIME);
    if (timer < 0)
        timer = 0;

	if (gUpt == fixedUptime)
		return timer == 0;
    
    return (gUpt - fixedUptime) >= 0 ? (gUpt - fixedUptime) >= timer : true;
}

stock wait(timer)
{
    waitFrom(GetVar(UPTIME), timer);
}

stock waitFrom(fixedUptime, timer)
{
    while (!isTimerExpired(fixedUptime, timer))
        Delay(10);
}

stock unixTime2dateTime(time, &year, &month, &day, &hour, &minute, &second)
{
    new yearsFromStart = 0;
    new thisYearSeconds = NONLEAP_YEAR_SECONDS;
    while (time >= thisYearSeconds)
    {
        yearsFromStart++;
        time -= thisYearSeconds;
        thisYearSeconds = NONLEAP_YEAR_SECONDS + (((yearsFromStart % 4) == 2) ? SECONDS_PER_DAY : 0);
    }
    year = UNIX_EPOCH_YEAR_START + yearsFromStart;
    new monthDays[] = [JANUARY_DAYS, FEBRUARY_NONLEAP_DAYS, MARCH_DAYS, APRIL_DAYS, MAY_DAYS, JUNE_DAYS, JULY_DAYS, AUGUST_DAYS, SEPTEMBER_DAYS, OCTOBER_DAYS,
                        NOVEMBER_DAYS, DECEMBER_DAYS];
    if (isLeapYear(year))
        monthDays[1] = FEBRUARY_LEAP_DAYS;

    new daysElapsed = time / SECONDS_PER_DAY;
    new day1 = daysElapsed;
    for (new i = 0; i < sizeof(monthDays); i++)
    {
        if (day1 < monthDays[i])
        {
            month = i + 1;
            day = day1 + 1;
            break;
        }
        day1 -= monthDays[i];
    }
    new secondsFromMidnight = time - (daysElapsed * SECONDS_PER_DAY);
    hour = secondsFromMidnight / SECONDS_PER_HOUR;
    new secondsFromHour = secondsFromMidnight - (hour * SECONDS_PER_HOUR);
    minute = secondsFromHour / SECONDS_PER_MINUTE;
    second = secondsFromHour - (minute * SECONDS_PER_MINUTE);
}

stock uptimeLess(uptime1, uptime2)
{
    if (uptime1 == uptime2)
        return false;

    if (((uptime1 >= 0) && (uptime2 >= 0)) || ((uptime1 < 0) && (uptime2 < 0)))
        return uptime1 < uptime2;

    return (uptime1 >= 0) && (uptime2 < 0);
}

stock dateTime2unixTime(year, month, day, hour, minute, second, &unixtime)
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

stock isLeapYear(year)
{
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

stock duration(uptimeStart, uptimeEnd)
{
    return uptimeStart <= uptimeEnd ? uptimeEnd - uptimeStart : cellmax - uptimeStart + uptimeEnd;
}
