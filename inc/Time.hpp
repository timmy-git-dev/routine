#pragma once

#include <ctime>
#include <tuple>
struct Time
{
    // unsigned long date;

    int year;
    int month;
    int day;
    int hour;
    int minute;

    bool operator> (const Time& _time) const {return std::tie(year, month, day, hour, minute) >  std::tie(_time.year, _time.month, _time.day, _time.hour, _time.minute);}
    bool operator< (const Time& _time) const {return std::tie(year, month, day, hour, minute) <  std::tie(_time.year, _time.month, _time.day, _time.hour, _time.minute);}
    bool operator==(const Time& _time) const {return std::tie(year, month, day, hour, minute) == std::tie(_time.year, _time.month, _time.day, _time.hour, _time.minute);}

    void update()
    {
        hour   +=  minute < 0 ? (minute - 59) / 60 : minute / 60;
        minute -= (minute < 0 ? (minute - 59) / 60 : minute / 60) * 60;

        day    +=  hour   < 0 ? (hour   - 23) / 24 : hour   / 24;
        hour   -= (hour   < 0 ? (hour   - 23) / 24 : hour   / 24) * 24;

        month  +=  day    < 0 ? (day    - 30) / 31 : day    / 31;
        day    -= (day    < 0 ? (day    - 30) / 31 : day    / 31) * 31;

        year   +=  month  < 0 ? (month  - 11) / 12 : month  / 12;
        month  -= (month  < 0 ? (month  - 11) / 12 : month  / 12) * 12;
    }


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