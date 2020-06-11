/* Author: Yuriy Kuzin
 * widget "main container" to place all clases onto it
 * adbviewer and charts
 */
#ifndef MAINCONTAINER_H
#define MAINCONTAINER_H

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include "adbviewer.h"
#include "qcustomplot.h"
#include "chartmanager.h"
class mainContainer;
namespace Ui {
class mainContainerForm;
}

class mainContainer : public QWidget
{
        Q_OBJECT
public:
    mainContainer(QWidget *parent = 0);
    ~mainContainer();
    QString datetimefile;
    adbViewer *adbViewer_cont;
    chartManager *chartsTop;
    chartManager *chartsDumpsys;
    QToolButton *resetAllCharts_toolbutton;
    QToolButton *showhideTop;
    QToolButton *showhideDump;
    QLabel *range_lable;
    QSpinBox *range_spinbox;
    QCheckBox *flow_checkbox;
    QToolButton *savechartimages_toolbutton;
    QComboBox *imageformat_combobox;
    QSpinBox *imagewidth_spinbox;
    QSpinBox *imageheight_spinbox;


signals:

public slots:
    void on_showhidebuttons_clicked();
    void on_range_spinbox_valueChanged(int range);
    int on_savechartimages_toolbutton_clicked();

private:
    Ui::mainContainerForm *ui;
};

#endif // MAINCONTAINER_H
