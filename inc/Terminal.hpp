#pragma once
#include <format>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

namespace terminal
{
    union CodeBits
    {
        unsigned int value;

        struct
        {
            unsigned int reset     : 1;
            unsigned int bold      : 1;
            unsigned int italic    : 1;
            unsigned int underline : 1;
            unsigned int blink     : 1;
            unsigned int strike    : 1;

            unsigned int wht_drk   : 1;
            unsigned int wht_lit   : 1;
            unsigned int blk_drk   : 1;
            unsigned int blk_lit   : 1;
            unsigned int red_drk   : 1;
            unsigned int red_lit   : 1;
            unsigned int ylw_drk   : 1;
            unsigned int ylw_lit   : 1;
            unsigned int grn_drk   : 1;
            unsigned int grn_lit   : 1;
            unsigned int blu_drk   : 1;
            unsigned int blu_lit   : 1;
            unsigned int pnk_drk   : 1;
            unsigned int pnk_lit   : 1;
        } bits;
    };
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

    inline std::string           gridChars;
    inline std::vector<CodeBits> gridProperties;


    template<typename... Args>
    size_t print(size_t _x, size_t _y, size_t _width, size_t _height, std::string _text, Args&&... _args)
    {
        _text = std::vformat(_text, std::make_format_args(_args...));

        size_t _offsetX = 0;
        size_t _offsetY = 0;
        for (size_t _i = 0; _i < _text.size(); ++_i)
        {
            if (_offsetX >= _width)
            {
                _offsetY += 1;
                _offsetX = 0;
            }
            if (_offsetY >= _height)
            {
                return _height;
            }

            char _char = _text[_i];
            if (_char == '%')
            {
                switch (static_cast<CODE_CHARS>(_text[++_i]))
                {
                    #define SET_BIT(CODE) gridProperties[(_x + _offsetX) + (terminalWidth) * (_y + _offsetY)].bits.CODE = 1; continue
                    case CODE_CHARS::RESET       : SET_BIT(reset    );
                    case CODE_CHARS::BOLD        : SET_BIT(bold     );
                    case CODE_CHARS::ITALIC      : SET_BIT(italic   );
                    case CODE_CHARS::UNDERLINE   : SET_BIT(underline);
                    case CODE_CHARS::BLINK       : SET_BIT(blink    );
                    case CODE_CHARS::STRIKE      : SET_BIT(strike   );
                    case CODE_CHARS::WHT_DRK     : SET_BIT(wht_drk  );
                    case CODE_CHARS::WHT_LIT     : SET_BIT(wht_lit  );
                    case CODE_CHARS::BLK_DRK     : SET_BIT(blk_drk  );
                    case CODE_CHARS::BLK_LIT     : SET_BIT(blk_lit  );
                    case CODE_CHARS::RED_DRK     : SET_BIT(red_drk  );
                    case CODE_CHARS::RED_LIT     : SET_BIT(red_lit  );
                    case CODE_CHARS::YLW_DRK     : SET_BIT(ylw_drk  );
                    case CODE_CHARS::YLW_LIT     : SET_BIT(ylw_lit  );
                    case CODE_CHARS::GRN_DRK     : SET_BIT(grn_drk  );
                    case CODE_CHARS::GRN_LIT     : SET_BIT(grn_lit  );
                    case CODE_CHARS::BLU_DRK     : SET_BIT(blu_drk  );
                    case CODE_CHARS::BLU_LIT     : SET_BIT(blu_lit  );
                    case CODE_CHARS::PNK_DRK     : SET_BIT(pnk_drk  );
                    case CODE_CHARS::PNK_LIT     : SET_BIT(pnk_lit  );
                    default                      : continue;
                }
            }

            gridChars[(_x + _offsetX) + (terminalWidth) * (_y + _offsetY)] = _char;
            ++_offsetX;
        }

        return _offsetY;
    }

    inline void cursor(bool _visible) {std::cout << (_visible ? "\033[?25h" : "\033[?25l");}
    inline void write()
    {
        constexpr const char* CODE_ESCAPES[22] =
        {
            "\033[0m"               , // RESET
            "\033[1m"               , // BOLD
            "\033[3m"               , // ITALIC
            "\033[4m"               , // UNDERLINE
            "\033[5m"               , // BLINK
            "\033[9m"               , // STRIKE

            "\033[38;2;176;179;199m", // WHT_DRK
            "\033[38;2;210;214;224m", // WHT_LIT
            "\033[38;2;0;0;0m"      , // BLK_DRK
            "\033[38;2;112;117;141m", // BLK_LIT
            "\033[38;2;241;44;65m"  , // RED_DRK
            "\033[38;2;251;120;94m" , // RED_LIT
            "\033[38;2;255;197;55m" , // YLW_DRK
            "\033[38;2;255;233;19m" , // YLW_LIT
            "\033[38;2;19;237;71m"  , // GRN_DRK
            "\033[38;2;86;250;49m"  , // GRN_LIT
            "\033[38;2;96;82;247m"  , // BLU_DRK
            "\033[38;2;141;131;252m", // BLU_LIT
            "\033[38;2;252;89;252m" , // PNK_DRK
            "\033[38;2;252;142;252m", // PNK_LIT
        };

        std::string _text;
        _text.reserve(gridChars.size() * 2);
        for (size_t _i = 0; _i < gridChars.size(); ++_i)
        {
            CodeBits _properties = gridProperties[_i];
            if (_properties.value != 0)
            {
                for (size_t _code = 0; _code < 22; ++_code)
                {
                    if (_properties.value & (1 << _code))
                    {
                        _text += CODE_ESCAPES[_code];
                    }
                }
            }

            _text += gridChars[_i];
        }

        std::cout << _text;
        std::cout.flush();
    }
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

        gridChars     .clear();
        gridProperties.clear();
        gridChars     = std::string(terminalWidth * terminalHeight, '.');
        gridProperties.resize(terminalWidth * terminalHeight);

        std::cout << "\033[2J\033[3J\033[H";
    }
}