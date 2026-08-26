#pragma once
#include "Task.hpp"
#include <vector>

namespace loader
{
    void              save_tasks(std::string _saveFile, std::vector<Task> _tasks);
    std::vector<Task> load_tasks(std::string _saveFile                          );
};