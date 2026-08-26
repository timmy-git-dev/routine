OVERVIEW PAGE:
routine:
- 2026/08/22
- ...



TASK PAGE:
2026/08/22: description of task...
- sleep      0000 - 0400         -> complete           / green
- hygiene A  0400 - 0445 : 01/02 -> partially complete / orange
- breakfast  0445 - 0500 : 00/07 -> incomplete         / red
-            0500 - 0945         -> undefined          / dark gray
- work       0945 - 1315 : 01/04 -> in progress        / white
-            1315 - 2000         -> undefined          / dark gray
- sleep      2000 - 2400         -> upcoming           / gray



TASK PAGE:
hygiene A: description of task...
- shower 0400 - 0415 : 02/02 -> complete   / green
- shave  0415 - 0425         -> incomplete / red
-        0425 - 0445         -> undefined  / dark gray
...



MERGE PAGE:
breakfast: description of task...
↑ - merge with "hygiene A". # only if task above exists.
↓ - merge with "".          # only if task below exists.

← - cancel merge.           # if neither exist, simply do not show this menu.



SPLIT PAGE A:
breakfast: description of task...
↑ - increase split time.
↓ - decrease split time.

A 0445 - 0452
B 0452 - 0500

→ - change name of "A".
← - cancel.

SPLIT PAGE B:
breakfast: description of task...
_ - edit name of "example".

example 0445 - 0452
B       0452 - 0500

→ - change name of "B".
← - change split time.

SPLIT PAGE C:
breakfast: description of task...
_ - edit name of "test".

example 0445 - 0452
test    0452 - 0500

→ - split task.
← - change name of "example".



RENAME PAGE A:
breakfast: description of task...

_ - edit name of "testing".

testing 0445 - 0500 # description of task...

→ - change name of "testing".
← - cancel rename.

RENAME PAGE B:
breakfast: description of task...

_ - edit description of "testing".

testing 0445 - 0500 # example decription...

→ - rename task.
← - change name of "testing".



STORAGE:
HH 0 -> 1                         = start hour.
MM 2 -> 3                         = start minute.

HH 7 -> 8                         = end hour.
MM 9 ->10                         = end minute.

#  12                             = status.
## 14->15                         = subtask count.

"task name"        1st"+1->2nd"-1 = name.
"task description" 3rd"+1->4th"-1 = description.

HHMM - HHMM # ## "task name" : "task description"



KEYBINDS:
navigation:
- spacebar    : Change status (incomplete/complete). If status == incomplete, if current time > task end time, mark incomplete, otherwise mark upcoming, also does not work on unnamed tasks.
- up arrow    : Select the task above.
- down arrow  : Select the task below.
- right arrow : Enter selected task with description + sub-tasks, does not work on unnamed tasks.
- left arrow  : Return to parent task, quit on overview page, or otherwise specified by edit menu.

editing:
- s           : Split, present user with time to split (cannot be first/last minute), and the name of the first/second task.
- m           : Merge, present user with option to merge with previous/next task; unless no task above and/or below exists. Name becomes the currently selected task.
- r           : Rename, present user with new name of task.



NOTES:
- Tasks without names are considered undefined and cannot change status until renamed, and they can be split and merged with any other task.
- Splitting a task results in all subtasks being deleted and turned into a single, empty subtask.
- A task without any data, should simply show an empty subtask when entered.
- Empty tasks cannot be entered via right arrow.
- Empty tasks do not save data, it is assumed all tasks are put in order, and therefore gaps in time are considered empty tasks. That said, they are stored as task lists, up to the point where every task has a single empty subtask.
- Names/descriptions cannot contain quotation (") characters due to delimiter storage usage.



ADDITIONS?
- Grow/shrink tasks by increasing the last subtask in a task, and decreasing the first subtask in the next task. Can only adjust to the point where the subtasks still exist (>=1 min).