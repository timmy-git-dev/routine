#include "Display.hpp"
#include "Loader.hpp"
#include "Input.hpp"
#include <iostream>
#include <vector>

void print_task(Task _task, std::string _indent)
{
    std::cout << _indent << _task.name << ": " << _task.description << " | " << _task.startTime.hour << ":" << _task.startTime.minute << " - " << _task.endTime.hour << ":" << _task.endTime.minute << "\n";

    for (Task _subtask : _task.subtasks)
    {
        print_task(_subtask, _indent + "  ");
    }
}

int main()
{
    input::set_single_input();
    std::vector<Task> _tasks = loader::load_tasks("testing.txt"        );

    for (Task _task : _tasks)
    {
        print_task(_task, "");
    }

    display::begin(_tasks);
    loader::save_tasks("testing.txt", _tasks);
}