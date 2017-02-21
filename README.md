# Android Process Monitor (androidprocmon)

## Summary
It's a tool that allow monitoring of android application with logging possibility through ADB. Actually it's some kind of UI for adb and tools on android device - top and dumpsys. There is chart represantation of live data, possibility to execute command on device and somekind of interface for logcat. So it's very good when you need to test your applicatuion on android device and monitor resources.

<img src="https://cloud.githubusercontent.com/assets/25594311/23156611/5b34452a-f820-11e6-9ba3-07fba869ef3c.png" width="60%"></img> 

## Installation
You can get precompiled package for your OS here:
https://github.com/coozoo/androidprocmon/releases

### Windows
If you prefer to compile it by yourself. 
Suppose you have installed and configured:
* git
* QT + MinGW (not sure that with MSVC it will works)

Perform next steps:
 1. Download sources to your project dir:
git clone https://github.com/coozoo/androidprocmon
 2. Open QtCreator
And open androidprocmon.pro
 3. Compile
 
Optional:
 4. place generated exe file to separate directory and run from commandline:
   
   windeployqt androidprocmon.exe


### Mac OSx
### Linux

