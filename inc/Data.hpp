#pragma once
#include "Task.hpp"

namespace data
{
    void read (std::string _input , Task &_tasks);
    void write(std::string _output, Task &_tasks);
    void csv  (std::string _output, Task &_tasks);
};