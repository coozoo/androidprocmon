/* Author: Yuriy Kuzin
 * genarated by qt just a few strings are mine
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create form with all elements and place it into groupbox
    form= new mainContainer(ui->groupBox);
    ui->gridLayout_4->addWidget(form);

}

MainWindow::~MainWindow()
{
    delete form;
    delete ui;
}
