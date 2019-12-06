**ddcui**
=======

**ddcui** is a graphical user interface for **ddcutil**, implemented using Qt.

This is early beta level code, and many features are not yet fully implemented, including online help, error reporting, and proper interpretation of certain peculiar VCP features. 

For basic instructions on building **ddcui**, see file ***BUILDING.md***. Both a CMake ***CMakeLists.txt***
and a Qt ***ddcui.pro*** file are provided.
Additional documentation can be found on the [ddcutil web site](https://www.ddcutil.com).

Some user interface issues should be noted in advance.

It is possible for feature values reported in **ddcui** to become out of sync with actual monitor 
values.
- If feature values are changed using the monitor's On Screen Display. 
- If feature values are changed by another program, including the command line program **ddcutil**. 
- Some monitors will change their state (e.g. red gain) when the value in the GUI changes.
However, the monitor still reports the old value, which is shown in **ddcui**.  This is a bug 
in the monitor's DDC/CI implementation.
- Conversely, some monitors will report a newly set value, but the observed state of the monitor
is unchanged. Again, this is a bug in the monitor's DDC/CI implementation.

In some, but not all, cases, **ddcui** can be resynced with the actual monitor settings using menu item
***Actions->Rescan Monitor Values***. 

A second issue is that casually swiping a mouse over the feature values section of the GUI, or using the mouse's scroll wheel over the feature values, can unintentionally move a slider value
and thus change a feature value.  Setting menu item ***Options->User Interface Options->Require control key to move sliders*** will change the user
interface to require that a control key be held down while moving the slider. 
README.md:

## User Support

Please direct technical support questions, bug reports, and feature requests to the
[Issue Tracker](https://github.com/rockowitz/ddcui/issues) on the github repository.
Use of this forum allows everyone to benefit from individual questions and ideas.


## Author

Sanford Rockowitz  <rockowitz@minsoft.com>
