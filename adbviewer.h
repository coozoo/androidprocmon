/* Author: Yuriy Kuzin
 * Some widget class that provides different adb functions:
 * - logcat with regexp posibility;
 * - execute shell commands using adb;
 * - get Process list and select one for monitoring;
 * - monitor process CPU memory... using top;
 * - monitor process java memory, sql, views.. using dumpsys;
 * it's good to be broke it onto few smaller classes
 */
#ifndef ADBVIEWER_H
#define ADBVIEWER_H

#include <QApplication>
#include <QMainWindow>
#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QProcess>
#include <QRegExp>
#include <QRegularExpression>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QToolBox>
#include <QComboBox>
#include <QToolButton>
#include <QPushButton>
#include <QCheckBox>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QLineEdit>
#include <QScrollBar>
#include <QMenu>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QStringList>
#include <QSpinBox>
#include <QCompleter>
#include <QStringListModel>
#include <QSpacerItem>
#include <QSettings>
#include "qplaintexteditcursor.h"
#include "adbexecute.h"



class adbViewer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString strDateTimeFile
           READ getstrDateTimeFile
           WRITE setstrDateTimeFile
        )
    Q_PROPERTY(QString adbBinary
           READ getadbBinary
           WRITE setadbBinary
        )
    Q_PROPERTY(QString androidDevice
           READ getandroidDevice
           WRITE setandroidDevice
        )
public:
    explicit adbViewer(QWidget *parent = 0);
    ~adbViewer();


    //yes i know ugly global variables.. but i need it
    //to handle .csv column changing without readin file every time
    //and currently no implemented
    //now columns amount came from csv file
    //QMap<QString, int> csvFileColums_map;

    QGridLayout *main_gridlayout;
    QToolBox *main_toolbox;
    QToolButton *refreshDevices_toolbutton;
    QHBoxLayout *toolbar_hboxlayout;

    //logcat section
    QProcess *proc;
    QCheckBox *logcatEnable_checkbox;
    QCheckBox *showFilterSection_checkbox;
    QPlainTextEdit *logcatAllResultADB_plaintextedit;
    QPlainTextEdit *logcatFiltered_plaintextedit;
    QComboBox *filterCondition_combobox;
    QToolButton *rememberFilter_toolbutton;
    QToolButton *deleteFilter_toolbutton;
    QCheckBox *useRegexp_checkbox;
    QCheckBox *enableFilter_checkbox;
    QSpacerItem *top_spacer;
    QSpacerItem *regexp_spacer;
    QGridLayout *topLogcat_gridlayout;
    QGridLayout *regexpLogcat_gridlayout;
    void updateFilters();

//    //execute command section
//    QPlainTextEditCursor *execresult_plaintextedit;
//
//    QProcess *procexec;
//    QCheckBox *procexecGetSU_checkbox;
//    QCheckBox *executeUseLoop_checkbox;
//    QSpinBox *executeUseLoopDelay_spinbox;

//    QLineEdit *execute_lineedit;
//    QCompleter *completer;
//    int modelCursor;
    QString executeADBCommand(QString adbCommand);

//    QPushButton *executeCommand_pushbutton;
//    QToolButton *resetConsole_toolbutton;

    //process list section
    QPushButton *refreshProcessList_pushbutton;
    QTableView *processList_tableview;
    QStandardItemModel *processList_model;
    QLineEdit *processListStatus_lineedit;

    //top monitor section
    QToolButton *processTopStatClear_toolbutton;
    QPushButton *processTopStatStartStop_pushbutton;
    QLineEdit * processTopStatFilter_lineedit;
    QTableView *processTopStat_tableview;
    QCheckBox *processTopStatEnableLog_checkbox;
    QStandardItemModel *processTopStat_model;
    QLineEdit *processTopStatStatus_lineedit;
    QProcess *processTopStatExec;
    QSpinBox *processTopStatTimeout_spinbox;

    //dumpsys monitor section
    QToolButton *processDumpsysStatClear_toolbutton;
    QPushButton *processDumpsysStatStartStop_pushbutton;
    QLineEdit * processDumpsysStatFilter_lineedit;
    QTableView *processDumpsysStat_tableview;
    QCheckBox *processDumpsysStatEnableLog_checkbox;
    QStandardItemModel *processDumpsysStat_model;
    QLineEdit *processDumpsysStatStatus_lineedit;
    QProcess *processDumpsysStatExec;
    QSpinBox *processDumpsysStatTimeout_spinbox;
    QString processDumpsysAndroidVersion;

    adbExecute *execute_dock;

    QComboBox *devicesList_combobox;
    void startlogcat();

    QStringList policyList;

    void getSU();

    //some kind of global buffer which updated from processTopStatUpdateText
    //don't forget to reset it after/before using (actually stopping/starting top process monitor)
    //I will change it later maybe to another way (you know what does it mean)
    int totalCPUcolumnCounter;
    QString totalCPUBuffer;

    //functions to get column header id inside model for each table by name
    //yeah I know that it's possible to do one function but for me it's fine
    int getHeaderIDbyNameOfProcessList(QString columnName);
    int getHeaderIDbyNameOfProcessTopStat(QString columnName);
    int getHeaderIDbyNameOfProcessDumpsysStat(QString columnName);

    //append line to csv log file (in dumpsys columns are floating so sometimes file can be little bit jammed i will fix it later)
    void writeStatCSV(QString appendFileName, QString logLine, QString headersList);

    void startProcessTopStat();

    //functions to write history (commands and regexp) into file
    QString ReadFullFile(QString strPath);

    void setstrDateTimeFile(QString m_strDateTimeFile)
    {
        strDateTimeFile = m_strDateTimeFile;
    }
    QString getstrDateTimeFile() const
    { return strDateTimeFile; }


    void setadbBinary(QString m_adbBinary)
    {
        adbBinary = m_adbBinary;
        emit adbPath_changed();
    }
    QString getadbBinary() const
    { return adbBinary; }

    void setandroidDevice(QString m_androidDevice)
    {
        androidDevice = m_androidDevice;
        emit androidDeviceChanged(androidDevice);
    }
    QString getandroidDevice() const
    { return androidDevice; }

    QString whereAdbExists();


private:
    QTimer dumpsysProcTimer;
    QString strDateTimeFile;
    QString androidDevice;
    QString adbBinary="adb";


signals:
    void regexpMessageReceived(QString payLoad);
    void processTopStatNewData(QString headersList,QString dataList);
    void processDumpsysStatNewData(QString headersList,QString dataList);
    void dumpsysProcStopped();
    void dumpsysProcStarted();
    void RewriteFullFileSignal(QString strPath, QString appendString);
    void androidDeviceChanged(QString strAndroidDevice);
    void adbPath_changed();

public slots:

private slots:
    void on_refreshDevices_toolbutton_clicked();
    void on_logcatEnable_checkbox_clicked();
    void on_showFilterSection_checkbox_clicked();
    void on_rememberFilter_toolbutton_clicked();
    void on_deleteFilter_toolbutton_clicked();
    void on_filterCondition_combobox_currentIndexChanged(int index);

    void on_devicesList_combobox_currentIndexChanged(int);
    void on_proc_closed(int reason);
    void on_proc_started();


    void updateText();
    void updateError();

    void getProcesslist();
    void getProcesslist_error();
    void getProcesslist_standard();
    void on_topProcessExec_closed(int reason);
    void clearProcesslist();

    void on_processTopStatClear_toolbutton_clicked();
    void on_processTopStatStartStop_pushbutton_clicked();
    void clearProcessTopStat();
    void on_processTopStat_closed(int reason);
    void processTopStatUpdateText();
    void on_processTopStat_rowsInserted(const QModelIndex, int, int);

    void ShowContextMenu_processList_tableview(const QPoint& pos);

    void on_processTopStatTimeout_spinbox_valueChanged(int value);
    void on_processDumpsysStatTimeout_spinbox_valueChanged(int value);

    void on_processDumpsysStatClear_toolbutton_clicked();
    void on_processDumpsysStatStartStop_pushbutton_clicked();
    void clearProcessDumpsysStat();
    void on_dumpsysProcTimer();
    void on_dumpsysProcStopped();
    void on_dumpsysProcStarted();

    void on_RewriteFullFileSignal(QString strPath, QString appendString);

    void on_adbPath_changed();

};




#endif // ADBVIEWER_H
