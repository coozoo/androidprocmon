# Android Process Monitor (androidprocmon)

## Summary
It's a tool that allow monitoring of android application with logging possibility through ADB. Actually it's some kind of UI for adb and tools on android device - top and dumpsys. There is chart represantation of live data, possibility to execute command on device and somekind of interface for logcat. So it's very good when you need to test your applicatuion on android device and monitor resources.

<img src="https://cloud.githubusercontent.com/assets/25594311/23156611/5b34452a-f820-11e6-9ba3-07fba869ef3c.png" width="60%"></img> 

## Installation
You can get precompiled package for your OS here:

https://github.com/coozoo/androidprocmon/releases

If you prefer to compile it by yourself then see below. 

### Windows
Suppose you have installed and configured:
* git
* QT + MinGW (not sure that with MSVC it will works)

Perform next steps:
 1. Download sources to your project dir:
 
 `
 git clone https://github.com/coozoo/androidprocmon
 `

 2. Open QtCreator
And open androidprocmon.pro
 3. Compile
 
  Optionaly if you want to run it outside qtcreator:
 4. place generated exe file to separate directory and run from commandline (or perform manual copy of ddls):
   
   `windeployqt androidprocmon.exe`


### Mac OSx
Suppose you have installed and configured:
- xCode
- QT (it should work fine with clang or gcc)

Perform next steps:
 1. Create dir to build project and change current dir to created one:
 
 ```shell
 $ cd ~
 
 $ mkdir proj
 
 $ cd proj
 ```
 
 2. Download sources to your project dir and browse to it:
 
 ```shell
 $ git clone https://github.com/coozoo/androidprocmon
 
 $ cd androidprocmon
 ```
 
 3. Try if it's compiling. Open QtCreator
 
 And open androidprocmon.pro
 
 4. Compile. In case of success continue if no try to find what's wrong.
 
 5. open MAC_build_RELEASE.sh and change variables if needed:
 
  - QT_DIR - path to your QT
  - BIN_DIR - folder to build
  
 6. make script executable and run this script
 
 `shell
 $ chmod 777 MAC_build_RELEASE.sh
 $ ./MAC_build_RELEASE.sh
 `
 
 If everything goes fine you will find deployed androidprocmon.app inside working dir.


### Linux
Currently there I didn't released any 
