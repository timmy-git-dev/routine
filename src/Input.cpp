#include "Input.hpp"
#include <termios.h>
#include <unistd.h>

namespace input
{
    void set_single_input()
    {
        termios _terminal;
        tcgetattr(STDIN_FILENO, &_terminal);

        _terminal.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &_terminal);
    }

    char wait_for_input()
    {
        char _char;
        read(STDIN_FILENO, &_char, 1);
        if (_char == '\x1b')
        {
            char _sequence[2];
            read(STDIN_FILENO, _sequence, 2);
            if (_sequence[0] == '[')
            {
                return _sequence[1] - 'A';
            }
        }
        else if (_char == 0x7f || _char == 0x08)
            return 4;


        return _char;
    }
};