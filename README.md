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
Currently no any release for linux becuase of different dependencies so it's (suppose I will add some for fedora and debian). But now you need to compile it:

Suppose you have installed QT5 (on fedora you can do that # dnf install qt5*):
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
 3. Compile by executing next commands (maybe you need to use qmake if default is qt5):
 
 ```shell
 $ qmake-qt5
 
 $ make
 ```

## Configuration
###Translations
Included languages are only:

English - androidprocmon_en

Ukrainian - androidprocmon_uk_UA

.ts-translations, .qm-compiled translations

So .ts translations files you can use to create your own translation.

Compiled translations .qm should be placed inside "lang" folder which should be placed beside binary.

###ADB place
The main component of this tool it's adb.
There is few predefined locations of adb tool but the better way to place it beside binary (exe file). So application will definitely find it.

###Command history
File "exec_history" is created automatically to log commands that executed on "Execute" so it's some kind of ".bash_history". 

So it's simply list of strings. File placed beside binary.

###Filter list
File "filters_list" its purpose is to save filters list to filter logcat messages.
Can be created manually. File placed beside binary.

It is csv like file and has next format:
```
FILTER,isRegex

FILTER - it's a word or regex to filter logcat;
isRegex - true/false is it regular expression or not.
```

###Chart Rules
File "chart_rules.json" contains the rules how much charts and what date they drawing. File placed beside binary.
Attention rules handler very stupid  so it can work wrongly, maybe later I will reimplement it.

You can copy included "chart_rules.json" file and place it beside binary.

Format next:
- two main objects "toprules" and "dumpsysrules";
- inside each section there is "charts" array of chart objects:
 - "chart" name of chart dock window;
 - "description" simple description;
 - "legend" name of chart over plot area;
 - "vaxislabel" axis lable actually what we are mesuaring.
- "graphs" array of graph (lines) objects:
 - "name" actually name of line;
 - "rule" actually what will be used as value for this line, it's name of column from the top and dumpsys table, can be used some simple operations like summ, division, multiplication (just try how it works and maybe you will find which one will work).

Example:
```json
{
"toprules":{"charts":[
{"chart":"CPU","description":"Total CPU load vs Process","legend":"CPU load","vaxislabel":"percents %","graphs":[{"name":"[CPU User+System]","rule":"CPU(User)\\+CPU(System)"},{"name":"[CPU User]","rule":"CPU(User)"},{"name":"CPU process","rule":"CPU%"}]}
]},
"dumpsysrules":{"charts":[
{"chart":"Pss_Total","description":"","legend":"TOTAL|Pss_Total","vaxislabel":"kB","graphs":[{"name":"Pss_Total","rule":"TOTAL|Pss_Total"}]},
{"chart":"Private_Dirty","description":"","legend":"TOTAL|Private_Dirty","vaxislabel":"kB","graphs":[{"name":"Private_Dirty","rule":"TOTAL|Private_Dirty"}]}
]}
}
```

##How to use it
