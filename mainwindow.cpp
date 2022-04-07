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

    settings_toolbutton=new QToolButton();
    settings_toolbutton->setIcon(QIcon(QPixmap(":/images/configuration.png")));
    settings_toolbutton->setToolTip(tr("Settings"));
    ui->mainToolBar->addWidget(settings_toolbutton);
    about_toolbutton=new QToolButton();
    about_toolbutton->setIcon(QIcon(QPixmap(":/images/info.png")));
    about_toolbutton->setToolTip(tr("Show about info"));
    ui->mainToolBar->addWidget(about_toolbutton);
    connect(about_toolbutton,SIGNAL(clicked()),this,SLOT(aboutWindow()));
    //create form with all elements and place it into groupbox
    form= new mainContainer(ui->groupBox);
    ui->gridLayout_4->addWidget(form);
    connect(settings_toolbutton,&QToolButton::clicked,this,&MainWindow::settingsWindow);

}

MainWindow::~MainWindow()
{
    about_toolbutton->deleteLater();
    delete form;
    delete ui;
}

void MainWindow::aboutWindow()
{
    //just for fun
    QMessageBox msgbox;

    msgbox.setIconPixmap(QPixmap(":/images/info.png").scaled(QSize(40,40)));
    msgbox.setWindowIcon(QPixmap(":/images/info.png").scaled(QSize(40,40)));
    msgbox.setWindowTitle(tr("About"));
    msgbox.setText("<b>" + qApp->property("appname").toString() + " "
                   + qApp->property("appversion").toString() + "</b><br><br>"
                   + tr("Application targeted to make testers life easy :)<br>Monitor memory and CPU using top and dumpsys through ADB.<br>Log statistics to csv files, show charts etc...<br><br>")
                   + tr("Creator: ") + "Yuriy Kuzin<br>"
                   + tr("Compiled with QT: ") + QT_VERSION_STR + "<br>"
                   + tr("Runtime QT: ") + qVersion() +"<br>"
                   #if __GNUC__
                        + tr("Compiler GCC: ") + QString::number(__GNUC__) + "." + QString::number(__GNUC_MINOR__) + "." + QString::number(__GNUC_PATCHLEVEL__) + "<br>"
                        #if __MINGW32__
                            + tr("MinGW32: ") + QString::number(__MINGW32_MAJOR_VERSION) + "." + QString::number(__MINGW32_MINOR_VERSION) + "<br>"
                        #endif
                        #if __MINGW64__
                            + tr("MinGW64: ") + QString::number(__MINGW64_VERSION_MAJOR) + "." + QString::number(__MINGW64_VERSION_MINOR) + "<br>"
                        #endif
                   #endif
                   #if __clang__
                        + tr("Compiler clang: ") + QString::number(__clang_major__) + "." + QString::number(__clang_minor__) + "." + QString::number(__clang_patchlevel__)  + "<br>"
                   #endif
                   #if _MSC_VER
                        + tr("Compiler msvc: ") + QString::number(_MSC_FULL_VER) + " (build:" + QString::number(_MSC_BUILD) + ")<br>"
                   #endif
                   + tr("Project sources: ") + "<a href=\"https://github.com/coozoo/androidprocmon\">https://github.com/coozoo/androidprocmon</a><br><br>");

    QRect widgetRect = ui->centralWidget->geometry();
    msgbox.exec();
    msgbox.move(ui->centralWidget->mapToGlobal(QPoint(widgetRect.center().x()-msgbox.geometry().height()/2,widgetRect.center().y()-msgbox.geometry().width()/2)));


}


void MainWindow::settingsWindow()
{
    SettingsWindow settings_window;
    connect(&settings_window,&SettingsWindow::settings_changed,form,&mainContainer::on_setting_changed);
    settings_window.setWindowTitle("Settings");
    settings_window.setWindowIcon(QIcon(":/images/configuration.png"));
    settings_window.exec();
    disconnect(&settings_window);
}
