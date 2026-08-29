#include "Menu.hpp"
#include "Task.hpp"
#include "Terminal.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include <climits>
#include <cstddef>
#include <vector>

namespace menu
{
    enum class MENU_CASE : int
    {
        BACK = -1,
        STAY =  0,
        NEXT =  1,
    };

    MENU_CASE edit_str(std::string &_str, input::KEYBIND _input)
    {
        switch (_input)
        {
            case input::KEYBIND::UP:                                                 return MENU_CASE::STAY;
            case input::KEYBIND::DOWN:                                               return MENU_CASE::STAY;
            case input::KEYBIND::CONFIRM:                                            return MENU_CASE::STAY;

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

    void print_parent_task(const Task &_task)
    {
        terminal::print
        (
            "%^"
            "NAME:        {}\n"
            "DESCRIPTION: {}\n"
            "STATUS:      {}\n"
            "SUBTASKS:    {}\n"
            "START:       {:04}/{:02}/{:02} {:02}:{:02}\n"
            "END:         {:04}/{:02}/{:02} {:02}:{:02}\n",
            _task.name,
            _task.description,
            _task.status,
            _task.subtasks.size(),
            _task.startTime.year, _task.startTime.month, _task.startTime.day, _task.startTime.hour, _task.startTime.minute,
            _task.endTime  .year, _task.endTime  .month, _task.endTime  .day, _task.endTime  .hour, _task.endTime  .minute
        );
    }
    void print_subtask(const Task &_subtask, const bool _selected)
    {
        terminal::print
        (
            "%^{}{}{} : {:04}/{:02}/{:02} {:02}:{:02} - {:04}/{:02}/{:02} {:02}:{:02}\n",
            _selected            ? "%*" : ""  ,
            _subtask.status == 1 ? "%g" : "%r",
            _subtask.name,
            _subtask.startTime.year, _subtask.startTime.month, _subtask.startTime.day, _subtask.startTime.hour, _subtask.startTime.minute,
            _subtask.endTime  .year, _subtask.endTime  .month, _subtask.endTime  .day, _subtask.endTime  .hour, _subtask.endTime  .minute
        );
    }
    void print_subtasks(const Task &_task, const size_t _selectedSubtask)
    {
        print_parent_task(_task);

        for (size_t _i = 0; _i < _task.subtasks.size(); ++_i)
        {
            print_subtask(_task.subtasks[_i], _i == _selectedSubtask);
        }
    }
    void display_task(Task &_task)
    {
        size_t _selectedSubtask = 0;
        while (true)
        {
            terminal::clear();
            print_subtasks(_task, _selectedSubtask);

            input::KEYBIND _input = input::read();

            switch (_input)
            {
                case input::KEYBIND::NEW:     new_task(_task);                                                                               break;
                case input::KEYBIND::REMOVE:  remove_task(_task);                                                                            break;
                case input::KEYBIND::EDIT:    edit_task(_task.subtasks[_selectedSubtask]);                                                   break;
                case input::KEYBIND::UP:      if (--_selectedSubtask == (size_t)-1           ) _selectedSubtask = _task.subtasks.size() - 1; break;
                case input::KEYBIND::DOWN:    if (++_selectedSubtask == _task.subtasks.size()) _selectedSubtask = 0;                         break;
                case input::KEYBIND::NEXT:    if (_task.subtasks.size() > 0) display_task(_task.subtasks[_selectedSubtask]);                 break;
                case input::KEYBIND::BACK:                                                                                                   return;

                case input::KEYBIND::CONFIRM:                                                                                                break;
                case input::KEYBIND::DELETE:                                                                                                 break;
                default:                                                                                                                     break;
            }
        }
    }

    enum class NEW_STATE : int
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

        CONFIRM     ,
    };
    void print_new_task(const Task &_task, const NEW_STATE _state)
    {
        terminal::print
        (
            "%^NAME:        {}{}\n"
            "%^DESCRIPTION: {}{}\n"
            "%^START:       {}{:04}%^/{}{:02}%^/{}{:02} {}{:02}%^:{}{:02}\n"
            "%^END:         {}{:04}%^/{}{:02}%^/{}{:02} {}{:02}%^:{}{:02}\n",
            _state == NEW_STATE::NAME         ? "%^%*" : "%^", _task.name            ,
            _state == NEW_STATE::DESCRIPTION  ? "%^%*" : "%^", _task.description     ,

            _state == NEW_STATE::START_YEAR   ? "%^%*" : "%^", _task.startTime.year  ,
            _state == NEW_STATE::START_MONTH  ? "%^%*" : "%^", _task.startTime.month ,
            _state == NEW_STATE::START_DAY    ? "%^%*" : "%^", _task.startTime.day   ,
            _state == NEW_STATE::START_HOUR   ? "%^%*" : "%^", _task.startTime.hour  ,
            _state == NEW_STATE::START_MINUTE ? "%^%*" : "%^", _task.startTime.minute,

            _state == NEW_STATE::END_YEAR     ? "%^%*" : "%^", _task.endTime.year  ,
            _state == NEW_STATE::END_MONTH    ? "%^%*" : "%^", _task.endTime.month ,
            _state == NEW_STATE::END_DAY      ? "%^%*" : "%^", _task.endTime.day   ,
            _state == NEW_STATE::END_HOUR     ? "%^%*" : "%^", _task.endTime.hour  ,
            _state == NEW_STATE::END_MINUTE   ? "%^%*" : "%^", _task.endTime.minute
        );
    }
    void new_task(Task &_parentTask)
    {
        Task _task
        {
            .name        = "",
            .description = "",
            .status      = 0,
            .subtasks    = std::vector<Task>(),
            .startTime   = Time::now(),
            .endTime     = Time::now(),
        };

        NEW_STATE _state = NEW_STATE::NAME;
        while (true)
        {
            terminal::clear();
            print_new_task(_task, _state);

            input::KEYBIND _input = input::read();

            switch (_state)
            {
                case NEW_STATE::CANCEL:       return;
                case NEW_STATE::NAME:         _state = (NEW_STATE)((int)_state + (int)edit_str(_task.name            , _input)); break;
                case NEW_STATE::DESCRIPTION:  _state = (NEW_STATE)((int)_state + (int)edit_str(_task.description     , _input)); break;
                case NEW_STATE::START_YEAR:   _state = (NEW_STATE)((int)_state + (int)edit_num(_task.startTime.year  , _input, 0, INT_MAX)); break;
                case NEW_STATE::START_MONTH:  _state = (NEW_STATE)((int)_state + (int)edit_num(_task.startTime.month , _input, 1, 12     )); break;
                case NEW_STATE::START_DAY:    _state = (NEW_STATE)((int)_state + (int)edit_num(_task.startTime.day   , _input, 1, 31     )); break;
                case NEW_STATE::START_HOUR:   _state = (NEW_STATE)((int)_state + (int)edit_num(_task.startTime.hour  , _input, 0, 23     )); break;
                case NEW_STATE::START_MINUTE: _state = (NEW_STATE)((int)_state + (int)edit_num(_task.startTime.minute, _input, 0, 59     )); break;
                case NEW_STATE::END_YEAR:     _state = (NEW_STATE)((int)_state + (int)edit_num(_task.endTime  .year  , _input, 0, INT_MAX)); break;
                case NEW_STATE::END_MONTH:    _state = (NEW_STATE)((int)_state + (int)edit_num(_task.endTime  .month , _input, 1, 12     )); break;
                case NEW_STATE::END_DAY:      _state = (NEW_STATE)((int)_state + (int)edit_num(_task.endTime  .day   , _input, 1, 31     )); break;
                case NEW_STATE::END_HOUR:     _state = (NEW_STATE)((int)_state + (int)edit_num(_task.endTime  .hour  , _input, 0, 23     )); break;
                case NEW_STATE::END_MINUTE:   _state = (NEW_STATE)((int)_state + (int)edit_num(_task.endTime  .minute, _input, 0, 59     )); break;
                case NEW_STATE::CONFIRM:      _parentTask.subtasks.push_back(_task); return; // Put in order based on start time.
            }
        }
    }
    void remove_task(Task &_task)
    {
        // ...
    }
    void edit_task(Task &_task)
    {
        // ...
    }
};