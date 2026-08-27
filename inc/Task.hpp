#pragma once
#include <string>
#include <vector>
#include "Time.hpp"

struct Task
{
    std::string       name;
    std::string       description;
    int               status;
    std::vector<Task> subtasks;
    Time              startTime;
    Time              endTime;
};