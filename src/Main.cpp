#include "Data.hpp"

int main()
{
    Task _tasks;
    data::read ("tasks", _tasks);



    data::write("tasks", _tasks);
}