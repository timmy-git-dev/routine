#include "Menu.hpp"
#include "Terminal.hpp"
#include "Input.hpp"
#include <cstddef>

namespace menu
{
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
            "END:         {:04}/{:02}/{:02} {:02}:{:02}\n"
            "---\n",
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
                case input::KEYBIND::UP:      if (++_selectedSubtask == _task.subtasks.size()) _selectedSubtask = 0;                         break;
                case input::KEYBIND::DOWN:    if (--_selectedSubtask == (size_t)-1           ) _selectedSubtask = _task.subtasks.size() - 1; break;
                case input::KEYBIND::NEXT:    if (_task.subtasks.size() > 0) display_task(_task.subtasks[_selectedSubtask]);                 break;
                case input::KEYBIND::BACK:                                                                                                   return;

                case input::KEYBIND::CONFIRM:                                                                                                break;
                case input::KEYBIND::DELETE:                                                                                                 break;
                default:                                                                                                                     break;
            }
        }
    }
    void new_task(Task &_task)
    {
        // ...
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