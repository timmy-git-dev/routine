#include "Data.hpp"
#include "Menu.hpp"
#include "Terminal.hpp"

// const std::string _horizontal = "────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────";
// const std::string _vertical   = "││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││";

// void print_border_bg(size_t _x0, size_t _y0, size_t _width, size_t _height)
// {
//     size_t _x1 = _x0 + _width - 1;
//     size_t _y1 = _y0 + 1;
//     size_t _y2 = _y0 - 1 + _height;

//     terminal::print(_x0, _y0,                     "%W┌"); terminal::print(_x1, _y0,                       "┐");
//     terminal::print(_x0, _y1, 1, _height - 2, _vertical); terminal::print(_x1, _y1, 1, _height - 2, _vertical);
//     terminal::print(_x0, _y2,                       "└"); terminal::print(_x1, _y2,                       "┘");
// }
// void print_border_fg(size_t _x0, size_t _y0, size_t _width, size_t _height)
// {
//     size_t _x1 = _x0 + _width - 1;
//     size_t _y1 = _y0 + 1;
//     size_t _y2 = _y0 - 1 + _height;

//     terminal::print(_x0, _y0,                     "%W╭"); terminal::print(_x0 + 1, _y0, _width - 2, 1, _horizontal); terminal::print(_x1, _y0,                       "╮");
//     terminal::print(_x0, _y1, 1, _height - 2, _vertical);                                                            terminal::print(_x1, _y1, 1, _height - 2, _vertical);
//     terminal::print(_x0, _y2,                       "╰"); terminal::print(_x1 + 1, _y2, _width - 2, 1, _horizontal); terminal::print(_x1, _y2,                       "╯");
// }

int main()
{
    terminal::cursor(false);
    terminal::write();

    Task _tasks;
    data::read("routine.tasks", _tasks);

    menu::display_task(_tasks);

    // TODO: Put into menu system, not just in main.cpp.
    // terminal::clear();
    // print_border(0                                              , 0                                               , terminal::panelWidth, terminal::splitPanelHeight); // Current.
    // print_border(0                                              , terminal::splitPanelHeight + terminal::gapHeight, terminal::panelWidth, terminal::splitPanelHeight); // Preview.
    // print_border( terminal::panelWidth + terminal::gapWidth     , 0                                               , terminal::panelWidth, terminal::panelHeight     ); // Subtasks.
    // print_border((terminal::panelWidth + terminal::gapWidth) * 2, 0                                               , terminal::panelWidth, terminal::panelHeight     ); // Command.

    // std::cout.flush();
    // input::read();

    data::write("routine.tasks", _tasks);

    terminal::cursor(true); // Also set upon ctrl+c exit
}