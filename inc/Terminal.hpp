#pragma once
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

    inline int gapWidth  , gapHeight;
    inline int panelWidth, panelHeight;
    inline int splitPanelHeight;

    inline std::string output;

    template<typename... Args>
    void print(size_t _x, size_t _y, std::string _text, Args&&... _args)
    {
        ++_x;
        ++_y;
        _text = std::vformat(_text, std::make_format_args(_args...));
        output += std::format("\033[{};{}H", _y++, _x);

        size_t _index = 0;
        size_t _next;
        while ((_next = _text.find_first_of('%', _index)) != std::string::npos)
        {
            if (_next - _index > 0)
            {
                output += std::string_view(_text.data() + _index, _next - _index);
            }

            _index = _next + 1;

            switch (static_cast<CODE_CHARS>(_text[_index++]))
            {
                #define SET_BIT(CODE) gridProperties[(_x + _offsetX) + (terminalWidth) * (_y + _offsetY)].bits.CODE = 1; continue
                case CODE_CHARS::RESET       : output += "\033[0m";                break;
                case CODE_CHARS::BOLD        : output += "\033[1m";                break;
                case CODE_CHARS::ITALIC      : output += "\033[3m";                break;
                case CODE_CHARS::UNDERLINE   : output += "\033[4m";                break;
                case CODE_CHARS::BLINK       : output += "\033[5m";                break;
                case CODE_CHARS::STRIKE      : output += "\033[9m";                break;

                case CODE_CHARS::WHT_DRK     : output += "\033[38;2;176;179;199m"; break;
                case CODE_CHARS::WHT_LIT     : output += "\033[38;2;210;214;224m"; break;
                case CODE_CHARS::BLK_DRK     : output += "\033[38;2;0;0;0m";       break;
                case CODE_CHARS::BLK_LIT     : output += "\033[38;2;112;117;141m"; break;
                case CODE_CHARS::RED_DRK     : output += "\033[38;2;241;44;65m";   break;
                case CODE_CHARS::RED_LIT     : output += "\033[38;2;251;120;94m";  break;
                case CODE_CHARS::YLW_DRK     : output += "\033[38;2;255;197;55m";  break;
                case CODE_CHARS::YLW_LIT     : output += "\033[38;2;255;233;19m";  break;
                case CODE_CHARS::GRN_DRK     : output += "\033[38;2;19;237;71m";   break;
                case CODE_CHARS::GRN_LIT     : output += "\033[38;2;86;250;49m";   break;
                case CODE_CHARS::BLU_DRK     : output += "\033[38;2;96;82;247m";   break;
                case CODE_CHARS::BLU_LIT     : output += "\033[38;2;141;131;252m"; break;
                case CODE_CHARS::PNK_DRK     : output += "\033[38;2;252;89;252m";  break;
                case CODE_CHARS::PNK_LIT     : output += "\033[38;2;252;142;252m"; break;
                default                      :                                      break;
            }
        }

        if (_text.size() - _index > 0)
        {
            output += std::string_view(_text.data() + _index, _text.size() - _index);
        }
    }
    template<typename... Args>
    void print(size_t _x, size_t _y, size_t _width, size_t _height, const std::string &_input, Args&&... _args)
    {
        ++_x;
        ++_y;

        std::string _text = std::vformat(_input, std::make_format_args(_args...));

        output += std::format("\033[{};{}H", _y, _x);

        size_t _index = 0;
        size_t _col = 0;
        size_t _row = 0;

        auto newline = [&]
        {
            ++_row;
            _col = 0;

            if (_row >= _height)
                return false;

            output += std::format("\033[{};{}H", _y + _row, _x);
            return true;
        };

        while (_index < _text.size() && _row < _height)
        {
            if (_text[_index] == '\\')
            {
                if (++_index >= _text.size())
                    break;

                output += _text[_index++];
                continue;
            }
            else if (_text[_index] == '%')
            {
                if (_index + 1 >= _text.size())
                    break;

                switch (static_cast<CODE_CHARS>(_text[_index + 1]))
                {
                    case CODE_CHARS::RESET:     output += "\033[0m"; break;
                    case CODE_CHARS::BOLD:      output += "\033[1m"; break;
                    case CODE_CHARS::ITALIC:    output += "\033[3m"; break;
                    case CODE_CHARS::UNDERLINE: output += "\033[4m"; break;
                    case CODE_CHARS::BLINK:     output += "\033[5m"; break;
                    case CODE_CHARS::STRIKE:    output += "\033[9m"; break;

                    case CODE_CHARS::WHT_DRK:   output += "\033[38;2;176;179;199m"; break;
                    case CODE_CHARS::WHT_LIT:   output += "\033[38;2;210;214;224m"; break;
                    case CODE_CHARS::BLK_DRK:   output += "\033[38;2;0;0;0m";       break;
                    case CODE_CHARS::BLK_LIT:   output += "\033[38;2;112;117;141m"; break;
                    case CODE_CHARS::RED_DRK:   output += "\033[38;2;241;44;65m";   break;
                    case CODE_CHARS::RED_LIT:   output += "\033[38;2;251;120;94m";  break;
                    case CODE_CHARS::YLW_DRK:   output += "\033[38;2;255;197;55m";  break;
                    case CODE_CHARS::YLW_LIT:   output += "\033[38;2;255;233;19m";  break;
                    case CODE_CHARS::GRN_DRK:   output += "\033[38;2;19;237;71m";   break;
                    case CODE_CHARS::GRN_LIT:   output += "\033[38;2;86;250;49m";   break;
                    case CODE_CHARS::BLU_DRK:   output += "\033[38;2;96;82;247m";   break;
                    case CODE_CHARS::BLU_LIT:   output += "\033[38;2;141;131;252m"; break;
                    case CODE_CHARS::PNK_DRK:   output += "\033[38;2;252;89;252m";  break;
                    case CODE_CHARS::PNK_LIT:   output += "\033[38;2;252;142;252m"; break;
                    default: break;
                }

                _index += 2;
                continue;
            }

            if (_text[_index] == '\n')
            {
                ++_index;
                if (!newline())
                    break;
                continue;
            }

            size_t _charSize = 1;

            unsigned char c = _text[_index];

            if ((c & 0x80) == 0)
                _charSize = 1;
            else if ((c & 0xE0) == 0xC0)
                _charSize = 2;
            else if ((c & 0xF0) == 0xE0)
                _charSize = 3;
            else if ((c & 0xF8) == 0xF0)
                _charSize = 4;

            if (_col >= _width)
            {
                if (!newline())
                {
                    break;
                }
            }

            output.append(_text, _index, _charSize);

            _index += _charSize;
            ++_col;
        }
    }

    inline void cursor(bool _visible) {std::cout << (_visible ? "\033[?25h" : "\033[?25l");}
    inline void update()
    {
        struct winsize _windowSize{};
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &_windowSize);

        if (terminalWidth == _windowSize.ws_col && terminalHeight == _windowSize.ws_row)
        {
            return;
        }

        terminalWidth    = _windowSize.ws_col;
        terminalHeight   = _windowSize.ws_row;

        gapWidth         = 3 - (terminalWidth  % 3);
        gapHeight        = 2 - (terminalHeight % 2);

        panelWidth       = (terminalWidth  - gapWidth * 2) / 3;
        panelHeight      = (terminalHeight               );

        splitPanelHeight = (terminalHeight - gapHeight   ) / 2;
    }
    inline void write()
    {
        std::cout << output;
        std::cout.flush();
        output = "\033[2J\033[3J\033[H";
        output.reserve(terminalWidth * terminalHeight);
        update();
    }
}