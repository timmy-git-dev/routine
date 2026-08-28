#pragma once
#include "Task.hpp"

namespace menu
{
    void display_task(Task &_task);
    void new_task    (Task &_task);
    void remove_task (Task &_task);
    void edit_task   (Task &_task);
};