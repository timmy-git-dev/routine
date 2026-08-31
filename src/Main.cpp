// #include "Data.hpp"
// #include "Menu.hpp"
#include "Input.hpp"
#include "Terminal.hpp"

int main()
{
    terminal::cursor(false);
    terminal::clear();


    terminal::clear();
// VERTICAL:
    std::string _vertical = "%W││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││";

    // Current.
    terminal::print
    (
        0                         , 1,
        1                         , terminal::currentHeight - 2,
        _vertical
    );
    terminal::print
    (
        0                         , 1 + (terminal::currentHeight - 2) + (terminal::gapHeight + 2),
        1                         , terminal::currentHeight - 2,
        _vertical
    );

    // Preview.
    terminal::print
    (
        terminal::currentWidth - 1, 1,
        1                         , terminal::previewHeight - 2,
        _vertical
    );
    terminal::print
    (
        terminal::previewWidth - 1, 1 + (terminal::currentHeight - 2) + (terminal::gapHeight + 2),
        1                         , terminal::previewHeight - 2,
        _vertical
    );

    // Subtasks.
    terminal::print
    (
        terminal::currentWidth + terminal::gapWidth                              , 1,
        1                                                                        , terminal::subtasksHeight - 2,
        _vertical
    );
    terminal::print
    (
        terminal::currentWidth + terminal::gapWidth + terminal::subtasksWidth - 1, 1,
        1                                                                           , terminal::subtasksHeight - 2,
        _vertical
    );

    // Menu.
    terminal::print
    (
        terminal::currentWidth + terminal::gapWidth + terminal::subtasksWidth - 1 + terminal::gapWidth + 1                          , 1,
        1                                                                                                                           , terminal::menuHeight - 2,
        _vertical
    );
    terminal::print
    (
        terminal::currentWidth + terminal::gapWidth + terminal::subtasksWidth - 1 + terminal::gapWidth + 1 + terminal::menuWidth - 1, 1,
        1                                                                                                                           , terminal::menuHeight - 2,
        _vertical
    );

// HORIZONTAL:
    std::string _gap = std::string(terminal::gapWidth        , ' ');
    std::string _mid = std::string(terminal::currentWidth - 2, ' ');

    // Top.
    terminal::print
    (
        0                      , 0,
        "┌{}┐{}┌{}┐{}┌{}┐",
        _mid, _gap,
        _mid, _gap,
        _mid
    );
    // Bottom.
    terminal::print
    (
        0                      , terminal::terminalHeight - 1,
        "└{}┘{}└{}┘{}└{}┘",
        _mid, _gap,
        _mid, _gap,
        _mid
    );

    // Middle.
    terminal::print
    (
        0                      , 1 + (terminal::currentHeight - 3),
        "└{}┘",
        std::string(terminal::currentWidth - 2, ' ')
    );
    terminal::print
    (
        0                      , 1 + (terminal::currentHeight - 3) + terminal::gapHeight + 2,
        "┌{}┐",
        std::string(terminal::previewWidth - 2, ' ')
    );

    std::cout.flush();
    input::read();

    terminal::cursor(true); // Also set upon ctrl+c exit
}