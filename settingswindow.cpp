#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    connect(this,&SettingsWindow::accepted,this,&SettingsWindow::settingsSave);

    if(s.contains("adbPath"))
    {
        ui->enableCustomadb_checkBox->click();
        ui->adbPath_lineEdit->setText(s.value("adbPath").toString());
    }
    connect(ui->browseadb_toolButton,&QToolButton::click,this,&SettingsWindow::on_browseadb_toolButton_clicked);

}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::settingsSave()
{

    if(ui->enableCustomadb_checkBox->isChecked())
    {
        s.setValue("adbPath",ui->adbPath_lineEdit->text());
    }
    else
    {
        s.remove("adbPath");
    }
    emit settings_changed();
    QMessageBox msgbox;
    msgbox.setIconPixmap(QPixmap(":/images/info.png").scaled(QSize(40, 40)));
    msgbox.setWindowIcon(QPixmap(":/images/info.png").scaled(QSize(40, 40)));
    msgbox.setWindowTitle(tr("Info"));
    msgbox.setText(tr("Saved"));
    QRect widgetRect = this->geometry();
    msgbox.move(this->parentWidget()->mapToGlobal(widgetRect.center()));
    msgbox.exec();

}

void SettingsWindow::on_browseadb_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select adb"), "", tr("Files (adb*);;All Files (*)"));
    ui->adbPath_lineEdit->setText(fileName);
}
