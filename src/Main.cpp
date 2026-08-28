#include "Data.hpp"
#include "Menu.hpp"

int main()
{
    Task _tasks;
    data::read ("routine.tasks", _tasks);

    menu::display_task(_tasks);

    data::write("routine.tasks", _tasks);
}