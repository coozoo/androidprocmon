/* Author: Yuriy Kuzin
 * genarated by qt just a few string are mine
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
