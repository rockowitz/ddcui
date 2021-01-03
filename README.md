**ddcui**
=======

**ddcui** is a graphical user interface for **ddcutil**, implemented using Qt. 

For a guide to building **ddcui** from source, see [Building ddcui](http://www.ddcutil.com/building_ddcui)
in the ddcutil documentation.

Note that it is possible for feature values reported in **ddcui** to become out of sync with actual monitor 
values.
- If feature values are changed using the monitor's On Screen Display. 
- If feature values are changed by another program, including the command line program **ddcutil**. 
- Some monitors will change their state (e.g. red gain) when the value in the GUI changes.
However, the value reported by the monitor is unchanged.
- Conversely, some monitors report a newly set value, but the observed state of the monitor
is unchanged.

The first two cases can generlly be corrected by menu item ***Actions->Rescan Feature Values***. 

## User Support

Please direct technical support questions, bug reports, and feature requests to the
[Issue Tracker](https://github.com/rockowitz/ddcui/issues) on the github repository.
Use of this forum allows everyone to benefit from individual questions and ideas.


## Author

Sanford Rockowitz  <rockowitz@minsoft.com>
