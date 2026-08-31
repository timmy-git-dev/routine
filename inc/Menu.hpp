#pragma once
#include "Task.hpp"

namespace menu
{
    void display_task(Task &_task);
    void new_task    (Task &_parentTask, const size_t _index, size_t &_offset);
    void remove_task (Task &_parentTask, const size_t _index);
    void edit_task   (Task &_parentTask, const size_t _index, size_t &_offset);
};