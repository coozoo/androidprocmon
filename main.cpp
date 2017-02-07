/* Author: Yuriy Kuzin
 * genarated by qt just a few string are mine (actually half of strings :) )
 */
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QTextStream cout(stdout);
    QApplication a(argc, argv);
    QTranslator translator;
    cout<<translator.load(a.applicationDirPath()+"/lang/"+a.applicationName()+"_"+QLocale::system().name()+".qm")<<endl;
    cout<<QLocale::system().name()<<endl;
    a.installTranslator(&translator);
    QString platform="";
    Q_UNUSED(platform);
    #if __GNUC__
        #if __x86_64__
            platform="-64bit";
        #endif
    #endif
    a.setProperty("appversion","0.5b" + platform);
    a.setProperty("appname","Android Process Monitor");
    #ifdef Q_OS_LINUX
        a.setWindowIcon(QIcon(":/images/appicon256.png"));
    #endif

    cout<<a.property("appname").toString()<<" "<<a.property("appversion").toString()<<endl;
    MainWindow w;
    w.setWindowTitle(a.property("appname").toString() + " " + a.property("appversion").toString());
    w.show();

    return a.exec();
}
