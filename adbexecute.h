/* Author: Yuriy Kuzin
 */
#ifndef ADBEXECUTE_H
#define ADBEXECUTE_H

#include <QObject>
#include <QMainWindow>
#include <QDebug>
#include <QDockWidget>
#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QProcess>
#include <QCheckBox>
#include <QSpinBox>
#include <QToolBox>
#include <QCompleter>
#include <QToolButton>
#include <QPushButton>
#include <QTimer>
#include <QTextStream>
#include "qplaintexteditcursor.h"

class adbExecute : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString adbBinary
           READ getadbBinary
           WRITE setadbBinary
        )
    Q_PROPERTY(QString androidDevice
           READ getandroidDevice
           WRITE setandroidDevice
        )


public:
    explicit adbExecute(QMainWindow *parent = 0);
    ~adbExecute();

    //execute command section
    QPlainTextEditCursor *execresult_plaintextedit;
    QHBoxLayout *toolbar_hboxlayout;
    QProcess *procexec;
    QCheckBox *procexecGetSU_checkbox;
    QCheckBox *executeUseLoop_checkbox;
    QSpinBox *executeUseLoopDelay_spinbox;
    QLineEdit *execute_lineedit;
    QCompleter *completer;
    int modelCursor;
    QString executeADBCommand(QString adbCommand);
    QToolButton *refreshDevices_toolbutton;
    QPushButton *executeCommand_pushbutton;
    QToolButton *resetConsole_toolbutton;

    void getSU();

    //functions to write history (commands and regexp) into file
    QString ReadFullFile(QString strPath);

    void setadbBinary(QString m_adbBinary)
    {
        adbBinary = m_adbBinary;
    }
    QString getadbBinary() const
    { return adbBinary; }

    void setandroidDevice(QString m_androidDevice)
    {
        androidDevice = m_androidDevice;
    }
    QString getandroidDevice() const
    { return androidDevice; }


private:
    QTimer executeUseLoopTimer;
    QString adbBinary;
    QString androidDevice;
    QStringList tabSuggest_stringlist;

signals:
    void AppendStringToFileSignal(QString strPath, QString appendString);
    void RewriteFullFileSignal(QString strPath, QString appendString);

public slots:

private slots:
    void on_androidDeviceChanged(QString strAndroidDevice);
    void on_executeCommand_pushbutton_clicked();
    void on_resetConsole_toolbutton_clicked();
    void on_procexecGetSU_checkbox_clicked();
    void on_executeUseLoopTimer();
    void on_procexec_closed(int reason);
    void on_execresult_plaintextedit_keyPress(QKeyEvent *e);

    void cmdUpdateText();
    void cmdUpdateError();

    void on_AppendStringToFile(QString strPath, QString appendString);
    void on_RewriteFullFileSignal(QString strPath, QString appendString);
    void on_completer_activated(QString);

protected:
     bool eventFilter(QObject* obj, QEvent *event);

};

#endif // ADBEXECUTE_H
