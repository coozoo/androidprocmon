/* Author: Yuriy Kuzin
 * genarated by qt just a few string are mine (actually half of strings :) )
 */
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

const QString APP_VERSION = "0.5b";

int main(int argc, char *argv[])
{
    QTextStream cout(stdout);
    QApplication a(argc, argv);
    cout << QLocale::system().name() << endl;
    QStringList translations;
    QDir dir(a.applicationDirPath());
    if (dir.cdUp() && dir.cd("share"))
        {
            translations.append(dir.absolutePath() + "/" + a.applicationName());
        }
    translations.append(QStandardPaths::standardLocations(QStandardPaths::DataLocation));
    translations.append(QCoreApplication::applicationDirPath());
    translations.append(a.applicationDirPath() + "/.qm");
    translations.append(a.applicationDirPath() + "/lang");
    QString translationFilePath = "";
    cout << "Search for translations" << endl;
    foreach (const QString &str, translations)
        {
            QFileInfo fileinfo(str + "/" + a.applicationName() + "_" + QLocale::system().name() + ".qm");
            cout << fileinfo.filePath() << endl;
            if (fileinfo.exists() && fileinfo.isFile())
                {
                    translationFilePath = fileinfo.filePath();
                    cout << "Translation found in: " + translationFilePath << endl;
                    break;
                }
        }

    QTranslator translator;
    cout << translator.load(translationFilePath) << endl;
    a.installTranslator(&translator);
    QString platform = "";
    Q_UNUSED(platform)
#if __GNUC__
#if __x86_64__
    platform = "-64bit";
#endif
#endif
    a.setProperty("appversion", APP_VERSION + platform);
    a.setProperty("appname", "Android Process Monitor");
#ifdef Q_OS_LINUX
    a.setWindowIcon(QIcon(":/images/appicon256.png"));
#endif

    cout << a.property("appname").toString() << " " << a.property("appversion").toString() << endl;
    MainWindow w;
    w.setWindowTitle(a.property("appname").toString() + " " + a.property("appversion").toString());
    w.show();

    return a.exec();
}
