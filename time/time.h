#ifndef TIME_H
#define TIME_H
// Библиотека функций времени

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

#define HOUR_MIN 0
#define HOUR_MAX (HOURS_PER_DAY - 1)
#define MINUTE_MIN 0
#define MINUTE_MAX (MINUTES_PER_HOUR - 1)
#define SECOND_MIN 0
#define SECOND_MAX (SECONDS_PER_MINUTE - 1)
#define MONTH_MIN 1
#define MONTH_MAX MONTHS_PER_YEAR
#define DAY_MIN 1

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

#define UNIX_EPOCH_YEAR_START 1970
#define UNIX_EPOCH_YEAR_END_32 2038
#define UNIX_EPOCH_MONTH_END_32 1
#define UNIX_EPOCH_DAY_END_32 19
#define UNIX_EPOCH_HOUR_END_32 3
#define UNIX_EPOCH_MINUTE_END_32 14
#define UNIX_EPOCH_SECOND_END_32 7


isTimerExpired(fixedTime, timer);
uptimeLess(uptime1, uptime2);
wait(timer);
waitFrom(fixedTime, timer);
unixTime2dateTime(time, &year, &month, &day, &hour, &minute, &second);
dateTime2unixTime(year, month, day, hour, minute, second, &unixtime);
isLeapYear(year);
duration(uptimeStart, uptimeEnd);

#endif // TIME_LIB