#pragma once

struct Time
{
    using u64 = unsigned long; // TODO: add alias file.
    using u32 = unsigned long;

private:
    u64 date_;

    u32 year  () const;
    u32 month () const;
    u32 day   () const;
    u32 hour  () const;
    u32 minute() const;

    Time(int _year, int _month, int _day, int _hour, int _minute);

    bool operator> (const Time& _time) const;
    bool operator< (const Time& _time) const;
    bool operator==(const Time& _time) const;
};