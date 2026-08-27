#include "Input.hpp"
#include <termios.h>
#include <unistd.h>

namespace input
{
    bool singleInput = false;

    KEYBIND read()
    {
        if (!singleInput)
        {
            termios _terminal;
            tcgetattr(STDIN_FILENO, &_terminal);

            _terminal.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &_terminal);
            singleInput = true;
        }

        char _char;
        ::read(STDIN_FILENO, &_char, 1);

        if (_char == '\x1b')
        {
            char _sequence[2];
            ::read(STDIN_FILENO, _sequence, 2);

            if (_sequence[0] == '[') return static_cast<KEYBIND>(_sequence[1] - 'A');
        }

        else if (_char == '\n') return KEYBIND::CONFIRM;
        else if (_char == 0x7f || _char == 0x08) return KEYBIND::DELETE;

        return static_cast<KEYBIND>(_char);
    }
};