#include "Menu.hpp"
#include "Data.hpp"
#include "Task.hpp"
#include "Terminal.hpp"
#include "Input.hpp"
#include <algorithm>
#include <climits>
#include <cstddef>
#include <format>
#include <vector>

namespace menu
{
    enum class MENU_CASE : int
    {
        BACK = -1,
        STAY =  0,
        NEXT =  1,
    };
    enum class TASK_STATE : int
    {
        CANCEL  = -1,

        NAME        ,
        DESCRIPTION ,
        START_YEAR  ,
        START_MONTH ,
        START_DAY   ,
        START_HOUR  ,
        START_MINUTE,
        END_YEAR    ,
        END_MONTH   ,
        END_DAY     ,
        END_HOUR    ,
        END_MINUTE  ,

        ENTER       ,
    };

    Task copiedTask;

    MENU_CASE edit_str(std::string &_str, input::KEYBIND _input)
    {
        switch (_input)
        {
            case input::KEYBIND::UP:                                                 return MENU_CASE::STAY;
            case input::KEYBIND::DOWN:                                               return MENU_CASE::STAY;
            case input::KEYBIND::ENTER:                                              return MENU_CASE::STAY;

            case input::KEYBIND::NEXT:                                               return MENU_CASE::NEXT;
            case input::KEYBIND::BACK:                                               return MENU_CASE::BACK;

            case input::KEYBIND::DELETE:  if (_str.size() > 0) {_str.pop_back();}    return MENU_CASE::STAY;
            default:                      _str.push_back(static_cast<char>(_input)); return MENU_CASE::STAY;
        }
    }
    MENU_CASE edit_num(int &_num, input::KEYBIND _input, int _min, int _max)
    {
        switch (_input)
        {
            case input::KEYBIND::UP:   if (_num++ == _max) {_num = _min;} return MENU_CASE::STAY;
            case input::KEYBIND::DOWN: if (_num-- == _min) {_num = _max;} return MENU_CASE::STAY;

            case input::KEYBIND::NEXT: return MENU_CASE::NEXT;
            case input::KEYBIND::BACK: return MENU_CASE::BACK;

            default:                   return MENU_CASE::STAY;
        }
    }

    const std::string _horizontal = "────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────";
    const std::string _vertical   = "││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││││";

    void print_border_bg(size_t _x0, size_t _y0, size_t _width, size_t _height)
    {
        size_t _x1 = _x0 + _width - 1;
        size_t _y1 = _y0 + 1;
        size_t _y2 = _y0 - 1 + _height;

        terminal::print(_x0, _y0,                     "%W┌"); terminal::print(_x1, _y0,                       "┐");
        terminal::print(_x0, _y1, 1, _height - 2, _vertical); terminal::print(_x1, _y1, 1, _height - 2, _vertical);
        terminal::print(_x0, _y2,                       "└"); terminal::print(_x1, _y2,                       "┘");
    }
    void print_border_fg(size_t _x0, size_t _y0, size_t _width, size_t _height)
    {
        size_t _x1 = _x0 + _width - 1;
        size_t _y1 = _y0 + 1;
        size_t _y2 = _y0 - 1 + _height;

        terminal::print(_x0, _y0,                     "%K╭"); terminal::print(_x0 + 1, _y0, _width - 2, 1, _horizontal); terminal::print(_x1, _y0,                       "╮");
        terminal::print(_x0, _y1, 1, _height - 2, _vertical);                                                            terminal::print(_x1, _y1, 1, _height - 2, _vertical);
        terminal::print(_x0, _y2,                       "╰"); terminal::print(_x0 + 1, _y2, _width - 2, 1, _horizontal); terminal::print(_x1, _y2,                       "╯");
    }

    void print_bg()
    {
        print_border_bg(0                                              , 0                                               , terminal::panelWidth, terminal::splitPanelHeight); // Current.
        print_border_bg(0                                              , terminal::splitPanelHeight + terminal::gapHeight, terminal::panelWidth, terminal::splitPanelHeight); // Preview.
        print_border_bg( terminal::panelWidth + terminal::gapWidth     , 0                                               , terminal::panelWidth, terminal::panelHeight     ); // Subtasks.
        print_border_bg((terminal::panelWidth + terminal::gapWidth) * 2, 0                                               , terminal::panelWidth, terminal::splitPanelHeight); // Command.
        print_border_bg((terminal::panelWidth + terminal::gapWidth) * 2, terminal::splitPanelHeight + terminal::gapHeight, terminal::panelWidth, terminal::splitPanelHeight); // Keybinds.
    }

    void print_current_task(const Task &_task)
    {
        print_border_fg(2, 1, terminal::panelWidth - 4, terminal::splitPanelHeight - 2);
        terminal::print(2, 0, "%^%*%wCurrent Task");

        terminal::print(4, terminal::splitPanelHeight - 6, terminal::panelWidth - 8, 1                             , "%^%k{}   ", _horizontal     );

        terminal::print(4, 1                             , terminal::panelWidth - 8, 1                             , "%^%*%k{}", _task.name       );
        terminal::print(4, 3                             , terminal::panelWidth - 8, terminal::splitPanelHeight - 9, "%^%/%K{}", _task.description);
        terminal::print(4, terminal::splitPanelHeight - 5                                                          , "%^%/%K{:04}/{:02}/{:02}-{:02}:{:02}", _task.startTime.year, _task.startTime.month, _task.startTime.day, _task.startTime.hour, _task.startTime.minute);
        terminal::print(4, terminal::splitPanelHeight - 4                                                          , "%^%/%K{:04}/{:02}/{:02}-{:02}:{:02}", _task.endTime  .year, _task.endTime  .month, _task.endTime  .day, _task.endTime  .hour, _task.endTime  .minute);
    }
    void print_preview(const Task &_task)
    {
        const size_t _y = terminal::splitPanelHeight + terminal::gapHeight;

        print_border_fg(2, _y + 1, terminal::panelWidth - 4, terminal::splitPanelHeight - 2);
        terminal::print(2, _y    , "%^%*%wPreview");

        terminal::print(4, _y + terminal::splitPanelHeight - 6, terminal::panelWidth - 8, 1                             , "%^%k{}   ", _horizontal     );

        terminal::print(4, _y + 1                             , terminal::panelWidth - 8, 1                             , "%^%*%k{}", _task.name       );
        terminal::print(4, _y + 3                             , terminal::panelWidth - 8, terminal::splitPanelHeight - 9, "%^%/%K{}", _task.description);
        terminal::print(4, _y + terminal::splitPanelHeight - 5                                                          , "%^%/%K{:04}/{:02}/{:02}-{:02}:{:02}", _task.startTime.year, _task.startTime.month, _task.startTime.day, _task.startTime.hour, _task.startTime.minute);
        terminal::print(4, _y + terminal::splitPanelHeight - 4                                                          , "%^%/%K{:04}/{:02}/{:02}-{:02}:{:02}", _task.endTime  .year, _task.endTime  .month, _task.endTime  .day, _task.endTime  .hour, _task.endTime  .minute);
    }
    void print_subtask(const Task &_task, size_t _index, const bool _selected)
    {
        const size_t _x = terminal::panelWidth + terminal::gapWidth + 2;

        print_border_fg(_x, 1 + _index * 4, terminal::panelWidth - 4, 4);
        terminal::print(_x + 2, 1 + _index * 4, terminal::panelWidth - 8, 1, "%^{}%k{}", _selected ? "%*" : "",_task.name       );
        terminal::print(_x + 2, 2 + _index * 4, terminal::panelWidth - 8, 1, "%^%/%K{:04}/{:02}/{:02}-{:02}:{:02}", _task.startTime.year, _task.startTime.month, _task.startTime.day, _task.startTime.hour, _task.startTime.minute);
        terminal::print(_x + 2, 3 + _index * 4, terminal::panelWidth - 8, 1, "%^%/%K{:04}/{:02}/{:02}-{:02}:{:02}", _task.endTime  .year, _task.endTime  .month, _task.endTime  .day, _task.endTime  .hour, _task.endTime  .minute);
    }
    void print_subtasks(const Task &_task, const size_t _offset, const size_t _count, const size_t _selectedSubtask)
    {
        terminal::print(terminal::panelWidth + terminal::gapWidth + 2, 0, "%^%*%wSubtasks");

        for (size_t _i = _offset; _i < _offset + _count; ++_i)
        {
            print_subtask(_task.subtasks[_i], _i - _offset, _i == _selectedSubtask);
        }
    }
    void print_base(const Task &_task, size_t _selectedSubtask, size_t &_offset)
    {
        size_t _max = std::min(_task.subtasks.size(), (terminal::panelHeight - 2) / 4ul);
        if (_selectedSubtask      <  _offset       ) _offset = _selectedSubtask;
        else if (_selectedSubtask >= _offset + _max) _offset = _selectedSubtask - _max + 1;

        print_bg();
        print_current_task(_task);
        print_subtasks(_task, _offset, _max, _selectedSubtask);
        if (_task.subtasks.size() > 0)
        {
            print_preview(_task.subtasks[_selectedSubtask]);
        }
    }
    void print_info(const std::string &_info)
    {
        size_t _x = (terminal::panelWidth + terminal::gapWidth) * 2 + 2;
        size_t _y = terminal::splitPanelHeight + terminal::gapHeight;

        terminal::print(_x, _y, "%^%*%wInfo");
        terminal::print(_x + 1, _y + 2, terminal::panelWidth - 6, terminal::splitPanelHeight - 4, _info);
    }

    void insert_task(Task &_parentTask, Task &_task)
    {
        for (size_t _i = 0; _i < _parentTask.subtasks.size(); ++_i)
        {
            if (_task.startTime < _parentTask.subtasks[_i].startTime)
            {
                _parentTask.subtasks.insert(_parentTask.subtasks.begin() + _i, copiedTask);
                return;
            }
        }

        _parentTask.subtasks.push_back(_task);
        return;
    }

    void display_task(Task &_task)
    {
        size_t _selectedSubtask = 0;
        size_t _offset          = 0;
        while (true)
        {
            std::string _info;
            _info.reserve(512);

                                            _info += std::format("%^%W[%w←%W] %KExit current task %*{}%^%K.\n"      , _task.name);
            if (_task.subtasks.size() > 0) {_info += std::format("%^%W[%w→%W] %KEnter selected subtask %*{}%^%K.\n" , _task.subtasks[_selectedSubtask].name);}
                                            _info += '\n';
                                            _info +=             "%^%W[%wn%W] %KCreate new subtask.\n";
            if (_task.subtasks.size() > 0) {_info += std::format("%^%W[%we%W] %KEdit selected subtask %*{}%^%K.\n"  , _task.subtasks[_selectedSubtask].name);}
            if (_task.subtasks.size() > 0) {_info += std::format("%^%W[%wr%W] %KRemove selected subtask %*{}%^%K.\n", _task.subtasks[_selectedSubtask].name);}
                                            _info += '\n';
            if (_task.subtasks.size() > 0) {_info += std::format("%^%W[%wc%W] %KCopy selected subtask %*{}%^%K.\n"  , _task.subtasks[_selectedSubtask].name);}
                                            _info += std::format("%^%W[%wv%W] %KPaste copied subtask %*{}%^%K.\n"   , copiedTask                      .name);


            print_base(_task, _selectedSubtask, _offset);
            print_info(_info);
            terminal::write();

            input::KEYBIND _input = input::read();

            switch (_input)
            {
                case input::KEYBIND::NEW:     new_task(_task, _selectedSubtask, _offset);                                                                                                             break;
                case input::KEYBIND::EDIT:    if (_task.subtasks.size() > 0) edit_task  (_task, _selectedSubtask, _offset);                                                                           break;
                case input::KEYBIND::REMOVE:  if (_task.subtasks.size() > 0) remove_task(_task, _selectedSubtask); _selectedSubtask = std::clamp(_selectedSubtask, 0ul, _task.subtasks.size() - 1ul); break;
                case input::KEYBIND::COPY :   if (_task.subtasks.size() > 0) copiedTask = _task.subtasks[_selectedSubtask];                                                                                           break;
                case input::KEYBIND::PASTE:   insert_task(_task, copiedTask);                                                                                                                         break;
                case input::KEYBIND::EXPORT:  if (_task.subtasks.size() > 0) data::csv("./routine/export.csv", _task.subtasks[_selectedSubtask]);                                                     break;

                case input::KEYBIND::UP:      _selectedSubtask = _selectedSubtask == 0                         ? _task.subtasks.size() - 1 : _selectedSubtask - 1;                                    break;
                case input::KEYBIND::DOWN:    _selectedSubtask = _selectedSubtask == _task.subtasks.size() - 1 ? 0                         : _selectedSubtask + 1;                                    break;

                case input::KEYBIND::NEXT:    if (_task.subtasks.size() > 0) display_task(_task.subtasks[_selectedSubtask]);                                                                          break;
                case input::KEYBIND::BACK:                                                                                                                                                            return;

                default:                                                                                                                                                                              break;
            }
        }
    }

    void print_command(std::string _title, const Task &_oldTask, const Task &_newTask, const TASK_STATE _state)
    {
        const size_t _x = (terminal::panelWidth + terminal::gapWidth) * 2 + 2;

        print_border_fg(_x, 1, terminal::panelWidth - 4, terminal::splitPanelHeight - 2);
        terminal::print(_x, 0, _title);
        terminal::print(_x + 2, terminal::splitPanelHeight - 6, terminal::panelWidth - 8, 1, "%^%k{}", _horizontal);

        #define BOLD(STATE) _state == TASK_STATE::STATE ? "%*" : ""

        terminal::print(_x + 2, 1, terminal::panelWidth - 8, 1                             ,                                                     "%^%K{}", _oldTask.name);
        terminal::print(_x + 2, 1, terminal::panelWidth - 8, 1                             , _state == TASK_STATE::NAME        ? "%^%k{}%^%K_" : "%^%k{}", _newTask.name);

        terminal::print(_x + 2, 3, terminal::panelWidth - 8, terminal::splitPanelHeight - 9,                                                     "%^%w{}", _oldTask.description);
        terminal::print(_x + 2, 3, terminal::panelWidth - 8, terminal::splitPanelHeight - 9, _state == TASK_STATE::DESCRIPTION ? "%^%K{}%^%w_" : "%^%K{}", _newTask.description);

        terminal::print(_x + 2, terminal::splitPanelHeight - 5, "%^%K{}{:04}%^%K/{}{:02}%^%K/{}{:02}%^%K-{}{:02}%^%K:{}{:02}", BOLD(START_YEAR), _newTask.startTime.year, BOLD(START_MONTH), _newTask.startTime.month, BOLD(START_DAY), _newTask.startTime.day, BOLD(START_HOUR), _newTask.startTime.hour, BOLD(START_MINUTE), _newTask.startTime.minute);
        terminal::print(_x + 2, terminal::splitPanelHeight - 4, "%^%K{}{:04}%^%K/{}{:02}%^%K/{}{:02}%^%K-{}{:02}%^%K:{}{:02}", BOLD(END_YEAR  ), _newTask.endTime  .year, BOLD(END_MONTH  ), _newTask.endTime  .month, BOLD(END_DAY  ), _newTask.endTime  .day, BOLD(END_HOUR  ), _newTask.endTime  .hour, BOLD(END_MINUTE  ), _newTask.endTime  .minute);


        #define INFO_STR(EXIT, ENTER, TYPE)                                     \
        {                                                                       \
            _info += std::format("%^%W[%w←%W] %K "     EXIT  "\n"        );     \
            _info += std::format("%^%W[%w→%W] %K "     ENTER "\n"        );     \
            _info += std::format("%^%W[%w_%W] %KEdit " TYPE " of task.\n");     \
            break;                                                              \
        }
        #define INFO_NUM(EXIT, ENTER, TYPE)                                     \
        {                                                                       \
            _info += std::format("%^%W[%w←%W] %K"          EXIT  "\n");         \
            _info += std::format("%^%W[%w→%W] %K"          ENTER "\n");         \
            _info += std::format("%^%W[%w↑%W] %KIncrease " TYPE ".\n");         \
            _info += std::format("%^%W[%w↓%W] %KDecrease " TYPE ".\n");         \
            break;                                                              \
        }

        std::string _info;
        _info.reserve(256);

        switch (_state)
        {
            case TASK_STATE::NAME:          INFO_STR("Cancel creating new task.", "Change description of task.", "name")
            case TASK_STATE::DESCRIPTION:   INFO_STR("Change name of task.", "Change start-year of task.", "description")
            case TASK_STATE::START_YEAR:    INFO_NUM("Change description of task.", "Change start-month of task.", "start-year")
            case TASK_STATE::START_MONTH:   INFO_NUM("Change start-year of task.", "Change start-day of task.", "start-month")
            case TASK_STATE::START_DAY:     INFO_NUM("Change start-month of task.", "Change start-hour of task.", "start-day")
            case TASK_STATE::START_HOUR:    INFO_NUM("Change start-day of task.", "Change start-minute of task.", "start-hour")
            case TASK_STATE::START_MINUTE:  INFO_NUM("Change start-hour of task.", "Change end-year of task.", "start-minute")
            case TASK_STATE::END_YEAR:      INFO_NUM("Change start-minute of task.", "Change start-month of task.", "end-year")
            case TASK_STATE::END_MONTH:     INFO_NUM("Change end-year of task.", "Change start-day of task.", "end-month")
            case TASK_STATE::END_DAY:       INFO_NUM("Change end-month of task.", "Change start-hour of task.", "end-day")
            case TASK_STATE::END_HOUR:      INFO_NUM("Change end-day of task.", "Change start-minute of task.", "end-hour")
            case TASK_STATE::END_MINUTE:    INFO_NUM("Change end-hour of task.", "Finish creating new task.", "end-minute")

            default: break;
        }

        print_info(_info);
    }

    void new_task(Task &_parentTask, const size_t _index, size_t &_offset)
    {
        TASK_STATE _state = TASK_STATE::NAME;
        Task       _oldTask
        {
            .name        = "name",
            .description = "description",
            .status      = 0,
            .subtasks    = std::vector<Task>(),
            .startTime   = Time::now(),
            .endTime     = Time::now(),
        };
        Task       _newTask = _oldTask;
        _newTask.name        = "";
        _newTask.description = "";

        while (true)
        {
            print_base(_parentTask, _index, _offset);
            print_command("%^%*%wNew", _oldTask, _newTask, _state);
            terminal::write();

            input::KEYBIND _input = input::read();

            switch (_state)
            {
                case TASK_STATE::NAME:         _state = (TASK_STATE)((int)_state + (int)edit_str(_newTask.name            , _input));             if (_state != TASK_STATE::CANCEL) break;
                case TASK_STATE::CANCEL:                                                                                                          return;
                case TASK_STATE::DESCRIPTION:  _state = (TASK_STATE)((int)_state + (int)edit_str(_newTask.description     , _input));             break;
                case TASK_STATE::START_YEAR:   _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.startTime.year  , _input, 0, INT_MAX)); break;
                case TASK_STATE::START_MONTH:  _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.startTime.month , _input, 1, 12     )); break;
                case TASK_STATE::START_DAY:    _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.startTime.day   , _input, 1, 31     )); break;
                case TASK_STATE::START_HOUR:   _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.startTime.hour  , _input, 0, 23     )); break;
                case TASK_STATE::START_MINUTE: _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.startTime.minute, _input, 0, 59     )); break;
                case TASK_STATE::END_YEAR:     _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.endTime  .year  , _input, 0, INT_MAX)); break;
                case TASK_STATE::END_MONTH:    _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.endTime  .month , _input, 1, 12     )); break;
                case TASK_STATE::END_DAY:      _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.endTime  .day   , _input, 1, 31     )); break;
                case TASK_STATE::END_HOUR:     _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.endTime  .hour  , _input, 0, 23     )); break;
                case TASK_STATE::END_MINUTE:   _state = (TASK_STATE)((int)_state + (int)edit_num(_newTask.endTime  .minute, _input, 0, 59     )); if (_state != TASK_STATE::ENTER) break;
                case TASK_STATE::ENTER:        insert_task(_parentTask, _newTask);                                                                                                 return;
            }
        }
    }

    void remove_task(Task &_parentTask, const size_t _index)
    {
        _parentTask.subtasks.erase(_parentTask.subtasks.begin() + _index);
    }

    void add_subtask_time(Task &_task, const Time &_time)
    {
        for (Task &_subtask : _task.subtasks)
        {
            _subtask.startTime.add_time(_time);
            _subtask.endTime  .add_time(_time);
            add_subtask_time(_subtask, _time);
        }
    }

    void edit_task(Task &_parentTask, const size_t _index, size_t &_offset)
    {
        TASK_STATE _state = TASK_STATE::NAME;
        Task       _task  = _parentTask.subtasks[_index];
        Time       _prevTime = _task.startTime;

        while (true)
        {
            print_base(_parentTask, _index, _offset);
            print_command("%^%*%wEdit", _parentTask.subtasks[_index], _task, _state);
            terminal::write();

            input::KEYBIND _input = input::read();

            switch (_state)
            {
                case TASK_STATE::NAME:         _state = (TASK_STATE)((int)_state + (int)edit_str(_task.name            , _input            )); if (_state != TASK_STATE::CANCEL) break;
                case TASK_STATE::CANCEL:                                                                                                       return;
                case TASK_STATE::DESCRIPTION:  _state = (TASK_STATE)((int)_state + (int)edit_str(_task.description     , _input            )); break;
                case TASK_STATE::START_YEAR:   _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.year  , _input, 0, INT_MAX)); break;
                case TASK_STATE::START_MONTH:  _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.month , _input, 1, 12     )); break;
                case TASK_STATE::START_DAY:    _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.day   , _input, 1, 31     )); break;
                case TASK_STATE::START_HOUR:   _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.hour  , _input, 0, 23     )); break;
                case TASK_STATE::START_MINUTE: _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.minute, _input, 0, 59     )); break;
                case TASK_STATE::END_YEAR:     _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .year  , _input, 0, INT_MAX)); break;
                case TASK_STATE::END_MONTH:    _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .month , _input, 1, 12     )); break;
                case TASK_STATE::END_DAY:      _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .day   , _input, 1, 31     )); break;
                case TASK_STATE::END_HOUR:     _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .hour  , _input, 0, 23     )); break;
                case TASK_STATE::END_MINUTE:   _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .minute, _input, 0, 59     )); if (_state != TASK_STATE::ENTER) break;
                case TASK_STATE::ENTER:
                {
                    _parentTask.subtasks.erase(_parentTask.subtasks.begin() + _index);
                    Time _timeChange = _task.startTime;
                    _timeChange.sub_time(_prevTime);
                    add_subtask_time(_task, _timeChange);

                    for (size_t _i = 0; _i < _parentTask.subtasks.size(); ++_i)
                    {
                        if (_task.startTime < _parentTask.subtasks[_i].startTime)
                        {
                            _parentTask.subtasks.insert(_parentTask.subtasks.begin() + _i, _task);
                            return;
                        }
                    }

                    _parentTask.subtasks.push_back(_task);
                    return;
                }
            }
        }
    }
};
