# Building **ddcui**

## Requirements
- ddcutil 0.9.6 or later.  If **ddcutil** is installed from pre-compiled packages, 
the shared library and development packages must be installed as well as the main ddcutil package. 
- A recent version of QT 5.  **ddcui** will not build with versions of Qt older than 5.5. 
Unless you want to modify the the dialog boxes, or prefer the Qt Creator IDE, Qt Creator is not required. 

## Building Using qmake
- A qmake control file (***ddcui.pro***) is provided.
- To create the Makefile, change to the directory in which you wish to 
  build **ddcui** and issue one of the commands: 
~~~
  qmake -qt=qt5 <directory containing ddcui.pro>/ddcui.pro
  qtchooser -qt=5 -run-tool=qmake <directory containing ddcui.pro>/ddcui.pro
~~~

## Building Using CMake
- A CMake control file (***CMakeLists.txt***) is provided.
- To create the Makefile, change to the directory in which you wish to 
  build **ddcui** and issue the command: 
~~~
   cmake <directory containing CMakeLists.txt> 
~~~  
- Using the -S and -B options can ensure that the proper directory is used for the build tree. 
See the cmake man page for detais.

## Using QtCreator

User arupda has written up [notes](https://github.com/rockowitz/ddcui/issues/1) on building **ddcui** using QtCreator and CMake.
Note that arupda has modified the source to build with an earlier version of Qt. 


## Issues: 
- If ddcutil is installed from the tarball using command ***make install***, the
PKGCONFIG file ddcutil.pc is installed in a subdirectory of /usr/local.  Depending on Linux distribution, 
this subdirectory may or may not be in the default PKGCONFIG search path.  For example, on Ubuntu it is in the 
search path, but on Fedora it is not.  To add the relevant path, issue the command: 
~~~
export PKG_CONFIG_DIR=/usr/local/lib/pkgconfig
~~~
before running ***qmake*** or ***cmake***. 

To see the default PKGCONFIG search path, issue the command:
~~~
pkg-config --variable pc_path pkg-config
~~~

For additional information, [Shared Library Configuration](http://www.ddcutil.com/shared_lib_config.md)
