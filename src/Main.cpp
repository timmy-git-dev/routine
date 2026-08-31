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
    terminal::print(0, 0, 20, 1, "┐");
    terminal::print(5, 1, 20, 1, "test");
    std::cout.flush();
    input::read();

    terminal::cursor(true); // Also set upon ctrl+c exit
}