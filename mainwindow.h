/* Author: Yuriy Kuzin
 * genarated by qt just a few string are mine
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <settingswindow.h>
#include "adbviewer.h"
#include "qcustomplot.h"
#include "chartmanager.h"
#include "maincontainer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    mainContainer *form;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QToolButton *settings_toolbutton;
    QToolButton *about_toolbutton;


private:
    Ui::MainWindow *ui;


public slots:
    void aboutWindow();
    void settingsWindow();
};

#endif // MAINWINDOW_H
