/* Author: Yuriy Kuzin
 * I'm tired to put it in code so generated it by qt
 */
/********************************************************************************
** Form generated from reading UI file 'maincontainer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINCONTAINER_H
#define UI_MAINCONTAINER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QTabWidget *adbviewer_tabWidget;
    QWidget *ADBviewer;
    QGridLayout *gridLayout_2;
    QGroupBox *adbviewer_groupBox;
    QWidget *charts_tab;
    QGridLayout *gridLayout_3;
    QScrollArea *charts_scrollArea;
    QLineEdit *chartsHeader_lineedit;
    QWidget *scrollAreaWidgetContents;
    QScrollArea *chartsDumpsys_scrollArea;
    QLineEdit *chartsDumpsysHeader_lineedit;
    QWidget *scrollAreaWidgetContents_2;
    QToolBar *toolbar;


    void setupUi(QWidget *mainContainerForm)
    {
        if (mainContainerForm->objectName().isEmpty())
            mainContainerForm->setObjectName(QStringLiteral("mainContainerForm"));
        mainContainerForm->resize(400, 300);
        gridLayout = new QGridLayout(mainContainerForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        adbviewer_tabWidget = new QTabWidget(mainContainerForm);
        adbviewer_tabWidget->setObjectName(QStringLiteral("adbviewer_tabWidget"));
        ADBviewer = new QWidget();
        ADBviewer->setObjectName(QStringLiteral("ADBviewer"));
        gridLayout_2 = new QGridLayout(ADBviewer);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        adbviewer_groupBox = new QGroupBox(ADBviewer);
        adbviewer_groupBox->setObjectName(QStringLiteral("adbviewer_groupBox"));

        gridLayout_2->addWidget(adbviewer_groupBox, 0, 0, 1, 1);

        adbviewer_tabWidget->addTab(ADBviewer, QString());
        charts_tab = new QWidget();
        charts_tab->setObjectName(QStringLiteral("charts_tab"));
        gridLayout_3 = new QGridLayout(charts_tab);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        charts_scrollArea = new QScrollArea(charts_tab);
        charts_scrollArea->setObjectName(QStringLiteral("charts_scrollArea"));
        charts_scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 356, 106));
        charts_scrollArea->setWidget(scrollAreaWidgetContents);


        gridLayout_3->addWidget(charts_scrollArea, 2, 0, 1, 1);


        chartsHeader_lineedit = new QLineEdit(charts_tab);
        chartsHeader_lineedit->setObjectName(QStringLiteral("chartsHeader_lineedit"));
        chartsHeader_lineedit->setFrame(false);
        chartsHeader_lineedit->setReadOnly(true);
        chartsHeader_lineedit->setText(QObject::tr("Charts Top"));
        chartsHeader_lineedit->setStyleSheet("QLineEdit { font: bold; }");
        //chartsHeader_lineedit->setStyleSheet("QLineEdit { font: bold; color:rgb(0, 0, 0); background: rgb(255, 255, 255); selection-background-color: rgb(233, 99, 0); }");

        gridLayout_3->addWidget(chartsHeader_lineedit, 1, 0, 1, 1);

        chartsDumpsys_scrollArea = new QScrollArea(charts_tab);
        chartsDumpsys_scrollArea->setObjectName(QStringLiteral("chartsDumpsys_scrollArea"));
        chartsDumpsys_scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 356, 80));
        chartsDumpsys_scrollArea->setWidget(scrollAreaWidgetContents_2);

        gridLayout_3->addWidget(chartsDumpsys_scrollArea, 4, 0, 1, 1);

        chartsDumpsysHeader_lineedit = new QLineEdit(charts_tab);
        chartsDumpsysHeader_lineedit->setObjectName(QStringLiteral("chartsDumpsysHeader_lineedit"));
        chartsDumpsysHeader_lineedit->setFrame(false);
        chartsDumpsysHeader_lineedit->setReadOnly(true);
        chartsDumpsysHeader_lineedit->setText(QObject::tr("Charts Dumpsys"));
        chartsDumpsysHeader_lineedit->setStyleSheet("QLineEdit { font: bold; }");

        gridLayout_3->addWidget(chartsDumpsysHeader_lineedit, 3, 0, 1, 1);

        toolbar=new QToolBar(charts_tab);
        toolbar->setObjectName(QStringLiteral("toolbar"));
        toolbar->setGeometry(0,21,509,458);
        toolbar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
        toolbar->setMovable(true);
        toolbar->setFloatable(true);
        toolbar->setAllowedAreas(Qt::AllToolBarAreas);

        gridLayout_3->addWidget(toolbar, 0, 0);

        adbviewer_tabWidget->addTab(charts_tab, QString());

        gridLayout->addWidget(adbviewer_tabWidget, 0, 0, 1, 1);


        retranslateUi(mainContainerForm);

        adbviewer_tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainContainerForm);
    } // setupUi

    void retranslateUi(QWidget *mainContainerForm)
    {
        mainContainerForm->setWindowTitle(QApplication::translate("mainContainerForm", "mainContainerForm", 0));
        adbviewer_groupBox->setTitle(QString());
        adbviewer_tabWidget->setTabText(adbviewer_tabWidget->indexOf(ADBviewer), QApplication::translate("mainContainerForm", "ADB manager", 0));
        adbviewer_tabWidget->setTabText(adbviewer_tabWidget->indexOf(charts_tab), QApplication::translate("mainContainerForm", "Charts Manager", 0));
    } // retranslateUi

};

namespace Ui {
    class mainContainerForm: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINCONTAINER_H
