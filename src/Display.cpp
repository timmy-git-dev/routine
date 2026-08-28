// #include "Display.hpp"
// #include "Input.hpp"
// #include "Task.hpp"
// #include <algorithm>
// #include <format>
// #include <iostream>
// #include <string>
// #include <vector>

// namespace display
// {
//     enum ANSII
//     {
//         BRIGHT_BLACK   = 90, REGULAR_BLACK   = 30,
//         BRIGHT_RED     = 91, REGULAR_RED     = 31,
//         BRIGHT_GREEN   = 92, REGULAR_GREEN   = 32,
//         BRIGHT_YELLOW  = 93, REGULAR_YELLOW  = 33,
//         BRIGHT_BLUE    = 94, REGULAR_BLUE    = 34,
//         BRIGHT_MAGENTA = 95, REGULAR_MAGENTA = 35,
//         BRIGHT_CYAN    = 96, REGULAR_CYAN    = 36,
//         BRIGHT_WHITE   = 97, REGULAR_WHITE   = 37,
//     };

//     enum COLOR
//     {
//         TITLE      = ANSII::BRIGHT_WHITE ,
//         INCOMPLETE = ANSII::BRIGHT_RED ,
//         COMPLETE   = ANSII::BRIGHT_GREEN ,
//         UPCOMING   = ANSII::BRIGHT_BLACK,
//         UNDEFINED  = ANSII::REGULAR_BLACK,
//         KEYBIND    = ANSII::REGULAR_BLACK,
//     };

//     void clear()
//     {
//         std::cout << "\033[2J\033[H";
//     }
//     void print(std::string _text, COLOR _color, bool _bold)
//     {
//         if (_bold) std::cout << "\033[1m";
//         else       std::cout << "\033[22m";

//         std::cout << "\033[" << _color << "m" << _text << "\n";
//     }

//     enum KEYBIND
//     {
//         STATUS = ' ',
//         SPLIT  = 's',
//         MERGE  = 'm',
//         RENAME = 'r',

//         UP     = 0,
//         DOWN   = 1,
//         IN     = 2,
//         OUT    = 3,
//         DELETE = 4,
//     };

//     Task empty_task(Time _start, Time _end)
//     {
//         return Task
//         {
//             .name        = "",
//             .description = "",
//             .status      = 0,
//             .startTime   = _start,
//             .endTime     = _end,
//             .subtasks    = std::vector<Task>(),
//         };
//     }
//     void print_task(Task &_task, bool _selected, size_t _maxName, size_t _maxDesc)
//     {
//         time_t _time = time(nullptr);
//         tm     _local { };
//         localtime_r(&_time, &_local);

//         std::string _text = std::format
//         (
//             "  {:<{}} : {:<{}} | {:02}{:02}-{:02}{:02}",
//             _task.name                                  , _maxName,
//             _task.name.empty() ? "" : _task.description , _maxDesc,
//             _task.startTime.hour, _task.startTime.minute,
//             _task.endTime  .hour, _task.endTime  .minute
//         );

//         COLOR _color =
//             _task.name.empty() ?
//                 COLOR::UNDEFINED :
//                 (
//                     _task.status == 1 ?
//                         COLOR::COMPLETE :
//                         (
//                             _local.tm_hour > _task.endTime.hour || (_local.tm_hour == _task.endTime.hour && _local.tm_min > _task.endTime.minute) ?
//                             COLOR::INCOMPLETE :
//                             COLOR::UPCOMING
//                         )
//                 );

//         print(_text, _color, _selected);
//     }
//     bool edit_time(std::string _title, Time &_time, Time _min, Time _max, std::string _keybinds)
//     {
//         Time _prev = _time;
//         while (true)
//         {
//             clear();
//             print(_title + ':', COLOR::TITLE, true);
//             print(std::format("{:02}{:02} → {:02}[{:02}]", _prev.hour, _prev.minute, _time.hour, _time.minute), COLOR::UPCOMING , true);
//             print(_keybinds, COLOR::KEYBIND, false);

//             char _input = input::wait_for_input();
//             switch (_input)
//             {
//                 case IN:
//                 {
//                     return true;
//                 }
//                 case OUT:
//                 {
//                     return false;
//                 }
//                 case UP:
//                 {
//                     if (++_time.minute == 60)
//                     {
//                         ++_time.hour;
//                         _time.minute = 0;
//                     }

//                     if (_time.hour > _max.hour || (_time.hour >= _max.hour && _time.minute > _max.minute))
//                     {
//                         _time = _min;
//                     }
//                     break;
//                 }
//                 case DOWN:
//                 {
//                     if (_time.minute-- == 0)
//                     {
//                         --_time.hour;
//                         _time.minute = 59;
//                     }

//                     if (_time.hour < _min.hour || (_time.hour == _min.hour && _time.minute < _min.minute))
//                     {
//                         _time = _max;
//                     }
//                     break;
//                 }
//                 default:
//                 {
//                     break;
//                 }
//             }
//         }
//     }
//     bool edit_text(std::string _title, std::string &_text, std::string _keybinds)
//     {
//         std::string _prev = _text;
//         while (true)
//         {
//             clear();
//             print(_title + ':', COLOR::TITLE, true);
//             print(_prev + " → " + _text + "_\n", COLOR::UPCOMING , true );
//             print(_keybinds, COLOR::KEYBIND, false);

//             char _input = input::wait_for_input();
//             switch (_input)
//             {
//                 case IN:
//                 {
//                     return true;
//                 }
//                 case OUT:
//                 {
//                     return false;
//                 }
//                 case DELETE:
//                 {
//                     if (_text.size() > 0) _text.erase(_text.size() - 1);
//                     break;
//                 }
//                 case '"':
//                 case '\n':
//                 case UP:
//                 case DOWN:
//                 {
//                     break;
//                 }
//                 default:
//                 {
//                     _text.push_back(_input);
//                     break;
//                 }
//             }
//         }
//     }

//     void menu_split(std::vector<Task> &_tasks, size_t _index)
//     {
//         Task &_subtask = _tasks[_index];
//         Task _taskA = Task
//         {
//             .name        = "task A"        ,
//             .description = "description...",
//             .status      = 0               ,
//             .startTime   = _subtask.startTime,
//             .endTime     = _subtask.endTime  ,
//             .subtasks    = std::vector<Task>()
//         };
//         Task _taskB = Task
//         {
//             .name        = "task B"        ,
//             .description = "description...",
//             .status      = 0               ,
//             .startTime   = _subtask.startTime,
//             .endTime     = _subtask.endTime  ,
//             .subtasks    = std::vector<Task>()
//         };

//         while(edit_text
//         (
//             "SPLIT",
//             _taskA.name,
//             "_ | edit name of 1st task.\n"
//             "→ | change description of 1st task.\n"
//             "← | cancel splitting task."
//         ))
//         {
//             while(edit_text
//             (
//                 "SPLIT",
//                 _taskA.description,
//                 "_ | edit description of 1st task.\n"
//                 "→ | change name of 2nd task.\n"
//                 "← | change 1st task name."
//             ))
//             {
//                 while(edit_text
//                 (
//                     "SPLIT",
//                     _taskB.name,
//                     "_ | edit name of 2nd task.\n"
//                     "→ | change description of 2nd task.\n"
//                     "← | change 1st task description."
//                 ))
//                 {
//                     while(edit_text
//                     (
//                         "SPLIT",
//                         _taskB.description,
//                         "_ | edit description of 2nd task.\n"
//                         "→ | change split time.\n"
//                         "← | change 2nd task name."
//                     ))
//                     {
//                         if (edit_time
//                         (
//                             "SPLIT",
//                             _taskA.endTime,
//                             _subtask.startTime,
//                             _subtask.endTime,
//                             "↑ - increase split time.\n"
//                             "↓ - decrease split time.\n"
//                             "→ | finish splitting tasks.\n"
//                             "← | change 2nd task description."
//                         ))
//                         {
//                             _taskB.startTime = _taskA.endTime;

//                             _taskA.subtasks.push_back(empty_task(_taskA.startTime, _taskA.endTime));
//                             _taskB.subtasks.push_back(empty_task(_taskB.startTime, _taskB.endTime));
//                             _tasks[_index] = _taskA;
//                             _tasks.insert(_tasks.begin() + _index + 1, _taskB);
//                             return;
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     Task menu_grow(std::vector<Task> &_tasks, size_t _index)
//     {

//     }
//     void menu_merge(std::vector<Task> &_tasks, size_t _index)
//     {
//         // while(edit_text
//         // (
//         //     "RENAME",
//         //     _name,
//         //     "_ | edit name of task.\n"
//         //     "→ | change description of task.\n"
//         //     "← | cancel renaming task."
//         // ))

//         Task &_taskA = _tasks[_index    ];
//         Task &_taskB = _tasks[_index + 1];

//         while (true)
//         {
//             clear();
//             print("MERGE:", COLOR::TITLE, true);
//             print_task(_taskA, true , std::max(_taskA.name.size(), _taskB.name.size()), std::max(_taskA.description.size(), _taskB.description.size()));
//             print_task(_taskB, false, std::max(_taskA.name.size(), _taskB.name.size()), std::max(_taskA.description.size(), _taskB.description.size()));
//             print
//             (
//                 "→ | confirm merging of tasks.\n"
//                 "← | cancel merging tasks.",
//                 COLOR::KEYBIND,
//                 false
//             );

//             char _input = input::wait_for_input();
//             switch (_input)
//             {
//                 case IN:
//                 {
//                     _taskA.endTime = _taskB.endTime;
//                     for (const Task &_task : _taskB.subtasks) _taskA.subtasks.push_back(_task);

//                     _tasks.erase(_tasks.begin() + _index + 1);
//                     return;
//                 }
//                 case OUT:
//                 {
//                     return;
//                 }
//                 default:
//                 {
//                     break;
//                 }
//             }
//         }
//     }
//     void menu_rename(Task &_task)
//     {
//         std::string _prevName        = _task.name;
//         std::string _prevDescription = _task.description;
//         std::string _name        = _prevName;
//         std::string _description = _prevDescription;

//         while(edit_text
//         (
//             "RENAME",
//             _name,
//             "_ | edit name of task.\n"
//             "→ | change description of task.\n"
//             "← | cancel renaming task."
//         ))
//         {
//             while(edit_text
//             (
//                 "RENAME",
//                 _description,
//                 "_ | edit description of task.\n"
//                 "→ | finish renaming task.\n"
//                 "← | change name of task."
//             ))
//             {
//                 _task.name        = _name;
//                 _task.description = _description;
//                 return;
//             }
//         }
//     }
//     void menu_task(std::string _name, std::vector<Task>& _subtasks)
//     {
//         size_t _currentIndex = 0;

//         while (true)
//         {
//             Task &_currentSubtask = _subtasks[_currentIndex];

//             clear();
//             print(_name + ':', COLOR::TITLE, true);

//             size_t _maxName = 0;
//             size_t _maxDesc = 0;
//             for (Task &_subtask : _subtasks)
//             {
//                 _maxName = std::max(_maxName, _subtask.name       .size());
//                 _maxDesc = std::max(_maxDesc, _subtask.description.size());
//             }

//             for (size_t _i = 0; _i < _subtasks.size(); ++_i)
//             {
//                 print_task(_subtasks[_i], _i == _currentIndex, _maxName, _maxDesc);
//             }

//             switch (input::wait_for_input())
//             {
//                 case STATUS:
//                 {
//                     _currentSubtask.status ^= 1;
//                     break;
//                 }
//                 case SPLIT:
//                 {
//                     menu_split(_subtasks, _currentIndex);
//                     break;
//                 }
//                 case MERGE:
//                 {
//                     if (_currentIndex < _subtasks.size() - 1) menu_merge(_subtasks, _currentIndex);
//                     break;
//                 }
//                 case RENAME:
//                 {
//                     menu_rename(_currentSubtask);
//                     break;
//                 }
//                 case UP:
//                 {
//                     _currentIndex = _currentIndex == 0 ? _subtasks.size() - 1 : _currentIndex - 1;
//                     break;
//                 }
//                 case DOWN:
//                 {
//                     _currentIndex = _currentIndex == _subtasks.size() - 1 ? 0 : _currentIndex + 1;
//                     break;
//                 }
//                 case IN:
//                 {
//                     if (_currentSubtask.name.empty()) break;
//                     menu_task(_currentSubtask.name, _currentSubtask.subtasks);
//                     break;
//                 }
//                 case OUT:
//                 {
//                     return;
//                 }
//                 default:
//                 {
//                     break;
//                 }
//             }
//         }
//     }

//     void begin(std::vector<Task> &_tasks)
//     {
//         menu_task("ROUTINE", _tasks);
//     }
// };