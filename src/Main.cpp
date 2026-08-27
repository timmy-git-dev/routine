#include "Display.hpp"
#include "Loader.hpp"
#include "Input.hpp"
#include <vector>

int main()
{
    input::set_single_input();
    std::vector<Task> _tasks = loader::load_tasks("testing.txt"        );

    display::begin(_tasks);
    loader::save_tasks("testing.txt", _tasks);
}