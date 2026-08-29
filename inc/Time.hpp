#pragma once
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
};