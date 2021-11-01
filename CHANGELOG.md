# Changelog

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
