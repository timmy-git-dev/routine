#pragma once
#include <string>

namespace terminal
{
    enum class CODE : char
    {
        RESET     = '^',

        BOLD      = '*',
        ITALIC    = '/',
        UNDERLINE = '_',
        BLINK     = '|',
        STRIKE    = '-',

        DARK_BLACK   = 'K', LIGHT_BLACK   = 'k',
        DARK_RED     = 'R', LIGHT_RED     = 'r',
        DARK_GREEN   = 'G', LIGHT_GREEN   = 'g',
        DARK_YELLOW  = 'Y', LIGHT_YELLOW  = 'y',
        DARK_BLUE    = 'B', LIGHT_BLUE    = 'b',
        DARK_MAGENTA = 'M', LIGHT_MAGENTA = 'm',
        DARK_CYAN    = 'C', LIGHT_CYAN    = 'c',
        DARK_WHITE   = 'W', LIGHT_WHITE   = 'w',
    };

    template<typename... Args>
    void print(std::string _text, Args&&... _args);

    void cursor(bool _visible);
    void clear (             );
}