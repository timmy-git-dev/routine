#include "Menu.hpp"
#include "Task.hpp"
#include "Terminal.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include <algorithm>
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

        CONFIRM     ,
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
                case input::KEYBIND::NEW:     new_task(_task);                                                                                                                                        break;
                case input::KEYBIND::EDIT:    if (_task.subtasks.size() > 0) edit_task  (_task, _selectedSubtask);                                                                                    break;
                case input::KEYBIND::REMOVE:  if (_task.subtasks.size() > 0) remove_task(_task, _selectedSubtask); _selectedSubtask = std::clamp(_selectedSubtask, 0ul, _task.subtasks.size() - 1ul); break;
                case input::KEYBIND::UP:      _selectedSubtask = _selectedSubtask == 0                         ? _task.subtasks.size() - 1 : _selectedSubtask - 1;                                    break;
                case input::KEYBIND::DOWN:    _selectedSubtask = _selectedSubtask == _task.subtasks.size() - 1 ? 0                         : _selectedSubtask + 1;                                    break;
                case input::KEYBIND::NEXT:    if (_task.subtasks.size() > 0) display_task(_task.subtasks[_selectedSubtask]);                                                                          break;
                case input::KEYBIND::BACK:                                                                                                                                                            return;

                case input::KEYBIND::CONFIRM:                                                                                                                                                         break;
                case input::KEYBIND::DELETE:                                                                                                                                                          break;
                default:                                                                                                                                                                              break;
            }
        }
    }

    void print_new_task(const Task &_task, const TASK_STATE _state)
    {
        terminal::print
        (
            "%^NAME:        {}{}\n"
            "%^DESCRIPTION: {}{}\n"
            "%^START:       {}{:04}%^/{}{:02}%^/{}{:02} {}{:02}%^:{}{:02}\n"
            "%^END:         {}{:04}%^/{}{:02}%^/{}{:02} {}{:02}%^:{}{:02}\n",
            _state == TASK_STATE::NAME         ? "%^%*" : "%^", _task.name            ,
            _state == TASK_STATE::DESCRIPTION  ? "%^%*" : "%^", _task.description     ,

            _state == TASK_STATE::START_YEAR   ? "%^%*" : "%^", _task.startTime.year  ,
            _state == TASK_STATE::START_MONTH  ? "%^%*" : "%^", _task.startTime.month ,
            _state == TASK_STATE::START_DAY    ? "%^%*" : "%^", _task.startTime.day   ,
            _state == TASK_STATE::START_HOUR   ? "%^%*" : "%^", _task.startTime.hour  ,
            _state == TASK_STATE::START_MINUTE ? "%^%*" : "%^", _task.startTime.minute,

            _state == TASK_STATE::END_YEAR     ? "%^%*" : "%^", _task.endTime.year  ,
            _state == TASK_STATE::END_MONTH    ? "%^%*" : "%^", _task.endTime.month ,
            _state == TASK_STATE::END_DAY      ? "%^%*" : "%^", _task.endTime.day   ,
            _state == TASK_STATE::END_HOUR     ? "%^%*" : "%^", _task.endTime.hour  ,
            _state == TASK_STATE::END_MINUTE   ? "%^%*" : "%^", _task.endTime.minute
        );
    }
    void new_task(Task &_parentTask)
    {
        TASK_STATE _state = TASK_STATE::NAME;
        Task       _task
        {
            .name        = "",
            .description = "",
            .status      = 0,
            .subtasks    = std::vector<Task>(),
            .startTime   = Time::now(),
            .endTime     = Time::now(),
        };

        while (true)
        {
            terminal::clear();
            print_new_task(_task, _state);

            input::KEYBIND _input = input::read();

            switch (_state)
            {
                case TASK_STATE::NAME:         _state = (TASK_STATE)((int)_state + (int)edit_str(_task.name            , _input));             if (_state != TASK_STATE::CANCEL) break;
                case TASK_STATE::CANCEL:                                                                                                       return;
                case TASK_STATE::DESCRIPTION:  _state = (TASK_STATE)((int)_state + (int)edit_str(_task.description     , _input));             break;
                case TASK_STATE::START_YEAR:   _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.year  , _input, 0, INT_MAX)); break;
                case TASK_STATE::START_MONTH:  _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.month , _input, 1, 12     )); break;
                case TASK_STATE::START_DAY:    _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.day   , _input, 1, 31     )); break;
                case TASK_STATE::START_HOUR:   _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.hour  , _input, 0, 23     )); break;
                case TASK_STATE::START_MINUTE: _state = (TASK_STATE)((int)_state + (int)edit_num(_task.startTime.minute, _input, 0, 59     )); break;
                case TASK_STATE::END_YEAR:     _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .year  , _input, 0, INT_MAX)); break;
                case TASK_STATE::END_MONTH:    _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .month , _input, 1, 12     )); break;
                case TASK_STATE::END_DAY:      _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .day   , _input, 1, 31     )); break;
                case TASK_STATE::END_HOUR:     _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .hour  , _input, 0, 23     )); break;
                case TASK_STATE::END_MINUTE:   _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .minute, _input, 0, 59     )); if (_state != TASK_STATE::CONFIRM) break;
                case TASK_STATE::CONFIRM:
                {
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

    void remove_task(Task &_parentTask, const size_t _index)
    {
        _parentTask.subtasks.erase(_parentTask.subtasks.begin() + _index);
    }

    void print_edit_task(const Task &_task, const TASK_STATE _state)
    {
        terminal::print
        (
            "%^NAME:        {}{}\n"
            "%^DESCRIPTION: {}{}\n"
            "%^START:       {}{:04}%^/{}{:02}%^/{}{:02} {}{:02}%^:{}{:02}\n"
            "%^END:         {}{:04}%^/{}{:02}%^/{}{:02} {}{:02}%^:{}{:02}\n",
            _state == TASK_STATE::NAME         ? "%^%*" : "%^", _task.name            ,
            _state == TASK_STATE::DESCRIPTION  ? "%^%*" : "%^", _task.description     ,

            _state == TASK_STATE::START_YEAR   ? "%^%*" : "%^", _task.startTime.year  ,
            _state == TASK_STATE::START_MONTH  ? "%^%*" : "%^", _task.startTime.month ,
            _state == TASK_STATE::START_DAY    ? "%^%*" : "%^", _task.startTime.day   ,
            _state == TASK_STATE::START_HOUR   ? "%^%*" : "%^", _task.startTime.hour  ,
            _state == TASK_STATE::START_MINUTE ? "%^%*" : "%^", _task.startTime.minute,

            _state == TASK_STATE::END_YEAR     ? "%^%*" : "%^", _task.endTime.year  ,
            _state == TASK_STATE::END_MONTH    ? "%^%*" : "%^", _task.endTime.month ,
            _state == TASK_STATE::END_DAY      ? "%^%*" : "%^", _task.endTime.day   ,
            _state == TASK_STATE::END_HOUR     ? "%^%*" : "%^", _task.endTime.hour  ,
            _state == TASK_STATE::END_MINUTE   ? "%^%*" : "%^", _task.endTime.minute
        );
    }
    void edit_task(Task &_parentTask, const size_t _index)
    {
        TASK_STATE _state = TASK_STATE::NAME;
        Task       _task  = _parentTask.subtasks[_index];

        while (true)
        {
            terminal::clear();
            print_edit_task(_task, _state);

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
                case TASK_STATE::END_MINUTE:   _state = (TASK_STATE)((int)_state + (int)edit_num(_task.endTime  .minute, _input, 0, 59     )); if (_state != TASK_STATE::CONFIRM) break;
                case TASK_STATE::CONFIRM:
                {
                    _parentTask.subtasks.erase(_parentTask.subtasks.begin() + _index);

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
