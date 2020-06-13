#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include<QToolButton>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();
      QSettings s;

private:
    Ui::SettingsWindow *ui;
signals:
    void settings_changed();
public slots:
    void settingsSave();
    void on_browseadb_toolButton_clicked();
};

#endif // SETTINGSWINDOW_H
