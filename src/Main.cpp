// #include "Data.hpp"
// #include "Menu.hpp"
#include "Input.hpp"
#include "Terminal.hpp"

int main()
{
    terminal::cursor(false);
    terminal::clear();

    // terminal::print(size_t _x, size_t _y, size_t _width, size_t _height, std::string _text, Args &&_args...)

    terminal::clear();
    // terminal::print(0                         , 0, "%W┌");
    // terminal::print(terminal::currentWidth - 1, 0, "  ┐");

    std::string _gap = std::string(terminal::gapWidth, ' ');

    terminal::print
    (
        0                      , 0,
        "%*%W┌ %wCurrent Task{}%W┐{}┌ %wSubtasks{}%W┐{}┌{}┐",
        std::string(terminal::currentWidth  - 2 - 13, ' '), _gap,
        std::string(terminal::subtasksWidth - 2 -  9, ' '), _gap,
        std::string(terminal::menuWidth     - 2     , ' ')
    );
    terminal::print
    (
        2                      , 2,
        8                      , 5,
        "Hello, world! (:"
    );

    std::cout.flush();
    input::read();

    terminal::cursor(true); // Also set upon ctrl+c exit
}