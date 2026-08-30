// #include "Data.hpp"
// #include "Menu.hpp"
#include "Input.hpp"
#include "Terminal.hpp"
#include <string>

int main()
{
    terminal::cursor(false);
    terminal::clear();

    // Task _tasks;
    // data::read ("routine.tasks", _tasks);

    // menu::display_task(_tasks);

    // data::write("routine.tasks", _tasks);

    std::string _gap = std::string(terminal::gapWidth, ' ');
    terminal::print(0, 0, 20, 1, "┌");
    terminal::print(6, 3, terminal::terminalWidth, terminal::terminalHeight, "┌");
    terminal::print(5, 7, 20, 1, "┌");
    // terminal::print
    // (
    //     0                      , 0                       ,
    //     terminal::terminalWidth, terminal::terminalHeight,
    //     "%W┌{:<{}}%W┐{}┌{:<{}}%W┐{}┌{:<{}}%W┐",
    //     " %wCurrent Task", terminal::currentWidth  - 3, _gap,
    //     " %wSubtasks"    , terminal::subtasksWidth - 3, _gap,
    //     " %w"            , terminal::menuWidth     - 3
    // );
    // terminal::print
    // (
    //     0                      , 0,
    //     terminal::terminalWidth, 1,
    //     "%W┌{}┐{}┌{}┐{}┌{}┐",
    //     std::string(terminal::currentWidth  - 2, ' '), _gap,
    //     std::string(terminal::subtasksWidth - 2, ' '), _gap,
    //     std::string(terminal::menuWidth     - 2, ' ')
    // );
    // terminal::print
    // (
    //     0                      , terminal::terminalHeight - 1,
    //     terminal::terminalWidth, 1,
    //     "%k└{}┘{}└{}┘{}└{}┘",
    //     std::string(terminal::previewWidth  - 2, ' '), _gap,
    //     std::string(terminal::subtasksWidth - 2, ' '), _gap,
    //     std::string(terminal::menuWidth     - 5, ' ')
    // );
    terminal::write();
    input::read();

    terminal::cursor(true); // Also set upon ctrl+c exit
}