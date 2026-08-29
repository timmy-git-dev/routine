#pragma once
#include <algorithm>
#include <ctime>
// #include "Time.hpp"

struct Time
{
    using i64 =          long long;
    using i32 =          int;
    using u64 = unsigned long long;
    using u32 = unsigned int;

private:
    u64 date_;

    static constexpr u32 DAYS_BEFORE_MONTH[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    static constexpr bool is_leap         (u32 _year) {                               return _year % 4 == 0 && (_year % 100 != 0 || _year % 400 == 0);}
    static constexpr int  days_before_year(u32 _year) {if (_year-- == 0) {_year = 0;} return _year * 365 + _year / 4 - _year / 100 + _year / 400;     }

    u32 year() const
    {
        u32 _days     = date_    / 1440;

        u32 _400Eras  = _days    / 146097;
        u32 _400Days  = _days    % 146097;
        u32 _100Years = _400Days / 36524;
        u32 _100Days  = _400Days % 36524;
        u32 _4Eras    = _100Days / 1461;
        u32 _4Days    = _100Days % 1461;
        u32 _4Years   = _4Days   / 365;

        return _400Eras * 400 + std::min(_100Years, 3u) * 100 + _4Eras * 4 + std::min(_4Years, 3u);
    }
    u32 month() const
    {
        u32 _year = this->year();
        u32 _day  = date_ / 1440 - days_before_year(_year);

        if (is_leap(_year) && _day >= 59)
        {
            ++_day;
        }

        for (size_t _month = 1; _month < 12; ++_month)
        {
            if (_day < DAYS_BEFORE_MONTH[_month])
            {
                return _month;
            }
        }

        return 12;
    }
    u32 day() const
    {
        u32 _year  = this->year ();
        u32 _month = this->month();
        u32 _day  = date_ / 1440 - days_before_year(_year) + 1;

        return _day - DAYS_BEFORE_MONTH[_month - 1] - (is_leap(_year) && _month > 2);
    }
    u32 hour() const
    {
        return (date_ % 1440) / 60;
    }
    u32 minute() const
    {
        return date_ % 60;
    }

    void add_year  (i32 _years  ) {date_ += _years   * ;};
    void add_month (i32 _months ) {date_ += _months  * DAYS_BEFORE_MONTH[month()] * 1440;};
    void add_day   (i32 _days   ) {date_ += _days    * 1440;};
    void add_hour  (i32 _hours  ) {date_ += _hours   * 60;};
    void add_minute(i32 _minutes) {date_ += _minutes;   };

    Time(int _year, int _month, int _day, int _hour, int _minute) :
        date_((days_before_year(_year) + DAYS_BEFORE_MONTH[_month - 1] + (is_leap(_year) && _month > 2) + _day - 1) * 1440 + _hour * 60 + _minute)
    { }

    bool operator> (const Time& _time) const { return date_ >  _time.date_; }
    bool operator< (const Time& _time) const { return date_ <  _time.date_; }
    bool operator==(const Time& _time) const { return date_ == _time.date_; }

    static Time now()
    {
        std::time_t _now = std::time(nullptr);
        std::tm _local{};
        localtime_r(&_now, &_local);

        return Time
        (
            _local.tm_year + 1900,
            _local.tm_mon  + 1,
            _local.tm_mday,
            _local.tm_hour,
            _local.tm_min
        );
    }
};