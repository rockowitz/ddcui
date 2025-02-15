# Changelog

## [0.6.0] 2025-02-15

Requires libddcutil from ddcutil 2.2.0 or later.

### Changed

- Dynamically add and remove displays as reported by libddcutil using a callback function.

- Add API button to stats dialog

## Building
- Use "universal" headers in source code for compatibility between Qt5 and Qt6 (per Luis Periera)
[ddcui pull rquest 64: Fixes a FTBFS using Qt6](https://github.com/rockowitz/ddcui/pull/64)


#@ [0.5.4] 2024-02-13

### Added

- Install file /usr/lib/modules-load.d/ddcui.conf to ensure that 
  kernel module i2c-dev is loaded.

## [0.5.3] 2024-02-07

- Require libddcutil from ddcutil 2.1.3 or later

## [0.5.2] 2024-01-27 

- Ensure display status change detection is enabled for display connection
  and disconnection, not for DPMS asleep.

## [0.5.0] 2023-01-17

### Added

- When libddcutil detects connection or disconnection of a display, ddcui 
  puts up a dialog box reporting the change and indicating that the user
  needs to redetect displays using Action->Redetect Displays.
- Options ***--enable-watch-displays***, ***--disable-watch-displays***
  By default **ddcui** watches for display connection events.

### Changed

- If multiple "options" lines are found in a segment of configuration file
  ddcuirc, their contents are combined into a single value.

### Building

- Requires libddcutil.so.5.1 or later
- If CMakeLists.txt option ASAN is specified, cmake compiles ddcui 
using options required for ASAN and links the executable with libasan.

## [0.4.2] 2023-09-27

### Added

- Option ***--disable-syslog***.  Normally **ddcui** and **libddcutil** write
  important messages to the system log.  This option disables that behavior.
- Option ***--libopts "<option string>"***.  Passes an option string to the 
  shared library.  This string is appended to the option string obtained 
  from configuration file $HOME/.config/ddcutil/ddcutilrc.
- Menu item Actions->Debug Locks reports locking the **libddcutil** as a
  debugging aid.

### Changed

- CMakeLists.txt recognizes option DDCUTIL_PROJECT_DIR, which specifies the
  **ddcutil** project directory in which **libddcutil** is built. Using this
  option avoids having to install **libddcutil.so** in a system directory in 
  order for **ddcui** to use it.
- Incremented required libddcutil version to 1.5.0

### Removed

- **ddcui** options that exist solely to control **libddcutil** behavior.
These include ***--ddc*** and ***--excp***, ***--trace***, ***--trcfunc***, and ***--trcfile***. 
  These can now be passed as text strings using **ddcui** option ***--libopts***.


## [0.3.0] 2022-07-28

### Added
- CTL-Q terminates **ddcui** (does not apply within dialog boxes)

### Changed
- Errors opening /dev/i2c and /dev/usb/hiddev devices are reporting using a message box
  instead of to the terminal. These are typically lack of permissions (Linux error EACCESS).
- Optionally requiring the control key to be pressed when changing feature values now applies
  to all changes, not just those made using sliders. (The option is set using command line 
  option ***--require-control-key*** or the UI Options dialog box).
  - A status message is issued when the control key is required.
- Special handling for feature X60: 
  - Allow extra sleep time before validation.  (It was observed that a Dell U4320 returns
    a Null Message if getvcap for validation is called too quickly
- For simple NC values, do not include the SH field in validation.
  (getvcp for feature x60 on Dell U4320 returns non-zero SH.)
- Options ***--force-slave-address*** and ***--disable-force-slave-address*** have no effect
  and are deprecated.

### Fixed
- Option dialogs: Tab key was not jumping to the OK and Cancel buttons
- Fix free() before final use in certain cases of function tracing.
- Custom feature lists: Fix assert statements.
- Memory leaks
- Cast many pointers to ( void* ) to avoid compiler warnings

## [0.2.2] 2022-02-22

### Fixed
- Erroneous assert() statment that could cause compilation failure or segfault in ValueSpecialWidgetX62.cpp.

## [0.2.1] 2022-01-28

### Added
- Command line option ***--force-slave-address***. Its meaning for **ddcui** is the same as for **ddcutil**.
  See [Option: --force-slave-address](https://www.ddcutil.com/other_options/#option-force-slave-address) 
  and section [Device Busy (EBUSY) Errors](https://www.ddcutil.com/release_notes/#device-busy-ebusy-errors) 
  on the [ddcutil web site](https://www.ddcutil.com). 
- Command line option ***--disable-force-slave-address*** disables the setting in case it has been 
  enabled by shared library configuration. 

### Changed
- If monitor communication fails because of Linux error EBUSY, the Summary view reports
  it as "Busy" instead of "Invalid" suggests use of option ***--force-slave-address***.
- The Capabilities and Feature views for monitor cannot be selected if it doesn't support 
  DDC/CI, or if the the capabilities string cannot be read.
- Disable all view selection if no monitor is detected.
- More consistently use the term "display" instead of "Monitor" in the user interface.
- **ddcui** recognizes environment variable DDCUTIL_DEBUG_PARSE to enable tracing in 
  command line parsing without recompiling.
- CMakeLists.txt has been extensively updated.
  - The minimum supported version is specified as 3.10, as that is known to work.
    If need be, CMakeLists.txt can be edited to specify	an earlier version and 
	the script will likely still work.
  - The script uses command line option ***--log-level*** to control message output.
    Message level VERBOSE is new as of cmake 3.15. If the script is being executed
	on an earlier version, messages marked VERBOSE are treated as STATUS. 
  - Command line option ***--use-cache*** controls whether ccache is used to speed
    builds (assuming it is installed). The default is ***--use-ccache=yes***. 
  - Building in the source directories is not allowed.
  - Better support for cmake variable CMAKE_BUILD_TYPE. The default value is 
    RelWithDebInfo. Validation of the supplied value  has been factored out into 
	file validate_CMAKE_BUILD_TYPE.cmake.
  - Requires glib version at least 2.40 for function g_option_context_strv().

### Fixed 
- Segfaults
  - In particular, handle the pathological case, reported for Gentoo, where
    glib function g_option_context_parse_strv() fails but does not return 
	an error message.
- Numerous memory leaks
- Code simplification, particularly for display initialization and error 
  message propogation

### Removed
- The "Use Latest Feature Values" setting in the NC Feature Values dialog, and
  corresponding command line option ***--use-latest-nc-values***.  This feature
  was hard to document and convoluted to implement. Instead, the User Defined
  Features facility allows for definition of the possible NC feature values
  for a monitor feature.

## [0.2.0] 2021-10-15 

This log summarizes user visible changes in **ddcui**. 

Though the list of changes is short, in fact the code base has been extensively reworked.  
For details, see [ddcui Release Notes](https://www.ddcutil.com/ddcui_release_notes).

### Added
- Support VCP features, e.g. x62 (Audio Balance), that have both a continuous 
  range. and a set of special values. The widget for these features has both a
  slider/spinbox for  the continuous range, and a combo-box for the special values.
- Desktop integration
   - Install ddcui.desktop in /usr/share/applications
   - Install icons in /usr/share/icons/hicolor, /usr/share/pixmaps
   - Install ddcui.appdata.xml in /usr/share/metainfo
- Configuration file, typically $HOME/.config/ddcutil/ddcutilrc, which is shared
  with **ddcutil** and **libddcutil**. 
- Special handling for unrecognizd NC feature values
  - When  obtaining the value id and names for a feature, **ddcui** normally uses the 
    values and names specified for the monitor's MCCS version.  Sometimes monitors 
    use feature values defined in a later version. The ***Use Latest MCCS Values*** 
    checkbox in the **NC Feature Values** dialog specifies that the NC value table 
    for the highest applicable MCCS version be used.  This can also be specified by 
    command line option ***--force-latest-nc_value-names***.
 - Handle the case where a monitor reports a NC value that is not in 
   the MCCS or capabilities list.
- Menubar option Actions->Redetect Monitors tells libddcutil to redetect displays. 
  The caller should replace its data structures to reflect the new list of monitors.

### Changed
- Use "video-display" icon from Oxygen theme
- Rename the "Other Options" dialog to "NC Feature Values"
- Revise and extend help screen content
- User interface cleanup
- Command line option ***--help*** does not report deveelopment related options.
  Use ***--help-developmen*** or ***--help-all***.

### Removed
- qmake is no longer supported, use cmake
- Command line options affecting only libddcutil that can be specified instead
  in configuration file ddcutilrc: --udf, --no-udf, --nousb, --maxtries, 
  --sleep-multiplier, --sleep-less, --less-sleep, --no-less-sleep, 
  --dynamic-sleep-adjustment

