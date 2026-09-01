#include "Data.hpp"
#include "Menu.hpp"
#include "Terminal.hpp"

int main()
{
    terminal::cursor(false);
    terminal::write();

    Task _tasks;
    data::read ("./routine/routine.tasks", _tasks);
    menu::display_task(_tasks);
    data::write("./routine/routine.tasks", _tasks);

    terminal::cursor(true);
}