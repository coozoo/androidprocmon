/* Author: Yuriy Kuzin
 * sure it's based on example provided by qcustomplot
 * so it can contain some odd strings :)
 */
#include "chartrealtime.h"

chartRealTime::chartRealTime(QMainWindow *parent) : QMainWindow(parent)
{
    timeReset=true;

    main_gridlayout=new QGridLayout();
    //create dock to place chart over it
    chart_dockwidget=new QDockWidget(parent);
    parent->addDockWidget(Qt::RightDockWidgetArea, chart_dockwidget);

    //create chart object
    customPlot=new QCustomPlot(parent);
    customPlot->setMaximumSize(std::numeric_limits<int>::max(),std::numeric_limits<int>::max());
    //reset button and hide it :) I don't nneed it for each chart
//    QPushButton *resetButton=new QPushButton();
//    resetButton->setVisible(false);
//    resetButton->setText("Reset");
//    main_gridlayout->addWidget(resetButton,1,0);
//    connect(resetButton,SIGNAL(clicked()),this,SLOT(reset()));

    chart_dockwidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    chart_dockwidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    chart_dockwidget->installEventFilter(this);


    chart_dockwidget->setWidget(customPlot);

    //set init data of chart
    setupRealtimeDataDemo(customPlot);
    parent->setLayout(main_gridlayout);

    //connect visibility signal for replotting
    connect(chart_dockwidget,SIGNAL(visibilityChanged(bool)),this,SLOT(on_visibilityChanged(bool)));
    //connect when dock undocked
    connect(chart_dockwidget,SIGNAL(topLevelChanged(bool)),this,SLOT(on_dockLocationChanged(bool)));


}

chartRealTime::~chartRealTime()
{
    chart_dockwidget->deleteLater();
    customPlot->deleteLater();

}

/* slot to emit signal when dock is undocked
 *
 */
void chartRealTime::on_dockLocationChanged(bool state)
{
    QTextStream cout(stdout);
    //if(chart_dockwidget->isFloating())
    //{
    //    chart_dockwidget->setFloating(true);
    //}
    cout<<"pos changed: "<<state<<endl;

    emit dockLocationChanged(state);

}

/* function to reset chart
 * set init range and reset timer (actually global bool that timer should be resetted)
 */
void chartRealTime::reset()
{

    customPlot->graph()->data()->clear();
    customPlot->yAxis->setRange(0, 30);
    customPlot->xAxis->setRange(0, getRange(), Qt::AlignRight);
    timeReset=true;
    if(customPlot->isVisible())
    {
    customPlot->replot();
    }
}


/* function to setup different init parameters */
void chartRealTime::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
    demoName = "Real Time Data Demo";

    // include this section to fully disable antialiasing for higher performance:
    /*
    customPlot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    customPlot->xAxis->setTickLabelFont(font);
    customPlot->yAxis->setTickLabelFont(font);
    customPlot->legend->setFont(font);
    */
    //customPlot->setMinimumSize(300,300);
    customPlot->xAxis->setTickLabelFont(QFont("Helvetica", 7));
    customPlot->yAxis->setTickLabelFont(QFont("Helvetica", 7));
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 7));
    customPlot->legend->setBrush(QColor(255, 255, 255, 150));


//    customPlot->addGraph(); // blue line
//    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    //customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    //customPlot->graph()->setBrush(QBrush(QColor(255,200,20,70)));
//    customPlot->addGraph(); // red line
//    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(0, 30);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    //show lable for mouse position
    connect(customPlot, SIGNAL(mouseMove(QMouseEvent*)), this,SLOT(showPointToolTip(QMouseEvent*)));
    // datacoming signal to the slot
    connect(this,SIGNAL(datacoming(QDateTime,QList<int>)),this,SLOT(on_datacoming(QDateTime,QList<int>)));



    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    //connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    //dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

//void chartRealTime::realtimeDataSlot()
//{
//    static QTime time(QTime::currentTime());
//    // calculate two new data points:
//    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
//    static double lastPointKey = 0;
//    //if (key-lastPointKey > 0.002) // at most add point every 2 ms
//    if (key-lastPointKey > 5)
//        {
//            // add data to lines:
//            customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
//            customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
//            // rescale value (vertical) axis to fit the current data:
//            customPlot->graph(0)->rescaleValueAxis();
//            customPlot->graph(1)->rescaleValueAxis(true);
//            lastPointKey = key;
//            if(key>=getRange())
//                {
//                    customPlot->xAxis->setRange(key, key, Qt::AlignRight);
//                }
//            else
//                {
//                    customPlot->xAxis->setRange(key, getRange(), Qt::AlignRight);
//                }

//            customPlot->replot();
//        }
//}

/* slot to set window title */
void chartRealTime::on_setWindowTitle()
{
    chart_dockwidget->setWindowTitle(getWindowTitle());
    chart_dockwidget->setObjectName(getWindowTitle());
}
/* slot to add vertical lable */
void chartRealTime::setYAxisLabel(QString yAxisLabel)
{
    customPlot->yAxis->setLabel(yAxisLabel);
}

/* function to set plot title on top of plot
 * plotTitle - yes it's title of plot :)
 */
void chartRealTime::setPlotTitle(QString plotTitle)
{
    QFont font("Helvetica", 9);
    font.setBold(true);
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, plotTitle,font));
}

/* slot to add value onto plot
 * dateTime - not used currently (in future maybe will be posibility to use it, currently only internal chart timer)
 * data - values list should match with graph values amount
 */
void chartRealTime::on_datacoming(QDateTime dateTime, QList<int> data)
{
    Q_UNUSED(dateTime);
    //static QTime time(QTime::currentTime());
    //if bool flag to reset timer is raised
    if(timeReset==true)
    {
        time=QTime::currentTime();
        timeReset=false;
    }
    double key = time.elapsed()/1000.0;
    //add each value to graph
    int i=0;
    foreach (int value, data)
        {
            customPlot->graph(i)->addData(key, value);
            if(customPlot->yAxis->range().upper<value)
                {
                    customPlot->yAxis->setRange(0,value+((value*10)/100));
                }
            i++;
        }
    //range flow if less
    //if more then range than "compress" it
    //later I want to add scroll type (flow like when old data moves to left side and lost when our of range)
    int currentRange=getRange();
    if(key>=currentRange)
        {
            customPlot->xAxis->setRange(key, key, Qt::AlignRight);
        }
    else
        {
            customPlot->xAxis->setRange(key, currentRange, Qt::AlignRight);
        }
    if(customPlot->isVisible())
    {
        customPlot->replot();
    }

}

/* dataslot but really function to emit signal that data coming
 * dateTime - not used currently (in future maybe will be posibility to use it, currently only internal chart timer)
 * data - values list should match with graph values amount
 */
void chartRealTime::dataSlot(QDateTime dateTime, QList<int> data)
{
    emit datacoming(dateTime,data);
}


/* Function to add graphs onto plot
 * graphs - name of lines
 *          if contains [ ] make it shaded at bottom
 */
void chartRealTime::addGraphs(QStringList graphs)
{
    //graphs.append("test");
    qDebug()<<graphs<<endl;
    int i=0;

    foreach (QString str, graphs)
        {
            customPlot->addGraph(); // blue line
            customPlot->graph(i)->setLineStyle(QCPGraph::lsLine);


            if(i<uniColor.count())
            {

                customPlot->graph(i)->setPen(QPen(uniColor[i],2));
                if(str.contains("[") && str.contains("]"))
                    {
                        customPlot->graph(i)->setBrush(QBrush(uniColor[i]));
                        QColor test;
                        test.operator =(uniColor[i]);
                        test.setAlpha(70);
                                customPlot->graph(i)->setBrush(QBrush(test));
                    }
            }
            else
            {
                customPlot->graph(i)->setPen(QPen(uniColor[i%uniColor.count()],2));
                if(str.contains("[") && str.contains("]"))
                    {
                        //customPlot->graph(i)->setBrush(QBrush(QColor(255,200,20,70)));
                    QColor test;
                    test.operator =(uniColor[i%uniColor.count()]);
                    test.setAlpha(70);
                            customPlot->graph(i)->setBrush(QBrush(test));
                    }
            }
//            if(str.contains("[") && str.contains("]"))
//                {
//                    customPlot->graph(i)->setBrush(QBrush(QColor(255,200,20,70)));
//                }
            customPlot->graph(i)->setName(str);
            i++;
        }

}

/* replot if chart become visible */
void chartRealTime::on_visibilityChanged(bool)
{
    QTextStream cout(stdout);
    cout<<"vis changed"<<endl;
    if(customPlot->isVisible())
    {
    customPlot->replot();
    }
}


/* show lable with current position over plot */
void chartRealTime::showPointToolTip(QMouseEvent *event)
{

    //int x = customPlot->xAxis->pixelToCoord(event->pos().x());
    int y = customPlot->yAxis->pixelToCoord(event->pos().y());

    //customPlot->setToolTip(QString("%1 , %2").arg(x).arg(y));
    customPlot->setToolTip(QString("%1").arg(y));

}

void chartRealTime::on_setRange()
{
    //something to handle  range change (need something really smart to make it work smoothly :) )

}

/* function to save plot as images
 * imagePath - path where to save image (image name not included it's generated inside chart based on name of chart)
 * imageType - imagetype png,jpg (bmp, pdf not supported yet)
 * width - image width
 * height - image height
 */
void chartRealTime::saveImage(QString imagePath, QString imageType, int width,int height)
{
    //replace charachters which posibly incorrect for filename in chart name with underscore
    QString chartname=chart_dockwidget->windowTitle()+"_"+QString::number(time.elapsed(),'g',10);
    QStringList forbidCharachters={" ","<",">",":","\"","/","\\","|","&","*","@","#","$","%","^","\"","+","=","`"};
    foreach(QString stringtoreplace, forbidCharachters)
    {
        chartname=chartname.replace(stringtoreplace,"_");
    }
    if(imageType.toLower()=="png")
    {
        customPlot->savePng(imagePath + chartname +".png",width,height,1.0,-1);
    }
    else if(imageType.toLower()=="jpg")
    {
        customPlot->saveJpg(imagePath + chartname+".jpg",width,height,1.0,-1);
    }
//    else if(imageType.toLower()=="bmp")
//    {
//        customPlot->saveRastered(imagePath + chartname+".bmp",height,768,1.0);
//    }
//    else if(imageType.toLower()=="pdf")
//    {
//        customPlot->savePdf(imagePath + chartname+".pdf",width,height);
//    }
}

bool chartRealTime::eventFilter(QObject* obj, QEvent *event)
{
    QTextStream cout(stdout);
    if (event->type() == QEvent::MouseMove)
      {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        cout<<QString("Chart dock mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y())<<endl;
      }
      return false;
}

