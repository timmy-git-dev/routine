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

    void add_minute(const int   _minute) {minute = minute + _minute; while (minute >   59) {minute -=   60;} while (minute < 0) {minute +=   60;}}
    void add_hour  (const int   _hour  ) {hour   = hour   + _hour;   while (hour   >   23) {hour   -=   24;} while (hour   < 0) {hour   +=   24;}}
    void add_day   (const int   _day   ) {day    = day    + _day;    while (day    >   31) {day    -=   31;} while (day    < 1) {day    +=   31;}}
    void add_month (const int   _month ) {month  = month  + _month;  while (month  >   12) {month  -=   12;} while (month  < 1) {month  +=   12;}}
    void add_year  (const int   _year  ) {year   = year   + _year;   while (year   > 9999) {year   -= 9999;} while (year   < 0) {year   += 9999;}}
    void add_time  (const Time &_time  )
    {
        add_year  (_time.year  );
        add_month (_time.month );
        add_day   (_time.day   );
        add_hour  (_time.hour  );
        add_minute(_time.minute);
    }
    void sub_time  (const Time &_time  )
    {
        add_year  (-_time.year  );
        add_month (-_time.month );
        add_day   (-_time.day   );
        add_hour  (-_time.hour  );
        add_minute(-_time.minute);
    }

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