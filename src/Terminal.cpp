#include "Terminal.hpp"
#include <format>
#include <iostream>

namespace terminal
{
    #define REPLACE(CODE) _text.replace(_index, 2, "\033[" #CODE "m")

    template<typename... Args>
    void print(std::string _text, Args&&... _args)
    {
        size_t _index = -1;
        while ((_index = _text.find_first_of('%', _index + 1)) != std::string::npos)
        {
            switch (static_cast<CODE>(_text[_index + 1]))
            {
                case CODE::RESET         : REPLACE( 0); break;
                case CODE::BOLD          : REPLACE( 1); break;
                case CODE::ITALIC        : REPLACE( 3); break;
                case CODE::UNDERLINE     : REPLACE( 4); break;
                case CODE::BLINK         : REPLACE( 5); break;
                case CODE::STRIKE        : REPLACE( 9); break;
                case CODE::DARK_BLACK    : REPLACE(30); break;
                case CODE::DARK_RED      : REPLACE(31); break;
                case CODE::DARK_GREEN    : REPLACE(32); break;
                case CODE::DARK_YELLOW   : REPLACE(33); break;
                case CODE::DARK_BLUE     : REPLACE(34); break;
                case CODE::DARK_MAGENTA  : REPLACE(35); break;
                case CODE::DARK_CYAN     : REPLACE(36); break;
                case CODE::DARK_WHITE    : REPLACE(37); break;
                case CODE::LIGHT_BLACK   : REPLACE(90); break;
                case CODE::LIGHT_RED     : REPLACE(91); break;
                case CODE::LIGHT_GREEN   : REPLACE(92); break;
                case CODE::LIGHT_YELLOW  : REPLACE(93); break;
                case CODE::LIGHT_BLUE    : REPLACE(94); break;
                case CODE::LIGHT_MAGENTA : REPLACE(95); break;
                case CODE::LIGHT_CYAN    : REPLACE(96); break;
                case CODE::LIGHT_WHITE   : REPLACE(97); break;
                default                  : return;
            }
        }

        std::cout << std::format(_text, _args...);
    }

    void cursor(bool _visible) {std::cout << (_visible ? "\033[?25h" : "\033[?25l");}
    void clear (             ) {std::cout <<             "\033[2J\033[3J\033[H";    }
}