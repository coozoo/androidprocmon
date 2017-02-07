/* Author: Yuriy Kuzin
 */
#include "adbviewer.h"


adbViewer::adbViewer(QWidget *parent) : QWidget(parent)
{
    QTextStream cout(stdout);
    //some stuf to handle out different stupid thisng :)
    totalCPUcolumnCounter=0;
    totalCPUBuffer="";
    processDumpsysAndroidVersion="";
    adbBinary=whereAdbExists();

    //toolbox to place each section inside and layout for it
    main_gridlayout=new QGridLayout();
    main_toolbox=new QToolBox();

    //logacat section
    //checkbox to start or stop logcat
    logcatEnable_checkbox=new QCheckBox();
    logcatEnable_checkbox->setText(tr("Enable"));
    logcatEnable_checkbox->setToolTip(tr("Start/Stop logcat"));
    //show hide filter selection
    showFilterSection_checkbox=new QCheckBox();
    showFilterSection_checkbox->setText(tr("Hide filter section"));
    showFilterSection_checkbox->setToolTip(tr("Hide filter section"));
    //results of logcat
    logcatAllResultADB_plaintextedit=new QPlainTextEdit();
    //results of filtered logcat
    logcatFiltered_plaintextedit=new QPlainTextEdit();
    logcatFiltered_plaintextedit->setVisible(true);
    top_spacer=new QSpacerItem(10, 10, QSizePolicy::Maximum, QSizePolicy::Minimum);
    regexp_spacer=new QSpacerItem(10, 10, QSizePolicy::Maximum, QSizePolicy::Minimum);
    //chekcbox to enable filtering
    enableFilter_checkbox=new QCheckBox();
    enableFilter_checkbox->setToolTip(tr("Start match monitoring"));
    enableFilter_checkbox->setText(tr("Enable"));
    //combobox with filters (regexp or simple)
    filterCondition_combobox=new QComboBox();
    filterCondition_combobox->setAutoCompletion(true);
    filterCondition_combobox->setDuplicatesEnabled(false);
    filterCondition_combobox->setEditable(true);
    filterCondition_combobox->setInsertPolicy(QComboBox::NoInsert);
    //fill out combobox with filters from file
    QString filtersList=ReadFullFile("filters_list");
    foreach(QString string,filtersList.split("\n"))
    {
        if(string!="")
        {
            filterCondition_combobox->addItem(string.split(",")[0],QVariant(string.split(",")[1]));
        }
    }
    //button to save current filter
    rememberFilter_toolbutton=new QToolButton();
    rememberFilter_toolbutton->setToolTip(tr("Save filter"));
    rememberFilter_toolbutton->setIcon(QIcon(QPixmap(":/images/disc-floopy.png")));
    //button to delete current filter
    deleteFilter_toolbutton=new QToolButton();
    deleteFilter_toolbutton->setToolTip(tr("Delete current filter from saved"));
    deleteFilter_toolbutton->setIcon(QIcon(QPixmap(":/images/remove.png")));
    //shows if condition inside is regexp
    useRegexp_checkbox=new QCheckBox();
    useRegexp_checkbox->setChecked(true);
    useRegexp_checkbox->setText(tr("RegExp"));
    useRegexp_checkbox->setToolTip(tr("Use regexp"));

    //processlist section
    //lineedit to show errors
    processListStatus_lineedit=new QLineEdit();
    processListStatus_lineedit->setVisible(false);
    //table to store proceses returned by top
    processList_tableview=new QTableView();
    processList_tableview->verticalHeader()->setDefaultSectionSize(20);
    processList_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    processList_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processList_tableview->setSortingEnabled(true);
    processList_model=new QStandardItemModel(0,0,this);
    processList_tableview->setModel(processList_model);

    processList_tableview->horizontalHeader()->setSectionsMovable(true);
    processList_tableview->horizontalHeader()->setDragEnabled(true);
    processList_tableview->horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    processList_tableview->setContextMenuPolicy(Qt::CustomContextMenu);
    //button to get list of processes
    refreshProcessList_pushbutton=new QPushButton();
    refreshProcessList_pushbutton->setText(tr("Refresh"));

    //process Top section
    //clear table with logged top data
    processTopStatClear_toolbutton=new QToolButton();
    processTopStatClear_toolbutton->setToolTip(tr("Clear"));
    processTopStatClear_toolbutton->setIcon(QIcon(QPixmap(":/images/remove.png")));
    //start stop top to perform monitoring of memory and cpu
    processTopStatStartStop_pushbutton=new QPushButton();
    processTopStatStartStop_pushbutton->setText(tr("Start"));
    //process name to monitor
    processTopStatFilter_lineedit=new QLineEdit();
    processTopStatFilter_lineedit->setPlaceholderText(tr("Enter part of app name"));
    //checkbox to enable disable csv file writing
    processTopStatEnableLog_checkbox=new QCheckBox();
    processTopStatEnableLog_checkbox->setChecked(true);
    processTopStatEnableLog_checkbox->setText(tr("Log"));
    processTopStatEnableLog_checkbox->setToolTip(tr("Enable/disable writing to file"));
    //table for data captured from top
    processTopStat_tableview=new QTableView();
    processTopStat_tableview->verticalHeader()->setDefaultSectionSize(20);
    processTopStat_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    processTopStat_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processTopStat_tableview->setSortingEnabled(true);
    processTopStat_model=new QStandardItemModel(0,0,this);
    processTopStat_tableview->setModel(processTopStat_model);
    //lineedit to show top errors
    processTopStatStatus_lineedit=new QLineEdit();
    processTopStatStatus_lineedit->setVisible(false);
    //spinbox to set timeout how often data returned default 5
    processTopStatTimeout_spinbox=new QSpinBox();
    processTopStatTimeout_spinbox->setMinimum(2);
    processTopStatTimeout_spinbox->setValue(5);
    processTopStatTimeout_spinbox->setToolTip(tr("Timeout"));
    //perform some actions when timeout is changed
    connect(processTopStatTimeout_spinbox,SIGNAL(valueChanged(int)),this,SLOT(on_processTopStatTimeout_spinbox_valueChanged(int)));
    //make top table little bit more user friendly :)
    processTopStat_tableview->horizontalHeader()->setSectionsMovable(true);
    processTopStat_tableview->horizontalHeader()->setDragEnabled(true);
    processTopStat_tableview->horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);


    //dumpsys section
    //clear table with dumpsys logged data
    processDumpsysStatClear_toolbutton=new QToolButton();
    processDumpsysStatClear_toolbutton->setToolTip(tr("Clear"));
    processDumpsysStatClear_toolbutton->setIcon(QIcon(QPixmap(":/images/remove.png")));
    //start stop dumpsys logging
    processDumpsysStatStartStop_pushbutton=new QPushButton();
    processDumpsysStatStartStop_pushbutton->setText(tr("Start"));
    //field for application name to monitor
    processDumpsysStatFilter_lineedit=new QLineEdit();
    processDumpsysStatFilter_lineedit->setPlaceholderText(tr("Enter app name"));
    //checkbox to enable disable csv file logging
    processDumpsysStatEnableLog_checkbox=new QCheckBox();
    processDumpsysStatEnableLog_checkbox->setChecked(true);
    processDumpsysStatEnableLog_checkbox->setText(tr("Log"));
    processDumpsysStatEnableLog_checkbox->setToolTip(tr("Enable/disable writing to file"));
    //table for dumpsyss data
    processDumpsysStat_tableview=new QTableView();
    processDumpsysStat_tableview->verticalHeader()->setDefaultSectionSize(20);
    processDumpsysStat_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    processDumpsysStat_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processDumpsysStat_tableview->setSortingEnabled(true);
    processDumpsysStat_model=new QStandardItemModel(0,0,this);
    processDumpsysStat_tableview->setModel(processDumpsysStat_model);
    //lineedit to show errors of dumpsys section
    processDumpsysStatStatus_lineedit=new QLineEdit();
    processDumpsysStatStatus_lineedit->setVisible(false);
    //spinbox to set timeout how often data returned default 5
    processDumpsysStatTimeout_spinbox=new QSpinBox();
    processDumpsysStatTimeout_spinbox->setMinimum(2);
    processDumpsysStatTimeout_spinbox->setValue(5);
    processDumpsysStatTimeout_spinbox->setToolTip(tr("Timeout"));
    connect(processDumpsysStatTimeout_spinbox,SIGNAL(valueChanged(int)),this,SLOT(on_processDumpsysStatTimeout_spinbox_valueChanged(int)));
    //make dumpsys table more user friendly
    processDumpsysStat_tableview->horizontalHeader()->setSectionsMovable(true);
    processDumpsysStat_tableview->horizontalHeader()->setDragEnabled(true);
    processDumpsysStat_tableview->horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);


    //layout for device list and refresh button
    toolbar_hboxlayout=new QHBoxLayout();
    //combobox for devices returned by adb
    devicesList_combobox=new QComboBox();
    //refresh button to get sevices from adb
    refreshDevices_toolbutton=new QToolButton;
    refreshDevices_toolbutton->setToolTip(tr("Refresh Devices (All activities will be stopped)"));
    refreshDevices_toolbutton->setText(tr("Refresh"));
    //place devices combobox and refresh button to layout
    toolbar_hboxlayout->addWidget(devicesList_combobox);
    toolbar_hboxlayout->addWidget(refreshDevices_toolbutton);
    //add it to main layout
    main_gridlayout->addLayout(toolbar_hboxlayout,0,0);
    //and add toolbox to main layout
    main_gridlayout->addWidget(main_toolbox,1,0);


    //logcat section composing
    QGridLayout *logcat_gridlayout=new QGridLayout();
    topLogcat_gridlayout=new QGridLayout();
    regexpLogcat_gridlayout=new QGridLayout();
    QGroupBox *logcat_groupbox=new QGroupBox();
    logcat_groupbox->setLayout(logcat_gridlayout);
    topLogcat_gridlayout->addWidget(logcatEnable_checkbox,0,0);
    topLogcat_gridlayout->addWidget(showFilterSection_checkbox,0,1);
    topLogcat_gridlayout->addItem(top_spacer,0,2,1,2);
    logcat_gridlayout->addLayout(topLogcat_gridlayout,0,0);
    logcat_gridlayout->addWidget(logcatAllResultADB_plaintextedit,1,0);
    regexpLogcat_gridlayout->addWidget(enableFilter_checkbox,0,0);
    regexpLogcat_gridlayout->addWidget(filterCondition_combobox,0,1,1,2);
    regexpLogcat_gridlayout->addWidget(rememberFilter_toolbutton,0,3);
    regexpLogcat_gridlayout->addWidget(deleteFilter_toolbutton,0,4);
    regexpLogcat_gridlayout->addWidget(useRegexp_checkbox,0,5);
    logcat_gridlayout->addLayout(regexpLogcat_gridlayout,2,0);
    logcat_gridlayout->addItem(regexp_spacer,2,4);
    logcat_gridlayout->addWidget(logcatFiltered_plaintextedit,3,0);

    //list of process section composing
    QGridLayout *processlist_gridlayout=new QGridLayout();
    QGroupBox *processlist_groupbox=new QGroupBox();
    processlist_groupbox->setLayout(processlist_gridlayout);
    processlist_gridlayout->addWidget(refreshProcessList_pushbutton,0,0);
    processlist_gridlayout->addWidget(processList_tableview,1,0);
    processlist_gridlayout->addWidget(processListStatus_lineedit,2,0);

    //top results section composing
    QGridLayout *processTopStat_gridlayout=new QGridLayout();
    QGroupBox *processTopStat_groupbox=new QGroupBox();
    processTopStat_groupbox->setLayout(processTopStat_gridlayout);
    processTopStat_gridlayout->addWidget(processTopStatFilter_lineedit,0,0);
    processTopStat_gridlayout->addWidget(processTopStatTimeout_spinbox,0,1);
    processTopStat_gridlayout->addWidget(processTopStatStartStop_pushbutton,0,2);
    processTopStat_gridlayout->addWidget(processTopStatEnableLog_checkbox,0,3);
    processTopStat_gridlayout->addWidget(processTopStatClear_toolbutton,0,4);
    processTopStat_gridlayout->addWidget(processTopStat_tableview,1,0,1,5);
    processTopStat_gridlayout->addWidget(processTopStatStatus_lineedit,2,0,1,5);

    //dumpsys presults section composing
    QGridLayout *processDumpsysStat_gridlayout=new QGridLayout();
    QGroupBox *processDumpsysStat_groupbox=new QGroupBox();
    processDumpsysStat_groupbox->setLayout(processDumpsysStat_gridlayout);
    processDumpsysStat_gridlayout->addWidget(processDumpsysStatFilter_lineedit,0,0);
    processDumpsysStat_gridlayout->addWidget(processDumpsysStatTimeout_spinbox,0,1);
    processDumpsysStat_gridlayout->addWidget(processDumpsysStatStartStop_pushbutton,0,2);
    processDumpsysStat_gridlayout->addWidget(processDumpsysStatEnableLog_checkbox,0,3);
    processDumpsysStat_gridlayout->addWidget(processDumpsysStatClear_toolbutton,0,4);
    processDumpsysStat_gridlayout->addWidget(processDumpsysStat_tableview,1,0,1,5);
    processDumpsysStat_gridlayout->addWidget(processDumpsysStatStatus_lineedit,2,0,1,5);



    QMainWindow *execute_mainwindow=new QMainWindow(this);
    execute_mainwindow->setAnimated(true);
    execute_mainwindow->setDockNestingEnabled(true);
    execute_mainwindow->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    execute_mainwindow->setMinimumHeight(200);
    execute_mainwindow->setMinimumWidth(150);
    execute_dock=new adbExecute(execute_mainwindow);
    execute_dock->setadbBinary(getadbBinary());
    connect(this,SIGNAL(androidDeviceChanged(QString)),execute_dock,SLOT(on_androidDeviceChanged(QString)));


    //place all composed sections onto tollboxes
    //0
    main_toolbox->addItem(logcat_groupbox,tr("Logcat"));
    main_toolbox->setItemIcon(0,QIcon(QPixmap(":/images/notepad.png")));
    //1
    main_toolbox->addItem(execute_mainwindow,tr("Execute"));
    main_toolbox->setItemIcon(1,QIcon(QPixmap(":/images/execute.png")));
    //2
    main_toolbox->addItem(processlist_groupbox,tr("Process List"));
    main_toolbox->setItemIcon(2,QIcon(QPixmap(":/images/tree.png")));
    //3
    main_toolbox->addItem(processTopStat_groupbox,tr("Top Process Stat Log"));
    main_toolbox->setItemIcon(3,QIcon(QPixmap(":/images/right.png")));
    //4
    main_toolbox->addItem(processDumpsysStat_groupbox,tr("Dumpsys Proc meminfo"));
    main_toolbox->setItemIcon(4,QIcon(QPixmap(":/images/right.png")));
    //5
    //main_toolbox->addItem(execute_mainwindow,"test");
    //main_toolbox->setItemIcon(5,QIcon(QPixmap(":/images/execute.png")));




    parent->setLayout(main_gridlayout);

    //init three process objects
    //logcat
    proc=new QProcess();

    //top process
    processTopStatExec=new QProcess();

    /* below a a lot connections to make me mad
     * but with them it works :)
     * and you will find just a few more lines without connect */

    //change refresh devices connections
    connect(refreshDevices_toolbutton,SIGNAL(clicked()),this,SLOT(on_refreshDevices_toolbutton_clicked()));
    connect(devicesList_combobox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_devicesList_combobox_currentIndexChanged(int)));
    //on logcat stopped and started
    connect(proc,SIGNAL(finished(int)),this,SLOT(on_proc_closed(int)));
    connect(proc,SIGNAL(started()),this,SLOT(on_proc_started()));

    //update list of devices after app start
    on_refreshDevices_toolbutton_clicked();

    //not using anymore
    //connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(updateError()));
    //connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(updateText()));

    //on logcat received text
    connect(proc,SIGNAL(readyRead()),this,SLOT(updateText()));

    //refresh process list button connect
    connect(refreshProcessList_pushbutton,SIGNAL(clicked()),this,SLOT(getProcesslist()));
    //right click and menu handler for process list table
    connect(processList_tableview, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(ShowContextMenu_processList_tableview(const QPoint&)));

    //clear top table button clicked
    connect(processTopStatClear_toolbutton,SIGNAL(clicked()),this,SLOT(on_processTopStatClear_toolbutton_clicked()));
    //stopstart monitor button clicked
    connect(processTopStatStartStop_pushbutton,SIGNAL(clicked()),this,SLOT(on_processTopStatStartStop_pushbutton_clicked()));
    //read top output
    connect(processTopStatExec,SIGNAL(readyReadStandardOutput()),this,SLOT(processTopStatUpdateText()));
    //if top closed
    connect(processTopStatExec,SIGNAL(finished(int)),this,SLOT(on_processTopStat_closed(int)));
    //if row inserted into top table
    connect(processTopStat_model,SIGNAL(rowsInserted(QModelIndex,int,int)),this,SLOT(on_processTopStat_rowsInserted(QModelIndex,int,int)));

    //clear dumpsys button clicked
    connect(processDumpsysStatClear_toolbutton,SIGNAL(clicked()),this,SLOT(on_processDumpsysStatClear_toolbutton_clicked()));
    //start stop dumpsys monitor
    connect(processDumpsysStatStartStop_pushbutton,SIGNAL(clicked()),this,SLOT(on_processDumpsysStatStartStop_pushbutton_clicked()));
    //connect emitted internaly signal when timer stopped as there is no internal signal for this
    connect(this,SIGNAL(dumpsysProcStarted()),this,SLOT(on_dumpsysProcStarted()));
    connect(this,SIGNAL(dumpsysProcStopped()),this,SLOT(on_dumpsysProcStopped()));
    //call when timer timeout
    connect(&dumpsysProcTimer, SIGNAL(timeout()), this, SLOT(on_dumpsysProcTimer()));


    //enable logcat
    connect(logcatEnable_checkbox,SIGNAL(clicked()),this,SLOT(on_logcatEnable_checkbox_clicked()));
    //show hide filter section
    connect(showFilterSection_checkbox,SIGNAL(clicked()),this,SLOT(on_showFilterSection_checkbox_clicked()));
    //on save filter action
    connect(rememberFilter_toolbutton,SIGNAL(clicked()),this,SLOT(on_rememberFilter_toolbutton_clicked()));
    //catch enter and connect it on save filter action
    connect(filterCondition_combobox->lineEdit(),SIGNAL(returnPressed()),this,SLOT(on_rememberFilter_toolbutton_clicked()));
    //on delete filter action
    connect(deleteFilter_toolbutton,SIGNAL(clicked()),this,SLOT(on_deleteFilter_toolbutton_clicked()));
    //filter changed
    connect(filterCondition_combobox,SIGNAL(currentIndexChanged(int)),this,SLOT(on_filterCondition_combobox_currentIndexChanged(int)));

    //emitted siganl to override file
    connect(this,SIGNAL(RewriteFullFileSignal(QString,QString)),this,SLOT(on_RewriteFullFileSignal(QString,QString)));

}

/*everything should be killed hope I didn't miss something*/
adbViewer::~adbViewer()
{
    logcatFiltered_plaintextedit->deleteLater();
    if(proc->processId()>0)
        {
            proc->close();
            proc->deleteLater();
        }
}

/*connect top spinbox timeout to dumpsys*/
void adbViewer::on_processTopStatTimeout_spinbox_valueChanged(int value)
{
    processDumpsysStatTimeout_spinbox->setValue(value);
}

/*connect dumpsys spinbox timeout to top*/
void adbViewer::on_processDumpsysStatTimeout_spinbox_valueChanged(int value)
{
    processTopStatTimeout_spinbox->setValue(value);
}

/*Slot if logcat checkbox marked call start function if no - close process*/
void adbViewer::on_logcatEnable_checkbox_clicked()
{
    if(logcatEnable_checkbox->isChecked())
        {
            startlogcat();
        }
    else
        {
            proc->close();
        }

}
/* Slot on devices list current index changed
 * close commandline process
 * close logcat process
 * and restart logcat process if checkbox activted
 */
void adbViewer::on_devicesList_combobox_currentIndexChanged(int)
{

    if(proc->isOpen())
        {
            proc->close();
        }
    if(logcatEnable_checkbox->isChecked())
        {
            startlogcat();
        }
    setandroidDevice(this->devicesList_combobox->currentText());

}

/* Slot on logcat process closed */
void adbViewer::on_proc_closed(int reason)
{
    QTextStream cout(stdout);
    cout<<reason<<endl;
    if(proc->isOpen())
        {
            proc->close();
        }
    logcatFiltered_plaintextedit->appendPlainText("####### Adb closed " + QString::number(reason) + " #######");
    logcatAllResultADB_plaintextedit->appendPlainText("####### Adb closed " + QString::number(reason) + " #######");
    logcatEnable_checkbox->setChecked(false);
}

/* Slot on logcat started */
void adbViewer::on_proc_started()
{
    if(devicesList_combobox->count()>0 && devicesList_combobox->currentText()!="")
        {
            logcatFiltered_plaintextedit->appendPlainText("####### New connection established to: " + devicesList_combobox->currentText() + " #######");
            logcatAllResultADB_plaintextedit->appendPlainText("####### New connection established to: " + devicesList_combobox->currentText() + " #######");
        }
}

/* simply function to start logcat  */
void adbViewer::startlogcat()
{
//later i will add this possibility
//    The priority is one of the following character values, ordered from lowest to highest priority:
//    V — Verbose (lowest priority)
//    D — Debug
//    I — Info (default priority)
//    W — Warning
//    E — Error
//    F — Fatal
//    S — Silent (highest priority, on which nothing is ever printed)
    if(proc->processId()==0)
        {

            QTextStream cout(stdout);
            cout<<adbBinary + " -s " + devicesList_combobox->currentText() + " logcat"<<endl;
            proc->start(adbBinary + " -s " + devicesList_combobox->currentText() + " logcat");
        }
}

/* Slot on refresh devices button clicked
 * close execute process
 * clear combobox and run adb devices
 * capture devices using regexp
 * dumpsys and top monitors will be stopped too
 */
void adbViewer::on_refreshDevices_toolbutton_clicked()
{
    devicesList_combobox->clear();
    QString result;
    result=executeADBCommand("devices");
    QTextStream cout(stdout);
    cout<<result<<endl;

    QRegularExpression reg("(.+)device");

    QRegularExpressionMatchIterator i = reg.globalMatch(result);
    if (i.isValid())
        {
            while (i.hasNext())
                {
                    QRegularExpressionMatch match = i.next();
                    if(match.captured(1).contains(" ")==0)
                        {
                            QTextStream cout(stdout);
                            cout<<match.captured(1).trimmed()<<endl;
                            devicesList_combobox->addItem(match.captured(1).trimmed());
                        }
                }

        }
    setandroidDevice(devicesList_combobox->currentText());
    //clearProcesslist();
    on_processDumpsysStatStartStop_pushbutton_clicked();
    on_processTopStatStartStop_pushbutton_clicked();

}

/* function useful to execute small adb commands in short time
 * execute any adb command and wait for result to return it
 * (maybe it's the reason of small glitching during dumpsys I will try to replace it later)
 */
QString adbViewer::executeADBCommand(QString adbCommand)
{
    QString result;
    QProcess adbexec;
    adbexec.start(adbBinary + " " + adbCommand);
    if(adbexec.waitForStarted())
        {
            adbexec.waitForFinished();
            result=adbexec.readAllStandardOutput();
            if(result=="")
            {result=adbexec.readAll();}
        }
    adbexec.close();
    return result;
}

//not connected slot anymore
void adbViewer::updateError()
{
    QString appendText(proc->readAll());
    //if(appendText.contains("I/GCM-MESSAGE-PAYLOD")){
    if(appendText!="")
        {
            logcatFiltered_plaintextedit->appendPlainText(appendText);
        }
    //}
}
/* function to capture logcat output and handle filters
 * maybe it can be extended later with highlighting
 */
void adbViewer::updateText()
{
    //QString appendText(proc->readAll());
    //testReadallADB->appendPlainText("**************\t"+QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss")+"\t**************");
    QString appendText="";
    QString htmlAppendText="";
    QString simpleMatches="";
    //just to minimize conditios inside while
    bool simpleMatchesActive=(enableFilter_checkbox->isChecked() && !useRegexp_checkbox->isChecked() && filterCondition_combobox->currentText()!="");
    while(proc->canReadLine()){
           //qDebug() << proc->readLine();
        QString line=proc->readLine().trimmed();
           //if(line.replace(QRegExp("\\s"),"")!="")
        if(line!="")
           {

            appendText=appendText+line+"\n";
            if(simpleMatchesActive && line.contains(filterCondition_combobox->currentText()))
            {
                simpleMatches=simpleMatches+"<html><font color=\"orange\">"+line+"</font></html>"+"<br>";
            }
           }
      }
    //testReadallADB->appendPlainText("**************\t"+QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss")+"\t**************");

    logcatAllResultADB_plaintextedit->appendPlainText(appendText);
    if(simpleMatchesActive && simpleMatches!="")
    {
        logcatFiltered_plaintextedit->appendHtml(simpleMatches);
    }

    //maybe later I will add some color posibility
//    if(QRegularExpression("(.+[0-9]+\\w E.+)").globalMatch(line).isValid())
//    {
//        htmlAppendText=htmlAppendText+"<html><font color=\"red\">"+line+"</font></html>"+"<br>";

//    }
//    logcatAllResultADB_plaintextedit->appendHtml(htmlAppendText);

    if(enableFilter_checkbox->isChecked() && useRegexp_checkbox->isChecked() && filterCondition_combobox->currentText()!="" && appendText!="")
        {

                    QTextStream cout(stdout);
                    cout<<appendText<<endl;
                    //QRegularExpression reg("(.+GCM-MESSAGE-PAYLOD.+}])");
                    QRegularExpression reg(filterCondition_combobox->currentText());

                    QRegularExpressionMatchIterator i = reg.globalMatch(appendText);
                    if (i.isValid())
                        {
                            while (i.hasNext())
                                {
                                    QRegularExpressionMatch match = i.next();
                                    logcatFiltered_plaintextedit->appendHtml("<html><font color=\"orange\">"+match.captured(1).trimmed()+"</font></html>");
                                    emit regexpMessageReceived(match.captured(1).right(match.captured(1).size()-match.captured(1).indexOf("bundle",0,Qt::CaseInsensitive)-6).trimmed());
                                }
                        }
//                    else
//                        {
//                            logcatFiltered_plaintextedit->appendHtml("<html><font color=\"blue\">"+appendText+"</font></html>");
//                        }

        }
}


/* Function to get process list by executing top command
 * maybe you will find this parsing dumb (so you'll be right)
 */
void adbViewer::getProcesslist()
{
    clearProcesslist();
    //run top one time with delay one second
    QString command="-s " + devicesList_combobox->currentText() + " shell top -n 1 -d 1";
    QString result;
    result=executeADBCommand(command);
    QStringList allprocesses;
    allprocesses.append(result.split("\n"));
    //qDebug()<<allprocesses;

    //stupid handler of old android stupid behaviour
    //pcyColID to handling old top empty policy
    //(column in some strings simply empty so you've got less columns then header)
    int pcyColID=-1;
    int rowj=0;
    int columnHeaderCount=0;
    foreach (const QString &str, allprocesses)
        {
            QString destr=str;
            destr.replace("\r","").replace("\n","");
            //qDebug() << QString("%1").arg(destr);
            //stupid way to remove all spaces
            while(destr.contains("  "))
                {
                    destr.replace(QString("  "),QString(" "));
                }
            //and remove one space if it's left at the beggining of string
            if(destr.left(1)==" ")
                {
                    destr=destr.right(destr.length()-1);
                }

            //qDebug() << destr.split(" ");
            //now we have only one space between each column
            QStringList valuesList=destr.split(" ");
            if(valuesList.count()>=1)
                {
                    //first string supposed to be pid (int value)
                    //if it's true than we got column with data
                    //it's no 100% way but at least good to check before start
                    bool ok;
                    int first = valuesList[0].toInt(&ok);
                    Q_UNUSED(first);
                    //and we need don't miss a header which start from PID word
                    if (ok || valuesList[0].toLower()=="pid")
                        {
                            qDebug() << destr.split(" ");
                            int coli=0;
                            //for each column of one process
                            foreach (const QString &col, valuesList)
                                {
                                    if(valuesList[0].toLower()=="pid")
                                        {
                                            //if it's header than add headers to tableview
                                            columnHeaderCount=valuesList.count()-1;
                                            processList_model->setHorizontalHeaderItem(coli, new QStandardItem(QString(col)));
                                            //find and remember pcy column ID we will need it later
                                            if(col.toLower()=="pcy")
                                            {
                                                pcyColID=coli;
                                            }
                                        }
                                    else
                                        {
                                            //if it's process data row
                                            QString columnvalue=col;
                                            //qDebug()<<columnvalue.left(columnvalue.length()-1)<<columnvalue.right(1);

                                            /* top returns data in kB I'm not sure maybe possible cases in MB and GB
                                             * so if the last charachter in column is K simply remove it
                                             * if M and G multiply them accordingly
                                             * */
                                            if(columnvalue.right(1)=="K" || columnvalue.right(1)=="M" || columnvalue.right(1)=="G")
                                                {
                                                    bool leftOK;
                                                    double leftvalue = columnvalue.left(columnvalue.length()-1).toInt(&leftOK);
                                                    if(leftOK && leftvalue!=0)
                                                        {
                                                        if(columnvalue.right(1)=="K")
                                                            {
                                                                columnvalue.replace("K","");
                                                            }
                                                        else if(columnvalue.right(1)=="M")
                                                            {
                                                                columnvalue=QString::number(leftvalue*1024);
                                                            }
                                                        else if(columnvalue.right(1)=="G")
                                                            {
                                                                columnvalue=QString::number(leftvalue*1024*1024);
                                                            }

                                                        }
                                                    //sure 0 it's a special case when we need simply to remove letter
                                                    else if(leftOK && leftvalue==0)
                                                        {
                                                            if(columnvalue.right(1)=="K")
                                                                {
                                                                    columnvalue.replace("K","");
                                                                }
                                                            else if(columnvalue.right(1)=="M")
                                                                {
                                                                    columnvalue.replace("M","");
                                                                }
                                                            else if(columnvalue.right(1)=="G")
                                                                {
                                                                    columnvalue.replace("G","");
                                                                }
                                                        }

                                                }
                                            // another stupid thing if we got more columns with data then actuall in header
                                            // yes it's possible because some process names contain a space...
                                            if(coli>columnHeaderCount)
                                                {
                                                    // some weird minuses and we are appending current value to previous column with adding space
                                                    QString prevvalue=processList_model->data(processList_model->index(rowj-1,coli-(coli-columnHeaderCount))).toString();
                                                    processList_model->setItem(rowj-1,coli-(coli-columnHeaderCount),new QStandardItem(QString(prevvalue + " " + columnvalue)));
                                                }
                                            else
                                                {
                                                // yes here it is pcy column id we are expecting to get bg or fg calues
                                                // if it's something else then replace it with - and go to the next column of table
                                                if(pcyColID!=-1 && coli==pcyColID && (columnvalue!="bg" && columnvalue!="fg" ))
                                                {

                                                   processList_model->setItem(rowj-1,coli,new QStandardItem(QString("-")));
                                                   coli++;
                                                }
                                                    // insert data  into table accordingly to type as number or string
                                                    bool isnumber;
                                                    int columnvaluenumber = columnvalue.toInt(&isnumber);
                                                    if(isnumber)
                                                        {
                                                            QStandardItem *numberitem = new QStandardItem();
                                                            numberitem->setData(QVariant(columnvaluenumber), Qt::DisplayRole);
                                                            processList_model->setItem(rowj-1,coli,numberitem);
                                                        }
                                                    else
                                                        {
                                                            processList_model->setItem(rowj-1,coli,new QStandardItem(QString(columnvalue)));
                                                        }

                                                }


                                        }

                                    coli++;
                                }

                            rowj++;
                        }
                }

        }
    processList_tableview->resizeColumnsToContents();
}

/* not connected slots for errors from top
 * and maybe later i will delete them
 * getProcesslist_error()
 * getProcesslist_standard()
 * on_topProcessExec_closed(int reason)
 */
void adbViewer::getProcesslist_error()
{
    QProcess *topProcessExec = (QProcess *)sender();
    QString appendText(topProcessExec->readAll());
    QTextStream cout(stdout);
    cout<<appendText<<endl;
    processListStatus_lineedit->setText("Error: " + appendText + " " + topProcessExec->errorString());
    processListStatus_lineedit->setVisible(true);
    if(topProcessExec->isOpen())
        {
            topProcessExec->close();
        }
    if(topProcessExec->processId()>0)
        {
            topProcessExec->deleteLater();
        }
    refreshProcessList_pushbutton->setDisabled(false);
}

void adbViewer::getProcesslist_standard()
{
    QProcess *topProcessExec = (QProcess *)sender();
    QString appendText(topProcessExec->readAll());
    QTextStream cout(stdout);
    cout<<appendText<<endl;
    if(topProcessExec->isOpen())
        {
            topProcessExec->close();
        }
    if(topProcessExec->processId()>0)
        {
            topProcessExec->deleteLater();
        }
    refreshProcessList_pushbutton->setDisabled(false);
}

void adbViewer::on_topProcessExec_closed(int reason)
{
    QProcess *topProcessExec = (QProcess *)sender();
    QTextStream cout(stdout);
    cout<<reason<<endl;
    processListStatus_lineedit->setText(processListStatus_lineedit->text() + " Adb closed " + QString::number(reason));
    processListStatus_lineedit->setVisible(true);
    if(topProcessExec->isOpen())
        {
            topProcessExec->close();
        }
    if(topProcessExec->processId()>0)
        {
            topProcessExec->deleteLater();
        }
    refreshProcessList_pushbutton->setDisabled(false);
}

/* function to clear process list table
*/
void adbViewer::clearProcesslist()
{

    processList_tableview->clearSelection();
    processList_model->setRowCount(0);
    processList_model->setColumnCount(0);

}

/* function that launching top process with delay
 */
void adbViewer::startProcessTopStat()
{

    totalCPUcolumnCounter=0;
    totalCPUBuffer="";
    if(processTopStatExec->processId()==0)
        {
            if(processTopStatFilter_lineedit->text()!="")
                {
                    //get headers
                    QString headercommand="-s " + devicesList_combobox->currentText() + " shell top -n 1 -d 1|grep -i pid|grep -i cpu";
                    QTextStream cout(stdout);
                    cout<<"headercommand: "<<headercommand<<endl;
                    QString result;
                    result=executeADBCommand(headercommand);
                    //QStringList allprocesses;
                    //allprocesses.append(result.split("\n"));
                    result=result.trimmed();
                    while(result.contains("  "))
                        {
                            result.replace(QString("  "),QString(" "));
                        }
                    //get system data and "headers" actually all of this in one row
                    QString totalcpucommand="-s " + devicesList_combobox->currentText() + " shell top -n 1 -d 1|grep User|grep System";
                    cout<<"totalcpucommand: "<<totalcpucommand<<endl;
                    // remove all multiple spaces
                    QString resulttotalcpu;
                    resulttotalcpu=executeADBCommand(totalcpucommand);
                    //QStringList allprocesses;
                    //allprocesses.append(result.split("\n"));
                    resulttotalcpu=resulttotalcpu.trimmed();
                    while(resulttotalcpu.contains("  "))
                        {
                            resulttotalcpu.replace(QString("  "),QString(" "));
                        }

                    cout<<result<<endl;
                    cout<<resulttotalcpu<<endl;
                    //make table headers for total CPU system, user...
                    QString resulttotalcpuHeaders;
                    foreach (QString str, resulttotalcpu.split(" "))
                        {
                            if(str!= "" && !str.contains("%"))
                                {
                                    resulttotalcpuHeaders=resulttotalcpuHeaders +"CPU("+str + ") ";
                                    totalCPUcolumnCounter++;
                                }

                        }

                    //add time header
                    result="Time " + resulttotalcpuHeaders + result;
                    // and now add all headers to table view
                    QStringList headersList=result.split(" ");
                    if(headersList.count()>=1)
                        {
                            int coli=0;
                            foreach (const QString &col, headersList)
                                {
                                    processTopStat_model->setHorizontalHeaderItem(coli, new QStandardItem(QString(col)));
                                    coli++;
                                }
                        }
                    processTopStat_tableview->resizeColumnsToContents();
                    //actually here is the start top with delay parameter
                    //tried a lot of devices looks like this way should work
                    //to get system and process data
                    QString command=adbBinary + " -s " + devicesList_combobox->currentText() + " shell top -d " + processTopStatTimeout_spinbox->text() + " | grep -E '" + processTopStatFilter_lineedit->text() +"$|User.*System'";
                    cout<<command<<endl;
                    processTopStatExec->start(command);
                    processTopStatFilter_lineedit->setEnabled(false);
                    processTopStatTimeout_spinbox->setEnabled(false);
                    processTopStatStartStop_pushbutton->setText(tr("Stop"));
                    main_toolbox->setItemIcon(3,QIcon(QPixmap(":/images/right_green.png")));
                }
        }
}

//slot on clear top table button
void adbViewer::on_processTopStatClear_toolbutton_clicked()
{
    clearProcessTopStat();
}

//slot on start stop top button
void adbViewer::on_processTopStatStartStop_pushbutton_clicked()
{
    if(processTopStatExec->processId()==0)
        {
            startProcessTopStat();
        }
    else
        {
            processTopStatExec->close();
        }
}

//stop and clear top table
void adbViewer::clearProcessTopStat()
{
    if(processTopStatExec->processId()!=0)
        {
            processTopStatExec->close();
        }
    processTopStat_tableview->clearSelection();
    processTopStat_model->setRowCount(0);
    processTopStat_model->setColumnCount(0);
}

//slot on top process closed
void adbViewer::on_processTopStat_closed(int reason)
{
    processTopStatFilter_lineedit->setEnabled(true);
    processTopStatTimeout_spinbox->setEnabled(true);
    processTopStatStatus_lineedit->setText("Adb closed " + QString::number(reason));
    processTopStatStartStop_pushbutton->setText(tr("Start"));
    main_toolbox->setItemIcon(3,QIcon(QPixmap(":/images/right.png")));
}
//slot to handle when new data from top process received
void adbViewer::processTopStatUpdateText()
{
    //timestamp date
    QString dt= QDateTime::currentDateTime().toString("dd/MM/yyyy_hh:mm:ss");
    QString appendText(processTopStatExec->readAll());
    //qDebug()<<appendText;

    //pcyColID to handling old top empty policy
    int pcyColID=getHeaderIDbyNameOfProcessTopStat("PCY");

    if(!appendText.contains(devicesList_combobox->currentText()))
        {
            QString totalCPULoadString="";
            QString appTopLine="";
            //get system CPU
            foreach (QString str, appendText.split("\n"))
                {
                    if(str.contains("System") && str.contains("User"))
                        {
                            totalCPULoadString=str.left(str.indexOf("\n"));
                            QTextStream cout(stdout);
                            cout<<"totalCPULoadString"<<totalCPULoadString<<endl;
                            totalCPULoadString=totalCPULoadString.trimmed();
                        }
                    else if(str.contains(processTopStatFilter_lineedit->text()))
                        {
                            appTopLine=str.left(str.indexOf("\n"));
                        }
                }
            //little bit stupid way
            //I'm pretty sure ot will fail in some cases
            if(totalCPUcolumnCounter>0)
                {
                    QString totalCPUdataline="";
                    if(totalCPULoadString!="")
                        {
                            totalCPULoadString=totalCPULoadString.trimmed();
                            while(totalCPULoadString.contains("  "))
                                {
                                    totalCPULoadString.replace(QString("  "),QString(" "));
                                }

                            foreach (QString str, totalCPULoadString.split(" "))
                                {
                                    if(str.contains("%"))
                                        {
                                            totalCPUdataline=totalCPUdataline+str.replace("%","").replace(",","")+" ";
                                        }
                                }
                            totalCPUBuffer=totalCPUdataline;

                        }
                    else if(totalCPULoadString!="" && totalCPUBuffer=="")
                        {
                            totalCPUBuffer="    ";
                        }
                }

            //appendText=appendText.left(appendText.indexOf("\n"));
            //parse process data
            if(appTopLine!="")
                {
                    appTopLine=appTopLine.trimmed();
                    while(appTopLine.contains("  "))
                        {
                            appTopLine.replace(QString("  "),QString(" "));
                        }



                    appTopLine=dt +" " + totalCPUBuffer + appTopLine;
                    QTextStream cout(stdout);
                    cout<<"appTopLine"<<appTopLine<<endl;

                    QStringList valuesList=appTopLine.split(" ");
                    if(valuesList.count()>=1)
                        {
                            int rowj=processTopStat_model->rowCount();
                            int columncount=processTopStat_model->columnCount()-1;
                            //qDebug()<<columncount<<"_____________________";
                            int coli=0;
                            foreach (const QString &col, valuesList)
                                {
                                    QString columnvalue=col;
                                    if(coli==0)
                                        {
                                            columnvalue.replace("_"," ");
                                        }
                                    //qDebug()<<columnvalue.left(columnvalue.length()-1)<<columnvalue.right(1);
                                    if(columnvalue.right(1)=="K" || columnvalue.right(1)=="M" || columnvalue.right(1)=="G" || columnvalue.right(1)=="%")
                                        {
                                            bool leftOK;
                                            double leftvalue = columnvalue.left(columnvalue.length()-1).toInt(&leftOK);
                                            if(leftOK && leftvalue!=0)
                                                {
                                                if(columnvalue.right(1)=="K")
                                                    {
                                                        columnvalue.replace("K","");
                                                    }
                                                else if(columnvalue.right(1)=="M")
                                                    {
                                                        columnvalue=QString::number(leftvalue*1024);
                                                    }
                                                else if(columnvalue.right(1)=="G")
                                                    {
                                                        columnvalue=QString::number(leftvalue*1024*1024);
                                                    }
                                                    else if(columnvalue.right(1)=="%")
                                                        {
                                                            columnvalue.replace("%","");
                                                        }

                                                }
                                            else if(leftOK && leftvalue==0)
                                                {
                                                    if(columnvalue.right(1)=="K")
                                                        {
                                                            columnvalue.replace("K","");
                                                        }
                                                    else if(columnvalue.right(1)=="M")
                                                        {
                                                            columnvalue.replace("M","");
                                                        }
                                                    else if(columnvalue.right(1)=="G")
                                                        {
                                                            columnvalue.replace("G","");
                                                        }
                                                    else if(columnvalue.right(1)=="%")
                                                        {
                                                            columnvalue.replace("%","");
                                                        }
                                                }

                                        }
                                    if(coli>columncount)
                                        {
                                            QString prevvalue=processTopStat_model->data(processTopStat_model->index(rowj,coli-(coli-columncount))).toString();
                                            processTopStat_model->setItem(rowj,coli-(coli-columncount),new QStandardItem(QString(prevvalue + " " + columnvalue)));
                                        }
                                    else
                                        {
                                        if(pcyColID!=-1 && coli==pcyColID && (columnvalue!="bg" && columnvalue!="fg" ))
                                        {

                                           processTopStat_model->setItem(rowj,coli,new QStandardItem(QString("-")));
                                           coli++;
                                        }
                                            bool isnumber;
                                            int columnvaluenumber = columnvalue.toInt(&isnumber);
                                            if(isnumber)
                                                {
                                                    QStandardItem *numberitem = new QStandardItem();
                                                    numberitem->setData(QVariant(columnvaluenumber), Qt::DisplayRole);
                                                    processTopStat_model->setItem(rowj,coli,numberitem);
                                                }
                                            else
                                                {
                                                    processTopStat_model->setItem(rowj,coli,new QStandardItem(QString(columnvalue)));
                                                }

                                        }




                                    coli++;
                                }

                            //write top process stat into file
                            QString headersList="";
                            QString logLine="";
                            QString pidcur="";
                            QString namecur="";
                            if(processTopStatEnableLog_checkbox->isChecked())
                                {
                                    for(int i = 0; i < processTopStat_model->columnCount(); i++)
                                        {
                                            headersList=headersList+processTopStat_model->headerData(i, Qt::Horizontal).toString() + ",";

                                            if (processTopStat_model->hasIndex(rowj,i))
                                                {
                                                    //qDebug()<<processTopStat_model->index(row ,  i).data();
                                                    logLine=logLine+processTopStat_model->data(processTopStat_model->index(rowj,i)).toString() +",";
                                                }
                                            if(processTopStat_model->headerData(i, Qt::Horizontal).toString().toLower().contains(QString("pid")))
                                                {
                                                    pidcur=processTopStat_model->data(processTopStat_model->index(rowj,i)).toString();
                                                }
                                            if(processTopStat_model->headerData(i, Qt::Horizontal).toString().toLower().contains(QString("name")))
                                                {
                                                    namecur=processTopStat_model->data(processTopStat_model->index(rowj,i)).toString();
                                                }

                                        }
                                    if(headersList!="")
                                        {
                                            headersList=headersList.left(headersList.length()-1);
                                        }
                                    if(logLine!="")
                                        {
                                            logLine=logLine.left(logLine.length()-1);
                                        }
                                    QTextStream cout(stdout);
                                    cout<<headersList<<endl<<logLine<<endl;
                                    writeStatCSV(namecur + "_" + pidcur +"_top",logLine,headersList);
                                }
                            emit processTopStatNewData(headersList,logLine);


                        }
                    processTopStat_tableview->resizeColumnsToContents();
                    //scrolltable on new row if it's bottom position
                    if(processTopStat_tableview->verticalScrollBar()->value()==processTopStat_tableview->verticalScrollBar()->maximum())
                        {
                            processTopStat_tableview->scrollToBottom();
                        }

                }
        }


}

/* function to get header id by name from process list table
 * columnName - case sensetive
 * return: id of column or -1 if not found
 */
int adbViewer::getHeaderIDbyNameOfProcessList(QString columnName)
{
    for(int i = 0; i < processList_model->columnCount(); i++)
        {
            if(processList_model->headerData(i, Qt::Horizontal).toString().toLower().contains(columnName.toLower()))
                {
                    return i;
                }
        }
    return -1;
}

/* function to get header id by name from top monitor table
 * columnName - case sensetive
 * return: id of column or -1 if not found
 */
int adbViewer::getHeaderIDbyNameOfProcessTopStat(QString columnName)
{
    for(int i = 0; i < processTopStat_model->columnCount(); i++)
        {
            if(processTopStat_model->headerData(i, Qt::Horizontal).toString().toLower().contains(columnName.toLower()))
                {
                    return i;
                }
        }
    return -1;
}

/* function to get header id by name from dumpsys monitor table
 * columnName - case sensetive
 * return: id of column or -1 if not found
 */
int adbViewer::getHeaderIDbyNameOfProcessDumpsysStat(QString columnName)
{
    for(int i = 0; i < processDumpsysStat_model->columnCount(); i++)
        {
            if(processDumpsysStat_model->headerData(i, Qt::Horizontal).toString().toLower().contains(columnName.toLower()))
                {
                    return i;
                }
        }
    return -1;
}

/* show custom menu when right click on process list table
 * monitor - will start dumpsys timer and top process to monitor and collect the data
 */
void adbViewer::ShowContextMenu_processList_tableview(const QPoint& pos)
{
    QTableView *tableview = (QTableView *)sender();
    // for most widgets

    QPoint globalPos = tableview->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    QAction *monitor;
    monitor=myMenu.addAction("Monitor Process");
//    QAction *enableLog;
//    enableLog=myMenu.addAction("Enable Log");
//    enableLog->setCheckable(true);
//    if(getenableDiffLog())
//    {
//        enableLog->setChecked(true);
//    }
//    else
//    {
//        enableLog->setChecked(false);
//    }


    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem==monitor)
        {
            if(processTopStatExec->processId()==0)
                {
                    int columnid=getHeaderIDbyNameOfProcessList("name");
                    int rowid = tableview->selectionModel()->currentIndex().row();
                    QTextStream cout(stdout);
                    cout<<tableview->model()->index(rowid , columnid).data().toString()<<endl;
                    processTopStatFilter_lineedit->setText(tableview->model()->index(rowid , columnid).data().toString());
                    processTopStatStartStop_pushbutton->click();
                }
            else
                {
                QTextStream cout(stdout);
                cout<<"another process monitor already started"<<endl;
                }
            if(!dumpsysProcTimer.isActive())
                {
                int columnid=getHeaderIDbyNameOfProcessList("name");
                int rowid = tableview->selectionModel()->currentIndex().row();
                QTextStream cout(stdout);
                cout<<tableview->model()->index(rowid , columnid).data().toString()<<endl;
                processDumpsysStatFilter_lineedit->setText(tableview->model()->index(rowid , columnid).data().toString());
                processDumpsysStatStartStop_pushbutton->click();
            }
            else
            {
                QTextStream cout(stdout);
                cout<<"another dumpsys already launched"<<endl;
            }
        }
//    else if (selectedItem==enableLog)
//    {
//        if(getenableDiffLog())
//        {
//            setenableDiffLog(false);
//            enableLog->setChecked(false);
//        }
//        else
//        {
//            setenableDiffLog(true);
//            enableLog->setChecked(true);
//        }

//    }
}

/* do something when new row added to top table
 */
void adbViewer::on_processTopStat_rowsInserted(const QModelIndex, int , int )
{

}

/* Function to write parsed data into CSV file
 * appendFileName - some part of file name
 * logLine - actually lin with data
 * headersList - headears
 * currently csv can be jammed because dumpsys data can add columns on the fly
 * need to add some handling for that to detect column increasing
 */
void adbViewer::writeStatCSV(QString appendFileName, QString logLine, QString headersList)
{
    QTextStream cout(stdout);
#ifdef Q_OS_WIN
    QString rootstatsdir=qApp->applicationDirPath();
    QString statsdirlocation="/stats/";
#endif
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    QString rootstatsdir=QDir::home().absolutePath();
    QString statsdirlocation="/androidprocmon/stats/";
#endif
    QString filepath=rootstatsdir + statsdirlocation + getstrDateTimeFile() + "/";
    if(!QDir(filepath).exists())
        {
            cout<<"Creating stats dir: "<<filepath<<endl;
            if(QDir().mkpath(filepath))
                {
                    cout<<"Creating succesfull!"<<endl;
                }
            else
                {
                    cout<<"unable to create: " <<filepath<<endl;
                }
        }
    else
        {
            cout<<"Directory alredy exists: "<<filepath<<endl;
        }
    filepath=filepath + appendFileName + ".csv";
    bool exists=false;
    if(!QFile(filepath).exists())
        {
            exists=false;
        }
    else
        {
            exists=true;
        }
    QFile outFile(filepath);
    //read current amount of columns
    //maybe it will be better to use some in memory map  for performance
    //so if some problems will be here i will use QMap as cache for column amount
    if(exists)
        {
    if (!outFile.open(QIODevice::ReadOnly | QIODevice::Text))return;
    int filecolumns=QString(outFile.readLine()).split(",").count();
    outFile.close();
    int inputcolumns=headersList.split(",").count();
    if(inputcolumns>filecolumns)
    {
        int addcommaamount=inputcolumns-filecolumns;
        //call rewrite file
        QFile newFile(filepath+".new");
        if (!(outFile.open(QIODevice::ReadOnly | QIODevice::Text) && newFile.open(QIODevice::WriteOnly | QIODevice::Append)))return;
        {
               QTextStream textdata(&outFile);
               QTextStream textStream(&newFile);
               textStream <<headersList<<endl;
               int linecnt=0;
               while (!textdata.atEnd())
               {
                   if(linecnt>0)
                   {
                    textStream<<textdata.readLine();
                   }
                   else
                   {
                       textdata.readLine();
                   }
                    linecnt++;
               }
               //replace original with new one
               outFile.remove();
               newFile.rename(filepath);

        }

    }
    }
    if(!outFile.open(QIODevice::WriteOnly | QIODevice::Append))return;
    QTextStream textStream(&outFile);
    if(!exists)
        {
            textStream <<headersList<<endl<< logLine<<endl;
        }
    else
        {
            textStream << logLine<<endl;
        }
    outFile.close();
}

// slot on clear dumpsys button clicked
void adbViewer::on_processDumpsysStatClear_toolbutton_clicked()
{
    clearProcessDumpsysStat();
}

//Function to clear dumpsys table
void adbViewer::clearProcessDumpsysStat()
{
    processDumpsysStat_tableview->clearSelection();
    processDumpsysStat_model->setRowCount(0);
    processDumpsysStat_model->setColumnCount(0);
}

/* slot on dumpsys timer stopped */
void adbViewer::on_dumpsysProcStopped()
{
    processDumpsysStatStartStop_pushbutton->setText(tr("Start"));
    main_toolbox->setItemIcon(4,QIcon(QPixmap(":/images/right.png")));
    processDumpsysStatFilter_lineedit->setEnabled(true);
    processDumpsysStatTimeout_spinbox->setEnabled(true);
}

/* slot on dumpsys timer started */
void adbViewer::on_dumpsysProcStarted()
{
    processDumpsysStatStartStop_pushbutton->setText(tr("Stop"));
    main_toolbox->setItemIcon(4,QIcon(QPixmap(":/images/right_green.png")));
    processDumpsysStatFilter_lineedit->setEnabled(false);
    processDumpsysStatTimeout_spinbox->setEnabled(false);
}

/* Slot on button dumpsys stop start clicked
 * sop start timer and emit signals for that
 */
void adbViewer::on_processDumpsysStatStartStop_pushbutton_clicked()
{
    if(processDumpsysStatFilter_lineedit->text()!="")
        {
            if(dumpsysProcTimer.isActive())
                {
                    dumpsysProcTimer.stop();
                    processDumpsysAndroidVersion="";
                    emit dumpsysProcStopped();
                }
            else
                {
                    processDumpsysAndroidVersion=executeADBCommand("-s " + devicesList_combobox->currentText() + " shell getprop ro.build.version.release").trimmed();
                    QTextStream cout(stdout);
                    cout<<"Android Version: "<<processDumpsysAndroidVersion<<endl;
                    dumpsysProcTimer.start(processDumpsysStatTimeout_spinbox->value() * 1000);
                    emit dumpsysProcStarted();
                }
        }
}

/* Slot connected to dumpsys timer
 * here's launched dumpsys and parse all the data to table
 */
void adbViewer::on_dumpsysProcTimer()
{
    QTextStream cout(stdout);
    cout<<"dumpsys triggered"<<endl;
    QString dt= QDateTime::currentDateTime().toString("dd/MM/yyyy_hh:mm:ss");

    QString result=executeADBCommand("-s " + devicesList_combobox->currentText() + " shell dumpsys meminfo " + processDumpsysStatFilter_lineedit->text());

    //another stupid parsing
    if(!result.toLower().contains("no process found") && result!="")
        {
            result=result.replace(QRegExp("(\\S)\\s(\\S)"),"\\1_\\2");
            //result=result.trimmed();
            while(result.contains("  "))
                {
                    result.replace(QString("  "),QString(" "));
                }
            //qDebug()<<result;
            QStringList resultStringList;
            foreach (QString str, result.split("\n",QString::SkipEmptyParts))
                {
                    resultStringList.append(str.trimmed());
                }
            QString dataLine=dt + "," + processDumpsysStatFilter_lineedit->text() + ",";
            QString headerLine="Time,Name,";

            QString currentsection="";
            QString subcurrentsection="";
            QString customsubheader="";
            QStringList bigheaderList=QStringList();
            int headerstrflag=0;
            foreach (QString str, resultStringList)
            {

                QTextStream cout(stdout);
                cout<<str<<endl;
                    if(str.toLower().contains("pid") && str.contains(processDumpsysStatFilter_lineedit->text()))
                        {
                            QStringList columnsStrings=str.split("_");
                            for(int columni=0; columni<columnsStrings.count()-1; columni++)
                                {
                                    if(columnsStrings[columni].toLower()=="pid")
                                        {
                                            bool ok;
                                            int first = columnsStrings[columni+1].toInt(&ok);
                                            Q_UNUSED(first);
                                            if(ok)
                                                {
                                                    headerLine=headerLine+"pid,";
                                                    dataLine=dataLine+columnsStrings[columni+1]+",";
                                                }
                                            currentsection="bigheader";
                                        }

                                }
                        }

                    //can failed if some section missed
                    if(currentsection=="bigheader" && !str.contains("----") && str.split(" ").count()>1)
                        {
                            if(headerstrflag<1)
                                {
                                    bigheaderList=str.split(" ");

                                }
                            else
                                {
                                    if(str.split(" ").count()==bigheaderList.count())
                                        {
                                            for(int i=0; i<bigheaderList.count(); i++)
                                                {
                                                    bigheaderList[i]=bigheaderList[i]+ "_" +str.split(" ")[i];
                                                }
                                        }
                                }
                            headerstrflag++;
                        }
                    else if(currentsection=="bigheader" && str.contains("----"))
                        {
                            currentsection="afterbigheader";
//                if(bigheaderList.count()>0)
//                {
//                    headerLine=headerLine+bigheaderList.join(",")+",";
//                }

                        }
                    else if(currentsection=="afterbigheader" && bigheaderList.count()>0)
                        {
                            QStringList valuesafterbigheader=str.split(" ");

                            for(int valueid=0; valueid<valuesafterbigheader.count(); valueid++)
                                {
                                    if(valueid>0)
                                        {
                                            headerLine=headerLine+valuesafterbigheader[0]+"|" +bigheaderList[valueid-1]+",";
                                            dataLine=dataLine+ valuesafterbigheader[valueid]+",";
                                        }

                                }
                            if(str.contains("TOTAL"))
                                {
                                    currentsection="aftermemmoryusage";
                                }

                        }
                    else if(currentsection=="aftermemmoryusage")
                    {
                        if(subcurrentsection=="" && str.toLower().contains("app_summary"))
                        {
                            subcurrentsection="appsummary";
                            customsubheader=str;
                            headerstrflag=true;
                        }
                        else if(subcurrentsection=="appsummary" && headerstrflag)
                        {
                            if(str.contains("----"))
                            {
                                headerstrflag=false;
                            }
                            else
                            {
                                customsubheader=customsubheader+"|"+str;
                            }
                        }
                        else if(subcurrentsection=="appsummary" && !headerstrflag && str.contains(":"))
                        {
                            foreach(QString node, str.split(" "))
                            {
                               if(node.contains(":"))
                               {
                                    headerLine=headerLine+customsubheader+"|"+node.left(node.length()-1)+",";
                               }
                               else
                               {
                                   dataLine=dataLine+node+",";
                               }
                            }
                        }
                        if(subcurrentsection=="appsummary" && !headerstrflag && str.contains("TOTAL"))
                        {
                            subcurrentsection="";
                            customsubheader="";

                        }

                        if(subcurrentsection=="" && str.contains("Objects"))
                        {
                            subcurrentsection="objects";
                            customsubheader=str;
                        }
                        else if(subcurrentsection=="objects" && str.contains(":"))
                        {
                            foreach(QString node, str.split(" "))
                            {
                               if(node.contains(":"))
                               {
                                    headerLine=headerLine+customsubheader+"|"+node.left(node.length()-1)+",";
                               }
                               else
                               {
                                   dataLine=dataLine+node+",";
                               }
                            }
                        }
                        else if(subcurrentsection=="objects" && !str.contains(":"))
                        {
                            subcurrentsection="";
                        }

                        if(subcurrentsection=="" && str.contains("SQL"))
                        {
                            subcurrentsection="sql";
                            customsubheader=str;
                        }
                        else if(subcurrentsection=="sql" && str.contains(":"))
                        {
                            foreach(QString node, str.split(" "))
                            {
                               if(node.contains(":"))
                               {
                                    headerLine=headerLine+customsubheader+"|"+node.left(node.length()-1)+",";
                               }
                               else
                               {
                                   dataLine=dataLine+node+",";
                               }
                            }
                        }
                        else if(subcurrentsection=="sql" && !str.contains(":"))
                        {
                            subcurrentsection="";
                        }


                    }




                }

            if(headerLine.lastIndexOf(","))
                {
                    headerLine=headerLine.left(headerLine.length()-1);
                }
            if(dataLine.lastIndexOf(","))
                {
                    dataLine=dataLine.left(dataLine.length()-1);
                }


            QTextStream cout(stdout);
            cout<<headerLine<<endl;
            cout<<dataLine<<endl;
            //fill in table
            QStringList headerLineList=headerLine.split(",");
            QStringList dataLineList=dataLine.split(",");
            //check if more than time and process name in result
            if(headerLineList.count()>2 && dataLineList.count()==headerLineList.count())
                {

                    int rowj=processDumpsysStat_model->rowCount();
                    int coli=0;
                    foreach (const QString &headerstr, headerLineList)
                        {
                            int headerID=getHeaderIDbyNameOfProcessDumpsysStat(headerstr);
                            if(headerID==-1)
                                {
                                    headerID=processDumpsysStat_model->columnCount();
                                    processDumpsysStat_model->setHorizontalHeaderItem(headerID, new QStandardItem(QString(headerstr)));

                                }

                            bool isnumber;
                            QString columnvalue=dataLineList[coli];
                            int columnvaluenumber = columnvalue.toInt(&isnumber);
                            if(isnumber)
                                {
                                    QStandardItem *numberitem = new QStandardItem();
                                    numberitem->setData(QVariant(columnvaluenumber), Qt::DisplayRole);
                                    processDumpsysStat_model->setItem(rowj,headerID,numberitem);
                                }
                            else
                                {
                                    processDumpsysStat_model->setItem(rowj,headerID,new QStandardItem(QString(columnvalue)));
                                }
                            coli++;


                        }
                }


            int rowj=processDumpsysStat_model->rowCount()-1;
            //write top process stat into file
            QString headersList="";
            QString logLine="";
            QString pidcur="";
            QString namecur="";
            if(processDumpsysStatEnableLog_checkbox->isChecked())
                {
                    for(int i = 0; i <processDumpsysStat_model->columnCount(); i++)
                        {
                            headersList=headersList+processDumpsysStat_model->headerData(i, Qt::Horizontal).toString() + ",";

                            if (processDumpsysStat_model->hasIndex(rowj,i))
                                {
                                    //qDebug()<<processTopStat_model->index(row ,  i).data();
                                    logLine=logLine+processDumpsysStat_model->data(processDumpsysStat_model->index(rowj,i)).toString() +",";
                                }
                            if(processDumpsysStat_model->headerData(i, Qt::Horizontal).toString().toLower().contains(QString("pid")))
                                {
                                    pidcur=processDumpsysStat_model->data(processDumpsysStat_model->index(rowj,i)).toString();
                                }
                            if(processDumpsysStat_model->headerData(i, Qt::Horizontal).toString().toLower().contains(QString("name")))
                                {
                                    namecur=processDumpsysStat_model->data(processDumpsysStat_model->index(rowj,i)).toString();
                                }

                        }
                    if(headersList!="")
                        {
                            headersList=headersList.left(headersList.length()-1);
                        }
                    if(logLine!="")
                        {
                            logLine=logLine.left(logLine.length()-1);
                        }
                    QTextStream cout(stdout);
                    cout<<headersList<<endl<<logLine<<endl;
                    writeStatCSV(namecur + "_" + pidcur +"_dumpsys",logLine,headersList);
                }
            emit processDumpsysStatNewData(headersList,logLine);

            if(processDumpsysStat_model->rowCount()<3)
            {
                processDumpsysStat_tableview->resizeColumnsToContents();
            }
            if(processDumpsysStat_tableview->verticalScrollBar()->value()==processDumpsysStat_tableview->verticalScrollBar()->maximum())
                {
                    processDumpsysStat_tableview->scrollToBottom();
                }

        }
    else
        {
        QTextStream cout(stdout);
        cout<<"No such process or incorrect output"<<endl;
        }
}

/* Function to read full file
 * strPath - path start from application dir
 * return: file contents or empty
 */
QString adbViewer::ReadFullFile(QString strPath)
{
    QFile file(qApp->applicationDirPath() + "/" +strPath);
    QString filetext =QString();
    if(!file.exists())
        {
            return "";
        }
    else
        {
            file.open(QIODevice::ReadOnly);
            filetext=file.readAll();
            file.close();

        }

    return filetext;
}


/* slot to write custom string into custom file (file wil be overrided)
 * strPath - file path
 * appendString - string to append (can contain multiline)
 */
void adbViewer::on_RewriteFullFileSignal(QString strPath, QString appendString)
{
    QFile outFile(qApp->applicationDirPath() + "/" +strPath);
    outFile.open(QIODevice::WriteOnly);
    QTextStream textStream(&outFile);
    textStream <<appendString<<endl;
    outFile.close();
}

/* Slot on remember current filter button clicked
 * add to filter combobox list and rewrite file with values
 */
void adbViewer::on_rememberFilter_toolbutton_clicked()
{
    QString currenttext=filterCondition_combobox->currentText();
    int testindex=0;
            testindex=filterCondition_combobox->findText(currenttext);
    if(testindex==-1)
    {
    filterCondition_combobox->addItem(currenttext,QVariant(useRegexp_checkbox->isChecked()));
    //select just added item
    int index=0;
    index =  filterCondition_combobox->findText(currenttext);
    QTextStream cout(stdout);
    cout<<"selection comboBox_env: " + QString::number(index)<<endl;
    if ( index != -1 ) { // -1 for not found

       filterCondition_combobox->setCurrentIndex(index);
       useRegexp_checkbox->setChecked(filterCondition_combobox->currentData().toBool());
    } else { QTextStream cout(stdout);
        cout<<"index not found!!!"<<endl;}

    }
    else if(testindex!=-1 && filterCondition_combobox->itemData(testindex).toBool()!=useRegexp_checkbox->isChecked())
    {
        filterCondition_combobox->setItemData(testindex,QVariant(useRegexp_checkbox->isChecked()));
    }
    updateFilters();

}

/* Slot on remove current filter button clicked
 * remove filter from combobox list and rewrite file with values
 */
void adbViewer::on_deleteFilter_toolbutton_clicked()
{
    //yes it's possible to remove by index, but I prefer behavior when it's deleted by text
    //so when you edit data you will delete exectly what you type or select
    QString currenttext=filterCondition_combobox->currentText();
    int index=0;
    index =  filterCondition_combobox->findText(currenttext);
    QTextStream cout(stdout);
    cout<<"selection comboBox_env: " + QString::number(index)<<endl;
    if ( index != -1 ) { // -1 for not found

       filterCondition_combobox->removeItem(index);
    } else { QTextStream cout(stdout);
        cout<<"index not found!!!"<<endl;}
    updateFilters();

}

/* Function to update filters in file
 */
void adbViewer::updateFilters()
{
    QString saveItems="";
    for(int i=0;i<filterCondition_combobox->count();i++)
    {
        saveItems =saveItems + filterCondition_combobox->itemText(i) +","+ ((filterCondition_combobox->itemData(i).toBool())?QString("true"):QString("false")) + "\n";
    }
    QTextStream cout(stdout);
    cout<<saveItems<<endl;
    emit RewriteFullFileSignal("filters_list",saveItems);
}

/* slot on filter combobox inex changed
 * read if regexp and set checkbox accordingly
*/
void adbViewer::on_filterCondition_combobox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    useRegexp_checkbox->setChecked(filterCondition_combobox->currentData().toBool());
}

/* Hide show filter section
 * yeah I know it would me much more smarter to put all elements onto some widget
 * but i've did as I did
 */
void adbViewer::on_showFilterSection_checkbox_clicked()
{

if(showFilterSection_checkbox->isChecked())
{
    logcatFiltered_plaintextedit->setVisible(false);
    filterCondition_combobox->setVisible(false);
    rememberFilter_toolbutton->setVisible(false);
    deleteFilter_toolbutton->setVisible(false);
    useRegexp_checkbox->setVisible(false);
    enableFilter_checkbox->setVisible(false);
    showFilterSection_checkbox->setText(tr("Show filter section"));

}
else
{
    logcatFiltered_plaintextedit->setVisible(true);
    filterCondition_combobox->setVisible(true);
    rememberFilter_toolbutton->setVisible(true);
    deleteFilter_toolbutton->setVisible(true);
    useRegexp_checkbox->setVisible(true);
    enableFilter_checkbox->setVisible(true);
    showFilterSection_checkbox->setText(tr("Hide filter section"));
}
}


/* I know it's little bit stupid but it does what I want
 * the first item appended to list is prefarable to launch
 */
QString adbViewer::whereAdbExists()
{
    QString returnpath="";
    QTextStream cout(stdout);
    cout<<"Choosing ADB path"<<endl;
    QStringList adbPath=QStringList();
#ifdef Q_OS_MAC
    adbPath.append("./adb");
    adbPath.append("~/Library/Android/sdk/platform-tools/adb");
    adbPath.append("adb");
    foreach(const QString &path,adbPath)
    {
        if(QFile(path).exists())
        {
            returnpath=path;
            break;
        }
    }
#endif
#ifdef Q_OS_WIN
    adbPath.append(qApp->applicationDirPath() + "/adb.exe");
    adbPath.append("/adb.exe");
            foreach(const QString &path,adbPath)
            {
                if(QFile(path).exists())
                {
                    returnpath=path;
                    break;
                }
            }

#endif
#ifdef Q_OS_LINUX
    adbPath.append("./adb");
    adbPath.append(qApp->applicationDirPath() + "/adb");
    adbPath.append("/usr/bin/adb");
    adbPath.append("adb");
    foreach(const QString &path,adbPath)
    {
        if(QFile(path).exists())
        {
            returnpath=path;
            break;
        }
    }
#endif
    cout<<"adb found: "<<returnpath<<endl;
    return returnpath;
}
