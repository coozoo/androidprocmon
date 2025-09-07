# Android Process Monitor (androidprocmon)

## Summary
It's a tool that allow monitoring of android application with logging possibility through ADB. Actually it's some kind of UI for adb and tools on android device - top and dumpsys. There is chart representation of live data, possibility to execute command on device and some kind of interface for logcat. So it's very good when you need to test your application on android device and monitor resources.

<img width="1920" height="1054" alt="image" src="https://github.com/user-attachments/assets/b9ac314f-1484-4dc2-a971-0077368bc856" />

<img width="1920" height="1055" alt="image" src="https://github.com/user-attachments/assets/238535bc-cb44-449f-b4d2-9d08d8d2d342" />

## Installation

Precompiled RPMs (Fedora,RHEL etc) can be found in COPR click below:

```
$ sudo dnf copr enable yura/androidprocmon
$ sudo dnf install androidprocmon
```

Ubuntu add repo

```
sudo add-apt-repository ppa:coozoo/androidprocmon
sudo apt update
sudo apt-get install androidprocmon
```

You can get precompiled package for other OS here:

https://github.com/coozoo/androidprocmon/releases

[![Build Status](https://travis-ci.com/coozoo/androidprocmon.svg?branch=master)](https://travis-ci.com/coozoo/androidprocmon)

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
 
  Optionally if you want to run it outside qtcreator:
  
 4. place generated exe file to separate directory and run from command line (or perform manual copy of ddls):
   
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
Currently no any release for linux because of different dependencies so it's (suppose I will add some for fedora and debian). But now you need to compile it:

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
### Translations
Included languages are only:

English - androidprocmon_en

Ukrainian - androidprocmon_uk_UA

.ts-translations, .qm-compiled translations

So .ts translations files you can use to create your own translation.

Compiled translations .qm should be placed inside "lang" folder which should be placed beside binary.

### ADB place

The main component of this tool it's adb.

There is few predefined locations of adb tool but the better way to place it beside binary (exe file). So application will definitely find it.

### Command history

File "exec_history" is created automatically to log commands that executed on "Execute" so it's some kind of ".bash_history". 

So it's simply list of strings. File placed beside binary.

### Filter list

File "filters_list" its purpose is to save filters list to filter logcat messages.

Can be created manually. File placed beside binary.

It is csv like file and has next format:

```
FILTER,isRegex

FILTER - it's a word or regex to filter logcat;
isRegex - true/false is it regular expression or not.
```

### Chart Rules

File "chart_rules.json" contains the rules how many charts will be appeared and what data they drawing. File placed beside binary.

Attention rules handler very stupid  so it can work improperly, maybe later I will reimplement it.

You can copy included "chart_rules.json" file and place it beside binary.

Format next:
- two main objects "toprules" and "dumpsysrules";
- inside each section there is "charts" array of chart objects:
 - "chart" name of chart dock window;
 - "description" simple description;
 - "legend" name of chart over plot area;
 - "vaxislabel" axis lable actually what we are measuring.
- "graphs" array of graph (lines) objects:
 - "name" actually name of line;
 - "rule" actually what will be used as value for this line, it's name of column from the top and dumpsys table, can be used some simple operations like sum, division, multiplication (just try how it works and maybe you will find which one will work).

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

## How to use it

First at all you need to enable USB debugging to do that:

1. Go to android settings and scroll down;

2. If you see "Developers Options" then tap on it and continue from step 6;

3. To enable "Developers Options" go to "About Phone" and tap on it;

4. Scroll down and find "Build Number" tap on it a lot of time until you will see message "you are developer";

5. Go back to root of settings and find "Developers Options" tap on it;

6. Enable "Developers Options" by toggling switch on top of page;

7. Scroll down and find "USB debugging" switch on it by switching toggle to the right.

Now you can run application.

### Monitor Some process

If everything fine you will see devices list on the top of "ADB manager" tab after start, there is refresh button to update list of devices if connected after application start.

 1. Select device what you want a deal with:
 
    <img src="https://cloud.githubusercontent.com/assets/25594311/23210848/0651a226-f908-11e6-9a68-6a30c58b4293.png" width="50%"></img> 

 2. Go to "Process List" section and press refresh button to get the process list from device
   * Note: maybe you will see message on your android device about accept connection. Accept it, mark remember if you want, and press refresh again.
   
     <img src="https://cloud.githubusercontent.com/assets/25594311/23210845/0650ad6c-f908-11e6-99cb-6434aed82b95.png" width="50%"></img> 

 3. Select the process that you want to monitor, right click on it and select "Monitor Process" from context menu
 
    <img src="https://cloud.githubusercontent.com/assets/25594311/23210846/0650e930-f908-11e6-90b9-22866ed4cec3.png" width="50%"></img> 

 4. This process will be automatically added "Top Process Stat Log" and "Dumpsys Proc meminfo" sections. Arrows on them will be green to indicate that monitoring active.
   * lineedit to enter process name;
   * delay how often request data, default is 5 seconds;
   * "Stop" button to stop monitoring;
   * "Log" checkbox if marked then write to file (marked by default);
   * "Clear" button to clear stats in table.
    Exactly the same set of elements for dumpsys section.
    <img src="https://cloud.githubusercontent.com/assets/25594311/23210847/0651474a-f908-11e6-8201-38e315c20235.png" width="50%"></img>
    
 5. Now you can go to "Chart Manager" tab
    Here you will see online data on charts. 
    Every chart can be detached and placed back by double clicking on window header.

   * There is "Range" spinbox. You can set start size in minutes of plot area, as much as more data added it will compress it to fit into plot area. Flow checkbox allow you to left only last data of range, so if range 10 minutes then only last 10 minutes will be on chart.

   * Another possibility it's to save all charts as images by pressing button with floppy icon
   
   <img src="https://cloud.githubusercontent.com/assets/25594311/23210849/0653cd80-f908-11e6-984c-6baea369971e.png" width="50%"></img>

 6. All logged data and images are saved to "stats" folder
 
   * under windows "stats" folder beside exe file
   * linux and mac "stats" folder is inside the user dir `$ cd ~/androidprocmon/stats`
   
   Inside "stats" folder you will find folders with dates:
   
   dd.MM.yyyy_hh.mm.ss.xxx
   
   Inside each folder there .csv log files for top and dumpsys tables:
   
   NameOfProcess_pid_top.csv
   NameOfProcess_pid_dumpsys.csv
   
   Images are saved here in the "saved_charts" folder and arranged by folders with date when save button has been pressed.
   
   ### ADB logcat viewer
   There is possibility to view logcat.
   Simply mark "Enable" checkbox on the top left corner. And application will start logcat.
   Below there is another "Enable" checkbox related to filter section it will filter all messages by word or regexp.
   It is sometimes very useful to filter just some exceptions.
   
   <img src="https://cloud.githubusercontent.com/assets/25594311/23214289/c106e974-f916-11e6-93e5-3ceb397aee20.png" width="50%"></img>



