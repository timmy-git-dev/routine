#pragma once
#include "Task.hpp"

namespace data
{
    void read (std::string _file, Task &_tasks);
    void write(std::string _file, Task &_tasks);
};