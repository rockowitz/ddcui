# Changelog

## [0.7.0] 2026-07-15

Requires libddcutil.so.5.5 from ddcutil 2.2.7 or later.

### Added

#### Miscellaneous

- Option ***--view***, specifies initial view (Summary, Capabilities, or
  Features). The default is Summary.

#### Special handling for ill-behaved monitors

Some monitors set a different brightness value from the one specified on the 
setvcp request. For example, Dell model 2407WFP adjusts request values less
than 50 upwards so that the value set is never less than 30.  Hence verifiation
of the value set will often fail. Other monitors, e.g. Samsung LS34A650U, adjust
the monitor brightness according to the value requested, but reading the value 
of feature x10 (brightness) always returns 0. 

Two new options address these problematic cases.

- Option: ***--noverify-x10***, Do not report an error if the value of feature 
  x10 that is read immediately after set the feature is set does not match the 
  value requested. The argument is the model name, as taken from the EDID and
  shown in the Display combo box. 
- Option: ***--estimate-x10***.  The value initially shown in the Features view 
  is 70% of the maximum value, as reported by get feature value response packet.
  For example, if the maximum value is 255, the initial value shown will be 179.
  This will undoubtedly be wrong. However, when the slider is moved or the 
  number in the spin box is changed, the new feature value is sent to the monitor. 
  There can be no check that the value is correctly set, but if it is correctly 
  set the then value shown in the user interface will will now match the actual
  value on the monitor. The argument to this option is the model name, as taken
  from the EDID and shown in the Display combo box.


#### Improved tracing facilities

Trace Options: 
- ***--trcmethod*** (alt. ***--trcmeth***, ***--trcfunc***). The argument can be
  of the form Class::Method or simply a method/function name.  The latter will 
  match a method in any class that has that name, as well as a non-class function.
- ***--trcfile***.  Turn on all tracing for a file, specified as its base name,
  with or without a trailing ".c" or ".cpp".
- ***--trcclass***: Takes as its argument the name of class to trace.
- ***--trcmetaclass***: Takes as its argument the name of a metaclass to trace. 
Commands
- **traceable-methods** (alt **traceable functions**). List all methods/functions 
  that can be traced.
- **traceable-classes**: List classes that can be traced.
- **traceable-metaclasses**: List traceable metaclasses


### Changed

- CMakeLists.txt defaults to QT 6. The minimum supported QT 6 version is 6.7.
  To build with QT5, use cmake argument ***-DUSE_QT5=ON***. Support for QT5 
  will be removed in a later release.
- The check for the model name specified in option ***--model***
  is now case-insensitive.
- Improved tracing facilities.
 
### Fixed

- Improve handling of display connection changes reported by libddcutil.
  Action **Redetect Displays** should no longer be needed, but is retained
  as a recovery operation.
- More closely adhere to ambiguous appdata naming standards.
  Renamed files:
  - ddcui.appdata.xml -> com.ddcutil.ddcui.metainfo.xml
  - ddcui.desktop -> com.ddcutil.ddcui.desktop
  Command **make install** erases files with the old names if they exist.
  Addresses pull request #71
- CMakeLists.txt: 
    - Replace deprecated execute_program() with exec_process().
    - Incorrect version check caused the VERBOSE message level to never be
      used, even if it is available on the current CMake Version.
    - Properly handle Qt5/Qt6 differences in include directory and linked 
      libraries.
    - Fix assignment of root directory of ddcutil project (DDCUTIL_PROJECT_DIR),
      so that the most recent version of libddcutil its header files are used 
      instead of ones obtained from /usr or /usr/local.
    - Typo in environment variable name (LD_LIBRARY_PATH) was misspelled 
      "LD_LIBRRARY_PATH". Caused invalid diagnostic message.  
    - Miscellaneous message typos.
- Thread safety: serialized concurrent access to shared model and queue state
  that runs across ddcui's worker threads.
    - **FeatureBaseModel** guards **_featureValues** and **_featuresChecked**
      with a mutex, makes **_caps_check_complete** atomic (set after the data
      fields), and makes the **FeatureValue** id counter atomic.
    - **MsgBoxQueue** serializes its queue with its mutex, and
      **VcpRequestQueue::pop()** guards the condition-variable wait with a loop
      rather than a single test.
    - Worker threads shut down cleanly: **MsgBoxThread** terminates on shutdown,
      and the **Monitor** destructor halts the request queue rather than
      posting a halt request.
- Robustness: many places that asserted on libddcutil API results, or on the
  presence of feature information, now handle failure gracefully instead of
  crashing — main window API-result handling and key-release event, a missing
  **FeatureValue** on value update, null feature metadata in the feature and
  value widgets, and display-info and feature-list query failures. Several
  previously uninitialized member variables are now initialized (feature and
  value widgets, spin slider, main window pointers, and the getvcp value
  record).
- Memory leaks and stale state fixed: feature-value instances are freed in the
  **FeatureBaseModel** destructor; leaks and stale state on feature reload; a
  custom feature-list parse error path; view-signal connections accumulating
  across monitor reloads; summary, capabilities, and dialog child widgets; and
  a dangling pointer in trace code.
- User interface fixes: remove the correct combo box entry when a monitor is
  removed; show the invalid-value message for aux fields; read both comboboxes
  when setting a byte value; forward focus and mouse events from the number
  entry widget; remove a duplicate value-widget signal connection; expire
  duplicate-message suppression after 60 seconds; bound the capabilities retry
  loop; and correct the ***--syslog*** help text.
- Corrected several malformed trace format strings.


## [0.6.0] 2025-02-15

Requires libddcutil.so.5.2 from ddcutil 2.2.0 or later.

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

