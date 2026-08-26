#include "Loader.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace loader
{
    int valid_subtask_count(const Task &_task)
    {
        int _count = 0;
        for (const Task& _subtask : _task.subtasks)
        {
            _count += !_subtask.name.empty();
        }

        return _count;
    }

    void print_task(std::ofstream &_file, const Task &_task)
    {
        if (_task.name.empty())
        {
            return;
        }

        int _taskCount = valid_subtask_count(_task);

        if (_task.startTime.hour   < 10) {_file << '0';} _file << _task.startTime.hour;
        if (_task.startTime.minute < 10) {_file << '0';} _file << _task.startTime.minute;
        _file << '-';
        if (_task.endTime  .hour   < 10) {_file << '0';} _file << _task.endTime  .hour;
        if (_task.endTime  .minute < 10) {_file << '0';} _file << _task.endTime  .minute;
        _file << ' ';
        _file << _task.status;
        _file << ' ';
        if (_taskCount < 10) {_file << '0';} _file << _taskCount;
        _file << ' ';
        _file << '"' << _task.name        << '"';
        _file << ' ';
        _file << '"' << _task.description << '"';
        _file << '\n';
    }
    void write_task(std::ofstream &_file, const Task &_task)
    {
        print_task(_file, _task);

        for (const Task &_subtask : _task.subtasks)
        {
            write_task(_file, _subtask);
        }
    }

    void save_tasks(std::string _saveFile, std::vector<Task> _tasks)
    {
        std::ofstream _file = std::ofstream(_saveFile, std::ios::trunc);

        for (Task _task : _tasks)
        {
            write_task(_file, _task);
        }

        _file.close();
    }

    Task empty_task(Time _start, Time _end)
    {
        return Task
        {
            .name        = "",
            .description = "",
            .status      = 0,
            .startTime   = _start,
            .endTime     = _end,
            .subtasks    = std::vector<Task>(),
        };
    }

    Task get_task(std::ifstream& _file)
    {
        std::string _line;
        std::getline(_file, _line);

        size_t _startName = _line.find_first_of('"') + 1;
        size_t _endName   = _line.find_first_of('"', _startName);
        size_t _startDesc = _endName + 3;
        size_t _endDesc   = _line.find_first_of('"', _startDesc);

        std::string _name = _line.substr(_startName, _endName - _startName);
        std::string _desc = _line.substr(_startDesc, _endDesc - _startDesc);

        Time _start =
        {
            .hour   = 10 * (_line[0] - '0') + (_line[1] - '0'),
            .minute = 10 * (_line[2] - '0') + (_line[3] - '0'),
        };
        Time _end   =
        {
            .hour   = 10 * (_line[5] - '0') + (_line[6] - '0'),
            .minute = 10 * (_line[7] - '0') + (_line[8] - '0'),
        };

        int _status = _line[10] - '0';

        int _subtaskCount = 10 * (_line[ 12] - '0') + (_line[13] - '0');
        std::vector<Task> _subtasks;

        Time _prevEnd = _start;
        for (int _i = 0; _i < _subtaskCount; ++_i)
        {
            Task _subtask = get_task(_file);

            if (_prevEnd.hour != _subtask.startTime.hour || _prevEnd.minute != _subtask.startTime.minute)
            {
                _subtasks.push_back(empty_task(_prevEnd, _subtask.startTime));
            }

            _subtasks.push_back(_subtask);
            _prevEnd = _subtask.endTime;
        }

        if (_prevEnd.hour != _end.hour || _prevEnd.minute != _end.minute)
        {
            _subtasks.push_back(empty_task(_prevEnd, _end));
        }

        return Task
        {
            .name        = _name    ,
            .description = _desc    ,
            .status      = _status  ,
            .startTime   = _start   ,
            .endTime     = _end     ,
            .subtasks    = _subtasks,
        };
    }

    std::vector<Task> load_tasks(std::string _saveFile)
    {
        std::ifstream _file = std::ifstream(_saveFile);

        std::vector<Task> _tasks;
        Time _prevEnd { };
        while (_file.peek() != EOF)
        {
            Task _task = get_task(_file);

            if (_prevEnd.hour != _task.startTime.hour || _prevEnd.minute != _task.startTime.minute)
            {
                _tasks.push_back(empty_task(_prevEnd, _task.startTime));
            }

            _tasks.push_back(_task);
            _prevEnd = _task.endTime;
        }

        _file.close();

        return _tasks;
    }
};



// 0000-0400 1 00 "sleep"       "do some sleep"
// 0400-0415 0 01 "hygiene"     "do some hygiene."
// 0400-0405 0 00 "brush teeth" "do some teeth brushin"
// 0600-2400 0 00 "test"        "example"