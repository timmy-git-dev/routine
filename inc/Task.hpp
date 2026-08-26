#pragma once
#include <string>
#include <vector>
#include "Time.hpp"

struct Task
{
    std::string       name;
    std::string       description;
    int               status;
    Time              startTime;
    Time              endTime;
    std::vector<Task> subtasks;
};