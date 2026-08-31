#pragma once
#include <algorithm>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/ioctl.h>
#include <unistd.h>

namespace terminal
{
    enum class CODE_CHARS : char
    {
        RESET     = '^',
        BOLD      = '*',
        ITALIC    = '/',
        UNDERLINE = '_',
        BLINK     = '|',
        STRIKE    = '-',

        WHT_DRK = 'w', WHT_LIT = 'W',
        BLK_DRK = 'k', BLK_LIT = 'K',
        RED_DRK = 'r', RED_LIT = 'R',
        YLW_DRK = 'y', YLW_LIT = 'Y',
        GRN_DRK = 'g', GRN_LIT = 'G',
        BLU_DRK = 'b', BLU_LIT = 'B',
        PNK_DRK = 'p', PNK_LIT = 'P',
    };

    inline int terminalWidth, terminalHeight;
    inline int gapWidth     , gapHeight;

    inline int currentWidth , currentHeight;
    inline int previewWidth , previewHeight;
    inline int subtasksWidth, subtasksHeight;
    inline int menuWidth    , menuHeight;


    template<typename... Args>
    void print(size_t _x, size_t _y, size_t _width, size_t _height, std::string _text, Args&&... _args)
    {
        ++_x;
        ++_y;
        _text = std::vformat(_text, std::make_format_args(_args...));

        size_t _remaining = _width;

        std::string _output = std::format("\033[{};{}H", _y++, _x);
        _output.reserve(_text.size() * 2);

        size_t _index = 0;
        while (true)
        {
            size_t _next  = _text.find_first_of('%', _index);
            size_t _count = std::min(_text.size() - _index, std::min(_remaining, _next - _index));

            while (_count > 0)
            {
                size_t _subCount = std::min(_remaining, _count);
                _output    += std::string_view(_text.data() + _index, _subCount);
                _index     += _subCount;
                _count     -= _subCount;
                _remaining -= _subCount;

                if (_remaining == 0)
                {
                    if (_height-- == 0)
                    {
                        std::cout << _output;
                        return;
                    }

                    _remaining = _width;
                    _output   += std::format("\033[{};{}H", _y++, _x);
                }
            }

            if (_next != std::string::npos)
            {
                switch (static_cast<CODE_CHARS>(_text[++_index]))
                {
                    #define SET_BIT(CODE) gridProperties[(_x + _offsetX) + (terminalWidth) * (_y + _offsetY)].bits.CODE = 1; continue
                    case CODE_CHARS::RESET       : _output += "\033[0m";                break;
                    case CODE_CHARS::BOLD        : _output += "\033[1m";                break;
                    case CODE_CHARS::ITALIC      : _output += "\033[3m";                break;
                    case CODE_CHARS::UNDERLINE   : _output += "\033[4m";                break;
                    case CODE_CHARS::BLINK       : _output += "\033[5m";                break;
                    case CODE_CHARS::STRIKE      : _output += "\033[9m";                break;

                    case CODE_CHARS::WHT_DRK     : _output += "\033[38;2;176;179;199m"; break;
                    case CODE_CHARS::WHT_LIT     : _output += "\033[38;2;210;214;224m"; break;
                    case CODE_CHARS::BLK_DRK     : _output += "\033[38;2;0;0;0m";       break;
                    case CODE_CHARS::BLK_LIT     : _output += "\033[38;2;112;117;141m"; break;
                    case CODE_CHARS::RED_DRK     : _output += "\033[38;2;241;44;65m";   break;
                    case CODE_CHARS::RED_LIT     : _output += "\033[38;2;251;120;94m";  break;
                    case CODE_CHARS::YLW_DRK     : _output += "\033[38;2;255;197;55m";  break;
                    case CODE_CHARS::YLW_LIT     : _output += "\033[38;2;255;233;19m";  break;
                    case CODE_CHARS::GRN_DRK     : _output += "\033[38;2;19;237;71m";   break;
                    case CODE_CHARS::GRN_LIT     : _output += "\033[38;2;86;250;49m";   break;
                    case CODE_CHARS::BLU_DRK     : _output += "\033[38;2;96;82;247m";   break;
                    case CODE_CHARS::BLU_LIT     : _output += "\033[38;2;141;131;252m"; break;
                    case CODE_CHARS::PNK_DRK     : _output += "\033[38;2;252;89;252m";  break;
                    case CODE_CHARS::PNK_LIT     : _output += "\033[38;2;252;142;252m"; break;
                    default                      :                                      break;
                }

                ++_index;
            }
            else
            {
                std::cout << _output;
                return;
            }
        }
    }

    inline void cursor(bool _visible) {std::cout << (_visible ? "\033[?25h" : "\033[?25l");}
    inline void clear()
    {
        struct winsize _windowSize{};
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &_windowSize);

        terminalWidth  = _windowSize.ws_col;
        terminalHeight = _windowSize.ws_row;
        gapWidth       = 3 - (terminalWidth  % 3);
        gapHeight      = 2 - (terminalHeight % 2);

        currentWidth   = (terminalWidth - gapWidth * 2) / 3;
        previewWidth   = currentWidth;
        subtasksWidth  = currentWidth;
        menuWidth      = currentWidth;

        currentHeight  = (terminalHeight - gapHeight) / 2;
        previewHeight  = currentHeight;
        subtasksHeight = terminalHeight;
        menuHeight     = terminalHeight;

        std::cout << "\033[2J\033[3J\033[H";
    }
}