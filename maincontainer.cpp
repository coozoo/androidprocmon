/* Author: Yuriy Kuzin
 */
#include "maincontainer.h"
#include "maincontainerform.h"


mainContainer::mainContainer(QWidget *parent) : QWidget(parent), ui(new Ui::mainContainerForm)
{
    ui->setupUi(this);
    //set datetime stamp for filename
    datetimefile = QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()).toString("dd.MM.yyyy_hh.mm.ss.zzz");

    //read rules for charts from file
    QString configpath = "";
#ifdef Q_OS_WIN
    configpath = qApp->applicationDirPath();
#endif
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    configpath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)[0] + "/" + qAppName();
#endif
    QFile file(configpath + "/chart_rules.json");
    QDir().mkpath(configpath) ;
    if (!QFileInfo::exists(configpath + "/chart_rules.json"))
        {
            QFile::copy(":/chart_rules.json", configpath + "/chart_rules.json");
            QFile(configpath + "/chart_rules.json").setPermissions(QFile::WriteUser | QFile::WriteOwner | QFile::WriteGroup | QFile::ReadUser | QFile::ReadOwner | QFile::ReadGroup);
        }

    file.open(QIODevice::ReadOnly);
    QString jsonRules = file.readAll();
    file.close();
    QTextStream cout(stdout);
    cout << jsonRules << endl;
    QString jsonTopRuleStr;
    QString jsonDumpsysRuleStr;
    QJsonParseError e;
    QJsonDocument jsonDocRules = QJsonDocument::fromJson(jsonRules.toUtf8(), &e);
    if (e.error == QJsonParseError::NoError)
        {
            if (!jsonDocRules.isNull() && !jsonDocRules.isEmpty())
                {
                    jsonTopRuleStr = QJsonDocument::fromVariant(jsonDocRules.object()["toprules"].toObject().toVariantMap()).toJson(QJsonDocument::Compact);
                    jsonDumpsysRuleStr = QJsonDocument::fromVariant(jsonDocRules.object()["dumpsysrules"].toObject().toVariantMap()).toJson(QJsonDocument::Compact);
                    qDebug() << jsonTopRuleStr << jsonDumpsysRuleStr;
                }

        }
    else
        {
            cout << "Error incorrect json chartrules!!! " << e.errorString() << " " << e.offset << endl;
        }

    //create adbviewer object
    adbViewer_cont = new adbViewer(ui->adbviewer_groupBox);
    adbViewer_cont->setstrDateTimeFile(datetimefile);
    ui->adbviewer_groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //create charts manger for top table and connect data slot to signal from adbviwer
    chartsTop = new chartManager(ui->charts_scrollArea->widget());
    chartsTop->setstrDateTimeFile(datetimefile);
    chartsTop->setjsonChartRuleObject(jsonTopRuleStr);
    connect(adbViewer_cont, SIGNAL(processTopStatNewData(QString, QString)), chartsTop, SLOT(dataIncome(QString, QString)));
    //create chart manager for dumpsys table and connect data slot to signal from adbviwer
    chartsDumpsys = new chartManager(ui->chartsDumpsys_scrollArea->widget());
    chartsDumpsys->setstrDateTimeFile(datetimefile);
    chartsDumpsys->setjsonChartRuleObject(jsonDumpsysRuleStr);
    connect(adbViewer_cont, SIGNAL(processDumpsysStatNewData(QString, QString)), chartsDumpsys, SLOT(dataIncome(QString, QString)));

    //QAction* resetAllCharts_toolbutton = new QAction("Reset All Charts",ui->toolbar);
    //resetAllCharts_toolbutton->setToolTip("Reset All Charts");

    //create toolbar and fill with buttons
    resetAllCharts_toolbutton = new QToolButton;
    resetAllCharts_toolbutton->setIcon(QIcon(QPixmap(":/images/error.png")));
    resetAllCharts_toolbutton->setToolTip(tr("Reset All Charts"));
    ui->toolbar->addWidget(resetAllCharts_toolbutton);
    ui->toolbar->addSeparator();
    showhideTop = new QToolButton;
    showhideTop->setIcon(QIcon(QPixmap(":/images/remove.png")));
    showhideTop->setToolTip(tr("Hide Top"));
    showhideTop->setCheckable(true);
    ui->toolbar->addWidget(showhideTop);
    showhideDump = new QToolButton;
    showhideDump->setIcon(QIcon(QPixmap(":/images/remove.png")));
    showhideDump->setToolTip(tr("Hide Dumpsys"));
    showhideDump->setCheckable(true);
    ui->toolbar->addWidget(showhideDump);
    ui->toolbar->addSeparator();
    range_lable = new QLabel();
    range_lable->setText(tr("Range(min): "));
    ui->toolbar->addWidget(range_lable);
    range_spinbox = new QSpinBox();
    range_spinbox->setMinimum(1);
    range_spinbox->setMaximum(100000);
    range_spinbox->setAlignment(Qt::AlignRight);
    range_spinbox->setValue(10);
    ui->toolbar->addWidget(range_spinbox);
    flow_checkbox = new QCheckBox();
    flow_checkbox->setText(tr("Flow"));
    flow_checkbox->setToolTip(tr("After this time data on chart will move out so it will look like a flow"));
    ui->toolbar->addWidget(flow_checkbox);
    ui->toolbar->addSeparator();
    savechartimages_toolbutton = new QToolButton();
    savechartimages_toolbutton->setIcon(QIcon(QPixmap(":/images/disc-floopy.png")));
    savechartimages_toolbutton->setToolTip(tr("Save all charts as png images to the log folder"));
    ui->toolbar->addWidget(savechartimages_toolbutton);
    imageformat_combobox = new QComboBox();
    imageformat_combobox->setToolTip(tr("Choose output format"));
    imageformat_combobox->addItem("png");
    imageformat_combobox->addItem("jpg");
    ui->toolbar->addWidget(imageformat_combobox);
    imagewidth_spinbox = new QSpinBox();
    imagewidth_spinbox->setToolTip(tr("Set image width"));
    imagewidth_spinbox->setMinimum(0);
    imagewidth_spinbox->setMaximum(5000);
    imagewidth_spinbox->setAlignment(Qt::AlignRight);
    imagewidth_spinbox->setValue(1366);
    ui->toolbar->addWidget(imagewidth_spinbox);
    imageheight_spinbox = new QSpinBox();
    imageheight_spinbox->setToolTip(tr("Set image height"));
    imageheight_spinbox->setMinimum(0);
    imageheight_spinbox->setMaximum(5000);
    imageheight_spinbox->setAlignment(Qt::AlignRight);
    imageheight_spinbox->setValue(768);
    ui->toolbar->addWidget(imageheight_spinbox);


    //make connection for each toolbar element
    connect(resetAllCharts_toolbutton, SIGNAL(clicked()), chartsTop, SLOT(resetAllCharts()));
    connect(resetAllCharts_toolbutton, SIGNAL(clicked()), chartsDumpsys, SLOT(resetAllCharts()));
    connect(showhideTop, SIGNAL(clicked()), this, SLOT(on_showhidebuttons_clicked()));
    connect(showhideDump, SIGNAL(clicked()), this, SLOT(on_showhidebuttons_clicked()));
    connect(range_spinbox, SIGNAL(valueChanged(int)), this, SLOT(on_range_spinbox_valueChanged(int)));
    connect(savechartimages_toolbutton, SIGNAL(clicked()), this, SLOT(on_savechartimages_toolbutton_clicked()));

}

mainContainer::~mainContainer()
{
    adbViewer_cont->deleteLater();
    chartsTop->deleteLater();
    chartsDumpsys->deleteLater();
    resetAllCharts_toolbutton->deleteLater();
    showhideTop->deleteLater();
    showhideDump->deleteLater();
    delete ui;
}

/* function that allow to hide one of the chartmanager
 * really it should be better but currently on attempt to hide both
 * it will reaveal both :)
 */
void mainContainer::on_showhidebuttons_clicked()
{
    if (showhideTop->isChecked())
        {
            ui->charts_scrollArea->setHidden(true);
            ui->chartsHeader_lineedit->setHidden(true);
            showhideTop->setToolTip(tr("Show Top"));
        }
    else
        {
            if (ui->chartsDumpsys_scrollArea->isVisible())
                {
                    ui->charts_scrollArea->setHidden(false);
                    ui->chartsHeader_lineedit->setHidden(false);
                    showhideTop->setToolTip(tr("Hide Top"));
                }
            else
                {
                    ui->charts_scrollArea->setHidden(false);
                    ui->chartsHeader_lineedit->setHidden(false);
                    ui->chartsDumpsys_scrollArea->setHidden(false);
                    ui->chartsDumpsysHeader_lineedit->setHidden(false);
                    showhideDump->setChecked(false);
                    showhideTop->setChecked(false);
                    showhideTop->setToolTip(tr("Hide Top"));
                    showhideDump->setToolTip(tr("Hide Dumpsys"));
                }
        }
    if (showhideDump->isChecked())
        {
            if (ui->charts_scrollArea->isVisible())
                {
                    ui->chartsDumpsys_scrollArea->setHidden(true);
                    ui->chartsDumpsysHeader_lineedit->setHidden(true);
                    showhideDump->setToolTip(tr("Show Dumpsys"));
                }
            else
                {
                    ui->chartsDumpsys_scrollArea->setHidden(false);
                    ui->chartsDumpsysHeader_lineedit->setHidden(false);
                    ui->charts_scrollArea->setHidden(false);
                    ui->chartsHeader_lineedit->setHidden(false);
                    showhideTop->setChecked(false);
                    showhideDump->setChecked(false);
                    showhideDump->setToolTip(tr("Hide Dumpsys"));
                    showhideTop->setToolTip(tr("Hide Top"));
                }
        }
    else
        {
            ui->chartsDumpsys_scrollArea->setHidden(false);
            ui->chartsDumpsysHeader_lineedit->setHidden(false);
        }
}

/* slot to coll range changing */
void mainContainer::on_range_spinbox_valueChanged(int range)
{
    //convert to minutes
    chartsTop->setAllRanges(range * 60);
    chartsDumpsys->setAllRanges(range * 60);
}

/* slot to call on image save button */
int mainContainer::on_savechartimages_toolbutton_clicked()
{
    QTextStream cout(stdout);
    QString chartdatetime = QDateTime::fromMSecsSinceEpoch(QDateTime::currentMSecsSinceEpoch()).toString("dd.MM.yyyy_hh.mm.ss.zzz");

    QString chartfoldername = "saved_charts";
#ifdef Q_OS_WIN
    QString rootstatsdir = qApp->applicationDirPath();
    QString statsdirlocation = "/stats/";
#endif
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    QString rootstatsdir = QDir::home().absolutePath();
    QString statsdirlocation = "/androidprocmon/stats/";
#endif


    QString filepath = rootstatsdir + statsdirlocation + datetimefile + "/" + chartfoldername + "/" + chartdatetime + "/";
    if (!QDir(filepath).exists())
        {
            cout << "Creating stats dir: " << filepath << endl;
            if (QDir().mkpath(filepath))
                {
                    cout << "Creating succesfull!" << endl;
                }
            else
                {
                    cout << "unable to create: " << filepath << endl;
                }
        }
    else
        {
            cout << "Directory alredy exists: " << filepath << endl;
        }
    if (!QFile(filepath).exists())
        {
            QTextStream cout(stdout);
            cout << "imposible to create folder" << endl;
        }
    else
        {
            chartsTop->saveAllCharts(filepath + "/top_", imageformat_combobox->currentText(), imagewidth_spinbox->value(), imageheight_spinbox->value());
            chartsDumpsys->saveAllCharts(filepath + "/dumpsys_", imageformat_combobox->currentText(), imagewidth_spinbox->value(), imageheight_spinbox->value());

        }
    //show messagebox that saved (really I'm not checking if it's saved or not :) )
    QMessageBox msgbox;

    msgbox.setIconPixmap(QPixmap(":/images/info.png").scaled(QSize(40, 40)));
    msgbox.setWindowIcon(QPixmap(":/images/info.png").scaled(QSize(40, 40)));
    msgbox.setWindowTitle(tr("Info"));
    msgbox.setText(tr("Saved"));
    QRect widgetRect = this->geometry();
    msgbox.move(this->parentWidget()->mapToGlobal(widgetRect.center()));
    msgbox.exec();

    return 0;

}

void mainContainer::on_setting_changed()
{
    QTextStream cout(stdout);
    cout<<"maincontainersettings"<<endl;
    adbViewer_cont->setadbBinary(adbViewer_cont->whereAdbExists());
}
