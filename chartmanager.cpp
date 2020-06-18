/* Author: Yuriy Kuzin
 */
#include "chartmanager.h"

chartManager::chartManager(QWidget *parent) : QWidget(parent)
{
    main_gridlayout=new QGridLayout();
    parent->setLayout(main_gridlayout);
    // just example that will be used here a lot
    // no loop here because no charts yet :)
    for(int i=0;i<listCharts.count();i++)
    {
       chartRealTime *chart=listCharts.at(i);
       QTextStream cout(stdout);
       cout<<chart;
    }
    //connect if property for json rules changed
    connect(this,SIGNAL(jsonChartRuleObjectChanged()),this,SLOT(on_jsonChartRuleObjectChanged()));
}

chartManager::~chartManager()
{
    for(int i=0;i<listCharts.count();i++)
    {
       chartRealTime *chart=listCharts.at(i);
       //delete chart objects
       if(chart!=NULL)
       {
       chart->deleteLater();
       //delete mainwindow
       chart->parentWidget()->deleteLater();
       }
    }
}

/* slot called when setting graph rules property
 * currently works only on adding graphs so no deletion
 * if you want change amount of graph by changing json
 * then you need to recreate object
 */
void chartManager::on_jsonChartRuleObjectChanged()
{
    QJsonDocument jsonRuleDoc=QJsonDocument::fromJson(getjsonChartRuleObject().toUtf8());
    QJsonObject jsonRuleObj=jsonRuleDoc.object();
    QJsonArray jsonRuleArr=jsonRuleObj["charts"].toArray();
    qDebug()<<jsonRuleArr;
    int i=0;
    int startrow=0;
    int numberinrow=2;
    //add charts
    foreach (const QJsonValue & chartItem, jsonRuleArr)
    {
        qDebug()<<"chartItem"<<chartItem;
        //place each chart into mainwindow
        QMainWindow *chart_mainwindow=new QMainWindow(this);
        chart_mainwindow->installEventFilter(this);
        chart_mainwindow->setAnimated(true);
        chart_mainwindow->setDockNestingEnabled(true);
        chartRealTime *chart=new chartRealTime(chart_mainwindow);
        chart->setParent(chart_mainwindow);
        chart->setRange(600);
        //when chart is undocked then hide mainwindow
        connect(chart,SIGNAL(dockLocationChanged(bool)),chart_mainwindow,SLOT(setHidden(bool)));
        //place all charts into grid
        if(i<numberinrow)
        {
            main_gridlayout->addWidget(chart_mainwindow,startrow,i);
        }
        else
        {
            if(i%numberinrow==0)
            {startrow++;}
            main_gridlayout->addWidget(chart_mainwindow,startrow,i%numberinrow);
        }
        chart_mainwindow->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        chart_mainwindow->setMinimumHeight(200);
        chart_mainwindow->setMinimumWidth(150);

        //set chart parameters from json
        chart->setWindowTitle(chartItem.toObject()["chart"].toString());
        if(!chartItem.toObject()["vaxislabel"].isNull() && chartItem.toObject()["vaxislabel"].isString() && chartItem.toObject()["vaxislabel"].toString()!="")
        {
            chart->setYAxisLabel(chartItem.toObject()["vaxislabel"].toString());
        }
        if(!chartItem.toObject()["legend"].isNull() && chartItem.toObject()["legend"].isString() && chartItem.toObject()["legend"].toString()!="")
        {
            chart->setPlotTitle(chartItem.toObject()["legend"].toString());
        }
//        if(!chartItem.toObject()["description"].isNull() && chartItem.toObject()["description"].isString() && chartItem.toObject()["description"].toString()!="")
//        {
//            chart->setToolTip(chartItem.toObject()["description"].toString());
//        }

        // add graphs (lines) to chart
        QStringList graphs=QStringList();
        QJsonArray jsonGraphsArr=chartItem.toObject()["graphs"].toArray();
        foreach (const QJsonValue & graphItem, jsonGraphsArr)
        {
            qDebug()<<"graphItem"<<graphItem;
            graphs.append(graphItem.toObject()["name"].toString());
        }
        chart->addGraphs(graphs);
        listCharts.append(chart);
        i++;
    }

    //just for debug
    for(int i=0;i<listCharts.count();i++)
    {
       chartRealTime *chart=listCharts.at(i);
       QTextStream cout(stdout);
       cout<<chart->getWindowTitle()<<endl;
    }

}

/* slot on data come
 * headersString - list of headers
 * dataString - values for Process
 * headersString and dataString elements count should be identical
 */
void chartManager::dataIncome(QString headersString, QString dataString)
{

    QJsonDocument jsonRuleDoc=QJsonDocument::fromJson(getjsonChartRuleObject().toUtf8());
    QJsonObject jsonRuleObj=jsonRuleDoc.object();
    QJsonArray jsonRuleArr=jsonRuleObj["charts"].toArray();
    // very stupid attempt to sum and multiply items
    // it works on simple conditions but it really should be changed
    foreach (const QJsonValue & chartItem, jsonRuleArr)
    {
        qDebug()<<"chartItem"<<chartItem;
        //chartItem.toObject()["chart"].toString()
        QStringList graphs=QStringList();
        QJsonArray jsonGraphsArr=chartItem.toObject()["graphs"].toArray();
        QList<int> dataset;
        foreach (const QJsonValue & graphItem, jsonGraphsArr)
        {
            qDebug()<<"graphItem"<<graphItem;
            QString graphrule=graphItem.toObject()["rule"].toString();
            if(graphrule.contains("\\/") || graphrule.contains("\\+"))
            {
                int calcresult=0;
                if(graphrule.contains("\\/"))
                {
                QStringList divPartsList=graphrule.split("\\/");
                 foreach (QString divstr, divPartsList)
                {
                    bool ok;
                    int divel = divstr.toInt(&ok);
                    if (ok)
                    {
                        if(calcresult!=0)
                        {
                            calcresult=calcresult/divel;
                        }
                        else
                        {
                            calcresult=divel;
                        }
                    }
                    else
                    {
                        int sumresult=0;
                        if(divstr.contains("\\+"))
                        {
                            QStringList sumPartsList=graphrule.split("\\+");
                            foreach (QString sumstr, sumPartsList)
                            {
                                bool ok;
                                int sumel = sumstr.toInt(&ok);
                                if (ok)
                                {
                                    sumresult=sumresult+sumel;
                                }
                                else
                                {
                                    sumresult=sumresult+getValueByHeader(sumstr,headersString,dataString);
                                }
                            }

                        }
                        else
                        {
                            if(calcresult!=0)
                            {
                            calcresult=calcresult/getValueByHeader(divstr,headersString,dataString);
                            }
                            else
                            {
                                calcresult=getValueByHeader(divstr,headersString,dataString);
                            }
                        }
                    }


                }
                }
                else if(!graphrule.contains("\\/") && graphrule.contains("\\+"))
                {
                    QStringList sumPartsList=graphrule.split("\\+");
                    foreach (QString sumstr, sumPartsList)
                    {
                        bool ok;
                        int sumel = sumstr.toInt(&ok);
                        if (ok)
                        {
                            calcresult=calcresult+sumel;
                        }
                        else
                        {
                            calcresult=calcresult+getValueByHeader(sumstr,headersString,dataString);
                        }
                    }
                }

                dataset.append(calcresult);
            }

            else
            {
                dataset.append(getValueByHeader(graphrule,headersString,dataString));
            }
        }
        for(int i=0;i<listCharts.count();i++)
        {
                  chartRealTime *chart=listCharts.at(i);
                  if(chart->getWindowTitle()==chartItem.toObject()["chart"].toString())
                  {
                    chart->dataSlot(QDateTime::fromString(dataString.split(",")[0]),dataset);
                  }
        }

    }
}

/* Function to get value from commaseparated strings by header from another string
 * header - name of header
 * headersString - string with commaseparated headers
 * dataString - string with commaseparated values
 * return: id  (-1 if not found)
 */
int chartManager::getValueByHeader(QString header, QString headersString, QString dataString)
{
    QStringList headersStringList=headersString.split(",");
    QStringList dataStringList=dataString.split(",");
    if(header.contains("\\|"))
    {
        foreach (QString str, header.split("\\|"))
        {
            if(headersStringList.contains(str))
            {
                header=str;
                break;
            }
        }

    }
    QTextStream cout(stdout);
    cout<<"IIIIIIIIIIIIIIIIIIIIIII"<<endl;
    cout<<header<<endl;
    qDebug()<<headersStringList;
    qDebug()<<dataStringList;
    cout<<headersStringList.count()<<"  "<<dataStringList.count()<<endl;
    cout<<"IIIIIIIIIIIIIIIIIIIIIII"<<endl;
    if(headersStringList.count()==dataStringList.count())
    {
    for(int i=0;i<headersStringList.count();i++)
    {
        if(headersStringList[i]==header)
        {
            return dataStringList[i].toInt();
        }
    }
    }
        else
    {QTextStream cout(stdout);
        cout<<"impossible to find: length does not matched";
        return -1;}
        cout<<"not Found";
        return -1;
}

/* function to reset all charts
 * it simply call reset functio for each chart
 */
void chartManager::resetAllCharts()
{
    for(int i=0;i<listCharts.count();i++)
    {
       chartRealTime *chart=listCharts.at(i);
       chart->reset();
    }
}

/* function set range of all charts
 * it simply call set range function for each chart
 */
void chartManager::setAllRanges(int range)
{
    for(int i=0;i<listCharts.count();i++)
    {
       chartRealTime *chart=listCharts.at(i);
       chart->setRange(range);
    }
}

/* function to save all charts as images
 * imagePath - path where to save image (image name not included it's generated inside chart based on name of chart)
 * imageType - imagetype png,jpg (bmp, pdf not supported yet)
 * width - image width
 * height - image height
 */
void chartManager::saveAllCharts(QString imagePath, QString imageType, int width, int height)
{
    for(int i=0;i<listCharts.count();i++)
    {
       chartRealTime *chart=listCharts.at(i);
       chart->saveImage(imagePath,imageType,width,height);
    }

}

bool chartManager::eventFilter(QObject* obj, QEvent *event)
{
    QTextStream cout(stdout);
    if (event->type() == QEvent::MouseMove)
      {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        cout<<QString("Chart main window mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y())<<endl;
      }
      return false;
}


