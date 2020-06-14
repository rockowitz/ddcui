ddcui
=======

ddcui is a beta version of a graphical user interface for ddcutil, implemented using Qt. 

For a guide to building ddcui from source, see [Building ddcui](http://www.ddcutil.com/building_ddcui)
in the ddcutil documentation.

There are some situations that ddcui does not have a good way to handle:

- Some monitors will change their state (e.g. red gain) when the value in the GUI changes.
However, the value reported by the monitor is unchanged.
- Conversely, some monitors report a newly set value, but the observed state of the monitor
is unchanged.


## User Support

Please direct technical support questions, bug reports, and feature requests to the
[Issue Tracker](https://github.com/rockowitz/ddcui/issues) on the github repository.
Use of this forum allows everyone to benefit from individual questions and ideas.


## Author

Sanford Rockowitz  <rockowitz@minsoft.com>
