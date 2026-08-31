// #include "Data.hpp"
// #include "Menu.hpp"
#include "Input.hpp"
#include "Terminal.hpp"

std::string _border = "││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││";

void print_border(size_t _x0, size_t _y0, size_t _width, size_t _height)
{
    size_t _x1 = _x0 + _width - 1;
    size_t _y1 = _y0 + 1;
    size_t _y2 = _y0 - 1 + _height;

    terminal::print(_x0, _y0,                   "%W┌"); terminal::print(_x1, _y0,                     "┐");
    terminal::print(_x0, _y1, 1, _height - 2, _border); terminal::print(_x1, _y1, 1, _height - 2, _border);
    terminal::print(_x0, _y2,                     "└"); terminal::print(_x1, _y2,                     "┘");
}

int main()
{
    terminal::cursor(false);
    terminal::clear();

    terminal::clear();
    print_border(0                                              , 0                                               , terminal::panelWidth, terminal::splitPanelHeight); // Current.
    print_border(0                                              , terminal::splitPanelHeight + terminal::gapHeight, terminal::panelWidth, terminal::splitPanelHeight); // Preview.
    print_border( terminal::panelWidth + terminal::gapWidth     , 0                                               , terminal::panelWidth, terminal::panelHeight     ); // Subtasks.
    print_border((terminal::panelWidth + terminal::gapWidth) * 2, 0                                               , terminal::panelWidth, terminal::panelHeight     ); // Command.

    std::cout.flush();
    input::read();

    terminal::cursor(true); // Also set upon ctrl+c exit
}