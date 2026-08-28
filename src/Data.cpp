#include "Data.hpp"
#include <fstream>
#include <iostream>
#include "Task.hpp"

namespace data
{
    std::vector<std::string_view> read_line(const std::string &_line)
    {
        size_t _curr = 0;
        std::vector<std::string_view> _args;

        do
        {
            size_t _next = _line.find_first_of('|', _curr);
            _args.push_back(std::string_view(_line.data() + _curr, _next - _curr));
            _curr = _next;
        }
        while (_curr++ != std::string::npos);

        return _args;
    }
    constexpr int read_digits(const std::string_view &_text)
    {
        int _mult = 1;
        int _digits = 0;
        for (size_t _i = 0; _i < _text.size(); ++_i)
        {
            _digits += (_text[_text.size() - _i - 1] - '0') * _mult;
            _mult *= 10;
        }

        return _digits;
    }
    Time read_time
    (
        const std::string_view &_year,
        const std::string_view &_month,
        const std::string_view &_day,
        const std::string_view &_hour,
        const std::string_view &_minute
    )
    {
        return Time
        {
            .year   = read_digits(_year  ),
            .month  = read_digits(_month ),
            .day    = read_digits(_day   ),
            .hour   = read_digits(_hour  ),
            .minute = read_digits(_minute),
        };
    }
    Task read_task(std::ifstream& _file, Task &_task)
    {
        std::string _line;
        std::getline(_file, _line);
        std::vector<std::string_view> _args = read_line(_line);

        _task.name        = _args[0];
        _task.description = _args[1];
        _task.status      = read_digits(_args[2]);
        _task.subtasks    = std::vector<Task>(read_digits(_args[3]));
        _task.startTime   = read_time(_args[ 4], _args[ 5], _args[ 6], _args[ 7], _args[ 8]);
        _task.endTime     = read_time(_args[ 9], _args[10], _args[11], _args[12], _args[13]);

        for (Task &_subtask : _task.subtasks)
        {
            read_task(_file, _subtask);
        }

        return _task;
    }

    void print_task(std::ofstream &_file, const Task &_task)
    {
        _file << std::format
        (
            "{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|{}|\n",
            _task.name           ,
            _task.description    ,
            _task.status         ,
            _task.subtasks.size(),
            _task.startTime.year, _task.startTime.month, _task.startTime.day, _task.startTime.hour, _task.startTime.minute,
            _task.endTime  .year, _task.endTime  .month, _task.endTime  .day, _task.endTime  .hour, _task.endTime  .minute
        );

        for (const Task &_subtask : _task.subtasks)
        {
            print_task(_file, _subtask);
        }
    }

    void read (std::string _input , Task &_tasks)
    {
        std::ifstream _file = std::ifstream(_input);
        read_task(_file, _tasks);
    }
    void write(std::string _output, Task &_tasks)
    {
        std::ofstream _file = std::ofstream(_output, std::ios::trunc);
        print_task(_file, _tasks);
    }
};