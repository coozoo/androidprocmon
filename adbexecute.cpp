/* Author: Yuriy Kuzin
 */
#include "adbexecute.h"

adbExecute::adbExecute(QMainWindow *parent) : QMainWindow(parent)
{
    modelCursor = -1;
#ifdef Q_OS_WIN
    sethistoryDir(qApp->applicationDirPath());
#endif
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    sethistoryDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0] + "/" + qAppName());
#endif
    QTextStream cout(stdout);
    cout<<gethistoryDir()<<endl;
    QDir().mkpath(gethistoryDir()) ;
    //read history from file
    QStringList wordList = ReadFullFile("exec_history").split("\n");
    //QSet<QString> wordset=wordList.toSet();
    //wordList=wordset.toList();

    //inverse strings from history file to make appear latest on top of completer (got this oneliner somwhere from stackowerflow)
    for (int k = 0; k < (wordList.size() / 2); k++) wordList.swap(k, wordList.size() - (1 + k));
    //leave only unique strings to update history file later
    QStringList wordListUnique;
    for (int k = 0; k < wordList.count(); k++)
        {
            if (wordList[k] != "" && !wordListUnique.contains(wordList[k], Qt::CaseSensitive))
                {
                    wordListUnique.append(wordList[k]);
                }
        }
    //set completer to suggest commands when you typing into execute field
    completer = new QCompleter(wordListUnique);
    completer->setMaxVisibleItems(10);
    completer->setCompletionMode(QCompleter::PopupCompletion);

    //execute field with history commands
    execute_lineedit = new QLineEdit();
    execute_lineedit->setCompleter(completer);
    execute_lineedit->installEventFilter(this);
    //connect completer with command history when you selecting somethig to slot
    connect(completer, SIGNAL(activated(QString)), this, SLOT(on_completer_activated(QString)), Qt::QueuedConnection);

    //execute section
    //try to get su while executing commands
    procexecGetSU_checkbox = new QCheckBox();
    procexecGetSU_checkbox->setText(tr("SU"));
    procexecGetSU_checkbox->setToolTip(tr("Execute as root"));
    //plaintextedit to show it like command line style
    execresult_plaintextedit = new QPlainTextEditCursor();
    //QFont font("unexistent");
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    execresult_plaintextedit->setFont(font);
    QPalette palette = execresult_plaintextedit->palette();
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::green);
    execresult_plaintextedit->setPalette(palette);
    execresult_plaintextedit->setReadOnly(true);
    //execresult_plaintextedit->setCursorWidth(8);
    execresult_plaintextedit->setTextInteractionFlags(execresult_plaintextedit->textInteractionFlags() | Qt::TextSelectableByKeyboard);

    //execute section composing
    QGridLayout *execute_gridlayout = new QGridLayout();
    QGroupBox *execute_groupbox = new QGroupBox();
    executeUseLoop_checkbox = new QCheckBox();
    executeUseLoop_checkbox->setToolTip(tr("Execute Command in loop"));
    executeUseLoop_checkbox->setText(tr("Use Loop"));
    executeUseLoopDelay_spinbox = new QSpinBox();
    executeUseLoopDelay_spinbox->setMinimum(1);
    executeUseLoopDelay_spinbox->setValue(5);
    executeUseLoopDelay_spinbox->setToolTip(tr("Set Timeout to loop command"));
    executeCommand_pushbutton = new QPushButton();
    executeCommand_pushbutton->setText(tr("Execute"));
    executeCommand_pushbutton->setToolTip(tr("Execute on device"));
    resetConsole_toolbutton = new QToolButton;
    resetConsole_toolbutton->setToolTip(tr("Reset console connection"));
    resetConsole_toolbutton->setIcon(QIcon(QPixmap(":/images/error.png")));
    execute_groupbox->setLayout(execute_gridlayout);
    execute_gridlayout->addWidget(procexecGetSU_checkbox, 0, 0);
    execute_gridlayout->addWidget(executeUseLoop_checkbox, 0, 1);
    execute_gridlayout->addWidget(executeUseLoopDelay_spinbox, 0, 2);
    execute_gridlayout->addWidget(execresult_plaintextedit, 1, 0, 1, 3);
    execute_gridlayout->addWidget(execute_lineedit, 2, 0);
    execute_gridlayout->addWidget(executeCommand_pushbutton, 2, 1);
    execute_gridlayout->addWidget(resetConsole_toolbutton, 2, 2);


    QDockWidget *execute_dockwidget = new QDockWidget("ADB shell", parent);
    //QDockWidget *execute_dockwidget = qobject_cast<QDockWidget*>(execute_titledockwidget);
    //execute_dockwidget->setFloating( !execute_dockwidget->isFloating () );
    //execute_dockwidget->showMaximized();
    //execute_dockwidget->setTitleBarWidget();
    parent->addDockWidget(Qt::RightDockWidgetArea, execute_dockwidget);
    execute_dockwidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    execute_dockwidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    execute_dockwidget->setWidget(execute_groupbox);



    //command line process
    procexec = new QProcess();

    //append string to file by emitted signal
    connect(this, SIGNAL(AppendStringToFileSignal(QString, QString)), this, SLOT(on_AppendStringToFile(QString, QString)));

    // execute command button and enter in command line calls the same slot
    connect(executeCommand_pushbutton, SIGNAL(clicked()), this, SLOT(on_executeCommand_pushbutton_clicked()));
    connect(execute_lineedit, SIGNAL(returnPressed()), this, SLOT(on_executeCommand_pushbutton_clicked()));
    //get su checkbox to get root access
    connect(procexecGetSU_checkbox, SIGNAL(clicked()), this, SLOT(on_procexecGetSU_checkbox_clicked()));

    //execute command line lost connection
    connect(procexec, SIGNAL(finished(int)), this, SLOT(on_procexec_closed(int)));

    //catches keypresses over plaintextedit area
    connect(execresult_plaintextedit, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(on_execresult_plaintextedit_keyPress(QKeyEvent *)));
    //reset console it will on button to replace ctrl+c functio in console it will kill process
    connect(resetConsole_toolbutton, SIGNAL(clicked()), this, SLOT(on_resetConsole_toolbutton_clicked()));

    //read all outputs from command line executor
    connect(procexec, SIGNAL(readyReadStandardError()), this, SLOT(cmdUpdateError()));
    connect(procexec, SIGNAL(readyReadStandardOutput()), this, SLOT(cmdUpdateText()));

    connect(&executeUseLoopTimer, SIGNAL(timeout()), this, SLOT(on_executeUseLoopTimer()));

    //emitted siganl to override file
    connect(this, SIGNAL(RewriteFullFileSignal(QString, QString)), this, SLOT(on_RewriteFullFileSignal(QString, QString)));

    //yeah I know it's a stupid to swap, traverse, inverse, blablabla.. but I'm generating stupid things
    //traverse back and update history file with unique only
    for (int k = 0; k < (wordListUnique.size() / 2); k++) wordListUnique.swap(k, wordListUnique.size() - (1 + k));
    emit RewriteFullFileSignal("exec_history", wordListUnique.join("\n"));


}

/*everything should be killed hope I didn't miss something*/
adbExecute::~adbExecute()
{
    execresult_plaintextedit->deleteLater();
    execute_lineedit->deleteLater();
    if (procexec->processId() > 0)
        {
            procexec->close();
            procexec->deleteLater();
        }
}

/* Slot to execute command */
void adbExecute::on_executeCommand_pushbutton_clicked()
{
    QTextStream cout(stdout);

    /* movecursor everytime to the end of plaintext */
    QTextCursor cursor(execresult_plaintextedit->textCursor());
    cursor.movePosition(QTextCursor::End);
    execresult_plaintextedit->setTextCursor(cursor);

    if (executeUseLoop_checkbox->isChecked() && execute_lineedit->text() != "")
        {
            if (!executeUseLoopTimer.isActive())
                {
                    executeUseLoopTimer.setSingleShot(true);
                    executeUseLoopTimer.start(executeUseLoopDelay_spinbox->value() * 1000);
                    execute_lineedit->setEnabled(false);
                    executeCommand_pushbutton->setEnabled(false);
                }
        }

    if (!procexec->isOpen())
        {
            procexec->start(adbBinary + " -s " + androidDevice + " shell");
            if (!procexec->isOpen() && procexecGetSU_checkbox->isChecked())
                {
                    getSU();
                }
            //interesting that I'm getting crash here only under linux under windows works fine...
            //if in some reson process doesn't exists anymore, for example device disconnected
            if (!procexec->isOpen())
                {
                    procexec->write(QString(execute_lineedit->text() + "\n").toStdString().c_str());
                }
            else
                {
                    cout << "Something weird no such device:" << androidDevice << ", maybe it was disconnected." << endl;
                }


        }
    else
        {
            procexec->write((execute_lineedit->text() + "\n").toStdString().c_str());
        }
    if (execute_lineedit->text() == "clear")
        {
            execresult_plaintextedit->clear();
        }
    //update completer and history with entered command
    if (execute_lineedit->text() != "")
        {
            completer->model()->insertRow(0);
            completer->model()->setData(completer->model()->index(0, 0), execute_lineedit->text());
            emit AppendStringToFileSignal("exec_history", execute_lineedit->text());
        }
    modelCursor = -1;
    if (!executeUseLoop_checkbox->isChecked())
        {
            execute_lineedit->setText("");
            execute_lineedit->clear();
        }
    execute_lineedit->setFocus();
}

void adbExecute::on_executeUseLoopTimer()
{
    on_executeCommand_pushbutton_clicked();
    if (!executeUseLoop_checkbox->isChecked())
        {
            execute_lineedit->setEnabled(true);
            executeCommand_pushbutton->setEnabled(true);
        }
}

/* on su checkbox checked try to start process as su
 * if unchecked then closeprocess
*/
void adbExecute::on_procexecGetSU_checkbox_clicked()
{
    if (procexecGetSU_checkbox->isChecked())
        {
            getSU();
        }
    else
        {
            if (procexec->isOpen())
                {
                    procexec->close();
                }
        }
}
/* Function to get su on rooted device
 * if connection already opened then close
 *  and try to get in the same way when closed
 */
void adbExecute::getSU()
{
    if (!procexec->isOpen())
        {
            procexec->start(adbBinary + " -s " + androidDevice + " shell\n");
            execresult_plaintextedit->appendPlainText("####### Trying to get root privileges #######");
            if (procexec->isOpen())
                {
                    procexec->write(QString("su\n").toStdString().c_str());
                    if (execute_lineedit->text() != "")
                        {
                            procexec->write(QString(execute_lineedit->text() + "\n").toStdString().c_str());
                        }
                }
//            else
//            {
//            }

        }
    else
        {
            execresult_plaintextedit->appendPlainText("####### Trying to get root privileges #######");
            procexec->write(QString("su\n").toStdString().c_str());
            procexec->write(QString(execute_lineedit->text() + "\n").toStdString().c_str());
        }
}


/* slot called by reset button
 * to close process launched by execute command line
 */
void adbExecute::on_resetConsole_toolbutton_clicked()
{
    if (procexec->isOpen())
        {
            procexec->close();
        }
}

/* slot when completer activated
 */
void adbExecute::on_completer_activated(QString)
{
    if (!executeUseLoop_checkbox->isChecked() && execute_lineedit->text() != "")
        {
            execute_lineedit->clear();
            execute_lineedit->setText("");
        }
    execute_lineedit->setFocus();
}

/* Slot to execute on command line process closed */
void adbExecute::on_procexec_closed(int reason)
{
    QTextStream cout(stdout);
    cout << reason << endl;
    execresult_plaintextedit->appendPlainText("####### Adb closed " + QString::number(reason) + " ####### device: " + getandroidDevice());
    if (procexec->isOpen())
        {
            procexec->close();
        }
    executeUseLoop_checkbox->setChecked(false);
}

/* Slot to catch keypresses over plaintext "terminal"
 * set focus linedit for executing command
 * and enter already typed keys to linedit
 */

void adbExecute::on_execresult_plaintextedit_keyPress(QKeyEvent *e)
{
    execute_lineedit->setFocus();
    execute_lineedit->setText(e->text().toUtf8());
}

//on error stream after excute command
void adbExecute::cmdUpdateError()
{
    QString appendText(procexec->readAll());
    execresult_plaintextedit->appendPlainText(appendText.toUtf8().replace("\r", "\n").replace("\n\n", "\n"));
    QTextStream cout(stdout);
    cout << appendText << endl;
}

/* on standard stream after excute command
 * it's not elegant way but at least it works
 * and looks close to terminal but accidentaly
 * platform dependent :(
 */
void adbExecute::cmdUpdateText()
{
//    QTextStream cout(stdout);
//    QString appendText(procexec->readAll());
//    cout<<appendText<<endl;

//    QStringList wholeStringList;
//    //wholeStringList=QString(appendText.toUtf8().replace("\r\n","\n")).split(QRegExp("[\n]"),QString::SkipEmptyParts);
//    wholeStringList=QString(appendText.toUtf8().replace("\r","\n").replace("\n\n","\n")).split(QRegExp("[\n]"),QString::SkipEmptyParts);
//    qDebug()<<wholeStringList;
//    foreach (QString str, wholeStringList)
//        {
//            if(str.contains("@") && str.contains(":/") && (str.contains("$") || str.contains("#")))
//                {
//                    execresult_plaintextedit->appendPlainText("\n"+str);
//                }
//            else
//                {
//                    execresult_plaintextedit->insertPlainText(str+"\n");
//                }
//        }

    QString appendText = "";
    while (procexec->canReadLine())
        {
            QString line = procexec->readLine().trimmed();
            if (!line.isNull() && line != "" && !line.isEmpty())
                {
                    if (line.contains("@") && line.contains(":/") && (line.contains("$") || line.contains("#")))
                        {
                            appendText = "\n" + appendText + line + "\n";
                        }
                    else
                        {
                            appendText = appendText + line + "\n";
                        }
                }
        }
    execresult_plaintextedit->insertPlainText(appendText);

    appendText = procexec->readAll();
    QStringList wholeStringList;
    //wholeStringList=QString(appendText.toUtf8().replace("\r\n","\n")).split(QRegExp("[\n]"),QString::SkipEmptyParts);
    wholeStringList = QString(appendText.toUtf8().replace("\r", "\n").replace("\n\n", "\n")).split(QRegExp("[\n]"), QString::SkipEmptyParts);
    qDebug() << wholeStringList;
    foreach (QString str, wholeStringList)
        {
            if (!str.isNull() && str != "" && !str.isEmpty())
                {
                    if (str.contains(":/") && (str.contains("$") || str.contains("#")))
                        {
                            execresult_plaintextedit->appendPlainText("\n" + str);
                        }
                }
        }


    QTextCursor cursor(execresult_plaintextedit->textCursor());
    cursor.movePosition(QTextCursor::End);
    execresult_plaintextedit->setTextCursor(cursor);
}

/* Function to read full file
 * strPath - path start from application dir
 * return: file contents or empty
 */
QString adbExecute::ReadFullFile(QString strPath)
{
    QFile file(gethistoryDir() + "/" + strPath);
    QString filetext = QString();
    if (!file.exists())
        {
            return "";
        }
    else
        {
            file.open(QIODevice::ReadOnly);
            filetext = file.readAll();
            file.close();

        }

    return filetext;
}

/* slot to append custom string to custom file
 * strPath - file path
 * appendString - string to append (can contain multiline)
 */
void adbExecute::on_AppendStringToFile(QString strPath, QString appendString)
{
    QFile outFile(gethistoryDir() + "/" + strPath);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream textStream(&outFile);
    textStream << appendString << endl;
    outFile.close();
}

/* slot to write custom string into custom file (file wil be overrided)
 * strPath - file path
 * appendString - string to append (can contain multiline)
 */
void adbExecute::on_RewriteFullFileSignal(QString strPath, QString appendString)
{
    QFile outFile(gethistoryDir() + "/" + strPath);
    outFile.open(QIODevice::WriteOnly);
    QTextStream textStream(&outFile);
    textStream << appendString << endl;
    outFile.close();
}

/* Slot universal event filter actually here for execute command linedit
 * obj - some object for example here for linedit
 * event - some event for example keypreses
 */
bool adbExecute::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == execute_lineedit)
        {
            if (event->type() == QEvent::KeyPress)
                {
                    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                    if (keyEvent->key() == Qt::Key_Up)
                        {
                            QTextStream cout(stdout);
                            cout << "lineEdit -> Qt::Key_Up " << modelCursor << endl;
                            if (modelCursor > -1)
                                {
                                    modelCursor--;
                                    execute_lineedit->setText(completer->model()->data(completer->model()->index(modelCursor, 0)).toString());

                                }
                            return true;
                        }
                    else if (keyEvent->key() == Qt::Key_Down)
                        {
                            QTextStream cout(stdout);
                            cout << "lineEdit -> Qt::Key_Down " << modelCursor << endl;
                            if (modelCursor >= -1 && modelCursor < completer->model()->rowCount() - 1)
                                {
                                    modelCursor++;
                                    execute_lineedit->setText(completer->model()->data(completer->model()->index(modelCursor, 0)).toString());

                                }
                            return true;
                        }
                    else if (keyEvent->key() == Qt::Key_Tab)
                        {
                            QTextStream cout(stdout);
                            cout << "lineEdit -> Qt::Key_Tab " << endl;
//                if(modelCursor>=-1 && modelCursor<completer->model()->rowCount()-1)
//                {
//                    modelCursor++;
//                    execute_lineedit->setText(completer->model()->data(completer->model()->index(modelCursor,0)).toString());

//                }
                            return true;
                        }
                }
            return false;
        }
    return adbExecute::eventFilter(obj, event);
}

void adbExecute::on_androidDeviceChanged(QString strAndroidDevice)
{
    if (procexec->isOpen())
        {
            procexec->close();
        }
    setandroidDevice(strAndroidDevice);
}
