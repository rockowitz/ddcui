# Building **ddcui**

## Requirements
- ddcutil 0.9.1 or later.  If installed from pre-compiled packages, the shared library and development packages must be installed
as well as the main ddcutil package.
- A recent version of QT 5.  **ddcui** will not build with versions of Qt older then 5.5. 
Unless you want to modify the the dialog boxes, or prefer the Qt Creator IDE, Qt Creator is not required. 

## Building Using qmake
- A qmake control file (***ddcui.pro***) is provided.
- To create the Makefile, change to the directory in which you wish to 
  build **ddcui** and issue the command: 
~~~
  qmake -qt=qt5 <directory containing ddcui.pro>/ddcui.pro
~~~

## Building Using CMake
- A CMake control file (***CMakeLists.txt***) is provided.
- To create the Makefile, change to the directory in which you wish to 
  build **ddcui** and issue the command: 
~~~
   cmake <directory containing CMakeLists.txt> 
~~~  

## Using QtCreator

User arupda has written up [notes](https://github.com/rockowitz/ddcui/issues/1) on building **ddcui** using QtCreator and CMake.
Note that arupda has modified the source to build with an earlier version of Qt. 


## Issues: 
- Depending on platform, if ddcutil is installed from the tarball using command ***make install***, the
PKGCONFIG file ddcutil.pc may be installed in a subdirectory of /usr/local that is not searched by the 
macros in ddcui.pro and CMakeLists.txt. 
