# Changelog

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
  - Requires glib version 2.39 for consistency with ddcutil.

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

