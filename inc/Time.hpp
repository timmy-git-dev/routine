#pragma once
#include <ctime>
#include <tuple>

struct Time
{
    int year;
    int month;
    int day;
    int hour;
    int minute;

    bool operator> (const Time& _time) const {return std::tie(year, month, day, hour, minute) >  std::tie(_time.year, _time.month, _time.day, _time.hour, _time.minute);}
    bool operator< (const Time& _time) const {return std::tie(year, month, day, hour, minute) <  std::tie(_time.year, _time.month, _time.day, _time.hour, _time.minute);}
    bool operator==(const Time& _time) const {return std::tie(year, month, day, hour, minute) == std::tie(_time.year, _time.month, _time.day, _time.hour, _time.minute);}

    static Time now()
    {
        std::time_t _now = std::time(nullptr);
        std::tm _local{};
        localtime_r(&_now, &_local);

        return Time
        {
            .year   = _local.tm_year + 1900,
            .month  = _local.tm_mon  + 1,
            .day    = _local.tm_mday,
            .hour   = _local.tm_hour,
            .minute = _local.tm_min,
        };
    }
};