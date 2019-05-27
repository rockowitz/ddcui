ddcui
=======

ddcui is an alpha version of a graphical user interface for ddcutil, implemented using Qt. 

It currently must be built from source. For basic instructions, see file ***BUILDING.md***. 

There are several situations that the GUI currently does not have a good way to handle:

- Some monitors will change their state (e.g. red gain) when the value in the GUI changes.
However, the value reported by the monitor is unchanged.
- Conversely, some monitors report a newly set value, but the observed state of the monitor
is unchanged.
- Casually swiping a mouse over the GUI can unintentionally change a slider value. It may be
necessary to add individual buttons for each feature controlling whether the feature value
is chnageable.

## User Support

Please direct technical support questions, bug reports, and feature requests to the
[Issue Tracker](https://github.com/rockowitz/ddcui/issues) on the github repository.
Use of this forum allows everyone to benefit from individual questions and ideas.


## Author

Sanford Rockowitz  <rockowitz@minsoft.com>
