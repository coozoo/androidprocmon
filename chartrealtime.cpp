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


    // Add vertical cursor line and label
    cursorLine = new QCPItemStraightLine(customPlot);
    cursorLine->setPen(QPen(Qt::DashLine));
    cursorLine->point1->setCoords(0, 0);
    cursorLine->point2->setCoords(0, 1);

    cursorLabel = new QCPItemText(customPlot);
    cursorLabel->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);
    cursorLabel->setText("");
    cursorLabel->setVisible(false);
    cursorLabel->setPen(QPen(Qt::black));
    cursorLabel->setBrush(QBrush(QColor(255,255,200,200)));
    customPlot->addLayer("overlay", customPlot->layer("main"), QCustomPlot::limAbove);
    cursorLabel->setLayer("overlay");
    cursorLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
    cursorLabel->setTextAlignment(Qt::AlignLeft);

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
    //if(chart_dockwidget->isFloating())
    //{
    //    chart_dockwidget->setFloating(true);
    //}
    qDebug()<<"pos changed: "<<state;

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
    connect(this,SIGNAL(datacoming(QDateTime,QList<double>)),this,SLOT(on_datacoming(QDateTime,QList<double>)));



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
void chartRealTime::on_datacoming(QDateTime dateTime, QList<double> data)
{
    Q_UNUSED(dateTime)
    //if bool flag to reset timer is raised
    if(timeReset==true)
    {
        timer.restart();
        timeReset=false;
    }

    double key = static_cast<double>(timer.elapsed())/1000.0;
    qDebug()<<getisflow()<<key;
    //add each value to graph
    double i=0;
    foreach (double value, data)
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
    if(key>=currentRange && !getisflow())
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
void chartRealTime::dataSlot(QDateTime dateTime, QList<double> data)
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
    qDebug()<<graphs;
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

            QCPItemTracer* tracer = new QCPItemTracer(customPlot);
            tracer->setStyle(QCPItemTracer::tsCircle);
            tracer->setPen(QPen(uniColor[i % uniColor.count()], 2));
            tracer->setBrush(Qt::yellow);
            tracer->setSize(12);
            tracer->setVisible(false);
            tracer->setLayer("overlay");
            tracers.append(tracer);

            i++;
        }

}

/* replot if chart become visible */
void chartRealTime::on_visibilityChanged(bool)
{
    qDebug()<<"vis changed";
    if(customPlot->isVisible())
    {
    customPlot->replot();
    }
}


/* show lable with current position over plot */
void chartRealTime::showPointToolTip(QMouseEvent *event)
{

    // //int x = customPlot->xAxis->pixelToCoord(event->pos().x());
    // int y = customPlot->yAxis->pixelToCoord(event->pos().y());

    // //customPlot->setToolTip(QString("%1 , %2").arg(x).arg(y));
    // customPlot->setToolTip(QString("%1").arg(y));

    QRect plotRect = customPlot->axisRect()->rect();
    QPoint mousePos = event->pos();

    QPoint labelOffset(-110, -50);
    int labelWidth = 120;
    int labelHeight = 40;
    if (mousePos.x() - labelWidth < plotRect.left())
        labelOffset.setX(40);
    if (mousePos.y() - labelHeight < plotRect.top())
        labelOffset.setY(20);

    QPoint labelPixelPos = mousePos + labelOffset;
    double labelX = customPlot->xAxis->pixelToCoord(labelPixelPos.x());
    double labelY = customPlot->yAxis->pixelToCoord(labelPixelPos.y());

    double mouseX = customPlot->xAxis->pixelToCoord(event->pos().x());
    cursorLine->point1->setCoords(mouseX, 0);
    cursorLine->point2->setCoords(mouseX, 1);

    QString xLabel = customPlot->xAxis->label();

    // We'll track the nearest dot of the first graph with data for the label's time
    double traceTime = 0;
    bool foundTraceTime = false;
    QString labelText;

    int tracerIndex = 0;
    bool anyData = false;

    // Track index of the closest data point for the first graph with data
    static int lastDataIndex = -1;
    int foundDataIndex = -1;

    for (int i = 0; i < customPlot->graphCount(); ++i)
    {
        QCPGraph *graph = customPlot->graph(i);
        double minDist = std::numeric_limits<double>::max();
        double nearestX = 0, nearestY = 0;
        bool foundData = false;
        int nearestIndex = -1, idx = 0;

        if (graph && graph->data()->size() > 0)
        {
            for (auto it = graph->data()->constBegin(); it != graph->data()->constEnd(); ++it, ++idx)
            {
                double dist = std::abs(it->key - mouseX);
                if (dist < minDist)
                {
                    minDist = dist;
                    nearestX = it->key;
                    nearestY = it->value;
                    foundData = true;
                    nearestIndex = idx;
                }
            }

            if (foundData) {
                anyData = true;
                // Record time and index for label from the first graph with data
                if (!foundTraceTime) {
                    traceTime = nearestX;
                    foundTraceTime = true;
                    foundDataIndex = nearestIndex;
                }
                // Show a tracer for each graph
                if (tracers.size() > tracerIndex && tracers[tracerIndex]) {
                    tracers[tracerIndex]->setGraph(graph);
                    tracers[tracerIndex]->setGraphKey(nearestX);
                    tracers[tracerIndex]->setInterpolating(false);
                    tracers[tracerIndex]->setVisible(true);
                }
                labelText += QString("%1: %2\n")
                                 .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name())
                                 .arg(QString::number(nearestY, 'g', 6));
            } else {
                if (tracers.size() > tracerIndex && tracers[tracerIndex])
                    tracers[tracerIndex]->setVisible(false);
                labelText += QString("%1: ---\n")
                                 .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name());
            }
        }
        else
        {
            if (tracers.size() > tracerIndex && tracers[tracerIndex])
                tracers[tracerIndex]->setVisible(false);
            labelText += QString("%1: ---\n")
                             .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name());
        }
        tracerIndex++;
    }

    // Set time in label to the time of actual dot, not mouse
    QString timeStr;
    if (xLabel.toLower().contains("time") || xLabel.toLower().contains("date"))
        timeStr = QDateTime::fromMSecsSinceEpoch((qint64)traceTime).toString("yyyy-MM-dd HH:mm:ss");
    else
        timeStr = QString::number(traceTime, 'f', 2);

    QString header = QString("%1: %2\n").arg(xLabel.isEmpty() ? "Time" : xLabel).arg(timeStr);
    labelText = header + labelText;

    // Only use cursorLabel if it exists!
    if (cursorLabel) {
        cursorLabel->setText(labelText.trimmed());
        cursorLabel->setVisible(anyData);
        cursorLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        cursorLabel->setTextAlignment(Qt::AlignLeft);
        if (anyData)
            cursorLabel->position->setCoords(labelX, labelY);
    }

    // Only emit signal when the data point index actually changes
    if (foundTraceTime && foundDataIndex != -1 && foundDataIndex != lastDataIndex) {
        emit tracerTimeChanged((qint64)traceTime);
        emit tracerIndexChanged(foundDataIndex);
        lastDataIndex = foundDataIndex;
        qDebug()<<"tracer datapoint changed index:"<<foundDataIndex<<" time:"<<traceTime;
    }

    customPlot->replot();


}


void chartRealTime::on_setRange()
{
    qDebug()<<"on_setRange"<<getRange();
    //something to handle  range change (need something really smart to make it work smoothly :) )

}

void chartRealTime::on_setisflow()
{
    qDebug()<<"on_setisflow"<<getisflow();
    //something to handle  flow change

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
    QString chartname=chart_dockwidget->windowTitle()+"_"+QString::number(static_cast<double>(timer.elapsed()),'g',10);
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
    Q_UNUSED(obj)
    if (event->type() == QEvent::MouseMove)
      {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        qDebug()<<QString("Chart dock mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y());
      }
      return false;
}


// void chartRealTime::showTracerAtTime(qint64 time)
// {
//     // Find nearest dot to time on each graph
//     int tracerIndex = 0;
//     double labelX = 0, labelY = 0;
//     bool found = false;
//     QString labelText;
//     QString xLabel = customPlot->xAxis->label();

//     for (int i = 0; i < customPlot->graphCount(); ++i)
//     {
//         QCPGraph *graph = customPlot->graph(i);
//         double minDist = std::numeric_limits<double>::max();
//         double nearestX = 0, nearestY = 0;
//         bool foundData = false;
//         if (graph && graph->data()->size() > 0)
//         {
//             for (auto it = graph->data()->constBegin(); it != graph->data()->constEnd(); ++it)
//             {
//                 double dist = std::abs(it->key - time);
//                 if (dist < minDist)
//                 {
//                     minDist = dist;
//                     nearestX = it->key;
//                     nearestY = it->value;
//                     foundData = true;
//                 }
//             }
//             if (foundData && tracers.size() > tracerIndex && tracers[tracerIndex]) {
//                 tracers[tracerIndex]->setGraph(graph);
//                 tracers[tracerIndex]->setGraphKey(nearestX);
//                 tracers[tracerIndex]->setInterpolating(false);
//                 tracers[tracerIndex]->setVisible(true);
//                 if (!found) {
//                     labelX = nearestX;
//                     labelY = nearestY;
//                     found = true;
//                 }
//                 labelText += QString("%1: %2\n")
//                                  .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name())
//                                  .arg(QString::number(nearestY, 'g', 6));
//             }
//             else if (tracers.size() > tracerIndex && tracers[tracerIndex]) {
//                 tracers[tracerIndex]->setVisible(false);
//                 labelText += QString("%1: ---\n")
//                                  .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name());
//             }
//         }
//         tracerIndex++;
//     }
//     // Show label beside tracer dot
//     if (cursorLabel && found) {
//         QString timeStr;
//         if (xLabel.toLower().contains("time") || xLabel.toLower().contains("date"))
//             timeStr = QDateTime::fromMSecsSinceEpoch((qint64)labelX).toString("yyyy-MM-dd HH:mm:ss");
//         else
//             timeStr = QString::number(labelX, 'f', 2);
//         QString header = QString("%1: %2\n").arg(xLabel.isEmpty() ? "Time" : xLabel).arg(timeStr);
//         labelText = header + labelText;
//         cursorLabel->setText(labelText.trimmed());
//         cursorLabel->setVisible(true);
//         cursorLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
//         cursorLabel->setTextAlignment(Qt::AlignLeft);
//         cursorLabel->position->setCoords(labelX, labelY);
//     } else if (cursorLabel) {
//         cursorLabel->setVisible(false);
//     }
//     customPlot->replot();
// }

//usually shows a bit off
void chartRealTime::showTracerAtTime(qint64 time)
{
    int tracerIndex = 0;
    double labelX = 0, labelY = 0;
    bool found = false;
    QString labelText;
    QString xLabel = customPlot->xAxis->label();

    // Track index of the nearest data point per graph
    QVector<int> nearestIndices;

    for (int i = 0; i < customPlot->graphCount(); ++i)
    {
        QCPGraph *graph = customPlot->graph(i);
        double minDist = std::numeric_limits<double>::max();
        double nearestX = 0, nearestY = 0;
        bool foundData = false;
        int nearestIndex = -1, idx = 0;

        if (graph && graph->data()->size() > 0)
        {
            for (auto it = graph->data()->constBegin(); it != graph->data()->constEnd(); ++it, ++idx)
            {
                double dist = std::abs(it->key - time);
                if (dist < minDist)
                {
                    minDist = dist;
                    nearestX = it->key;
                    nearestY = it->value;
                    foundData = true;
                    nearestIndex = idx;
                }
            }
            nearestIndices.append(nearestIndex);

            if (foundData && tracers.size() > tracerIndex && tracers[tracerIndex]) {
                tracers[tracerIndex]->setGraph(graph);
                tracers[tracerIndex]->setGraphKey(nearestX);
                tracers[tracerIndex]->setInterpolating(false);
                tracers[tracerIndex]->setVisible(true);
                if (!found) {
                    labelX = nearestX;
                    labelY = nearestY;
                    found = true;
                }
                labelText += QString("%1: %2 (idx %3)\n")
                                 .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name())
                                 .arg(QString::number(nearestY, 'g', 6))
                                 .arg(nearestIndex);
            }
            else if (tracers.size() > tracerIndex && tracers[tracerIndex]) {
                tracers[tracerIndex]->setVisible(false);
                labelText += QString("%1: ---\n")
                                 .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name());
            }
        } else {
            nearestIndices.append(-1); // No data available for this graph
            if (tracers.size() > tracerIndex && tracers[tracerIndex])
                tracers[tracerIndex]->setVisible(false);
            labelText += QString("%1: ---\n")
                             .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name());
        }
        tracerIndex++;
    }

    // Show label beside tracer dot with offset and edge checking
    if (cursorLabel && found) {
        QString timeStr;
        if (xLabel.toLower().contains("time") || xLabel.toLower().contains("date"))
            timeStr = QDateTime::fromMSecsSinceEpoch((qint64)labelX).toString("yyyy-MM-dd HH:mm:ss");
        else
            timeStr = QString::number(labelX, 'f', 2);
        QString header = QString("%1: %2\n").arg(xLabel.isEmpty() ? "Time" : xLabel).arg(timeStr);
        labelText = header + labelText;
        cursorLabel->setText(labelText.trimmed());
        cursorLabel->setVisible(true);
        cursorLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        cursorLabel->setTextAlignment(Qt::AlignLeft);

        // Offset logic: get pixel coordinates for tracer dot
        QRect plotRect = customPlot->axisRect()->rect();
        int tracerPixelX = customPlot->xAxis->coordToPixel(labelX);
        int tracerPixelY = customPlot->yAxis->coordToPixel(labelY);

        QPoint labelOffset(-110, -50); // Same as your mouse offset
        int labelWidth = 120;
        int labelHeight = 40;
        if (tracerPixelX - labelWidth < plotRect.left())
            labelOffset.setX(40);
        if (tracerPixelY - labelHeight < plotRect.top())
            labelOffset.setY(20);

        QPoint labelPixelPos = QPoint(tracerPixelX, tracerPixelY) + labelOffset;
        double labelCoordX = customPlot->xAxis->pixelToCoord(labelPixelPos.x());
        double labelCoordY = customPlot->yAxis->pixelToCoord(labelPixelPos.y());

        cursorLabel->position->setCoords(labelCoordX, labelCoordY);
    } else if (cursorLabel) {
        cursorLabel->setVisible(false);
    }

    // Optionally: emit a signal with nearestIndices (e.g., for sync)
    // emit tracerIndicesChanged(nearestIndices); // You need to declare this signal if you want to use it

    customPlot->replot();
}


void chartRealTime::showTracerAtIndex(int index)
{
    qDebug()<<"showTracerAtIndex"<<index;
    if (customPlot->graphCount() == 0) return;

    QString labelText;
    QString xLabel = customPlot->xAxis->label();
    QString timeStr;
    double labelX = 0, labelY = 0;
    bool foundFirst = false;

    for (int i = 0; i < customPlot->graphCount(); ++i)
    {
        QCPGraph *graph = customPlot->graph(i);
        if (!graph || graph->data()->size() == 0 || index < 0 || index >= graph->data()->size()) {
            // Hide tracer if no data
            if (tracers.size() > i && tracers[i])
                tracers[i]->setVisible(false);
            labelText += QString("%1: ---\n")
                             .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name());
            continue;
        }

        auto it = graph->data()->constBegin();
        std::advance(it, index);

        double x = it->key;
        double y = it->value;

        // For header (use first graph's time)
        if (!foundFirst) {
            if (xLabel.toLower().contains("time") || xLabel.toLower().contains("date"))
                timeStr = QDateTime::fromMSecsSinceEpoch((qint64)x).toString("yyyy-MM-dd HH:mm:ss");
            else
                timeStr = QString::number(x, 'f', 2);
            labelX = x;
            labelY = y;
            foundFirst = true;
        }

        // Set tracer
        if (tracers.size() > i && tracers[i]) {
            tracers[i]->setGraph(graph);
            tracers[i]->setGraphKey(x);
            tracers[i]->setInterpolating(false);
            tracers[i]->setVisible(true);
        }

        // Format label for each graph (matches mouse function)
        labelText += QString("%1: %2\n")
                         .arg(graph->name().isEmpty() ? QString("Graph %1").arg(i + 1) : graph->name())
                         .arg(QString::number(y, 'g', 6));
    }

    // Add header with time
    if (cursorLabel && foundFirst) {
        QString header = QString("%1: %2\n").arg(xLabel.isEmpty() ? "Time" : xLabel).arg(timeStr);
        cursorLabel->setText(header + labelText.trimmed());
        cursorLabel->setVisible(true);
        cursorLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        cursorLabel->setTextAlignment(Qt::AlignLeft);

        // Offset logic
        QRect plotRect = customPlot->axisRect()->rect();
        int tracerPixelX = customPlot->xAxis->coordToPixel(labelX);
        int tracerPixelY = customPlot->yAxis->coordToPixel(labelY);

        QPoint labelOffset(-110, -50);
        int labelWidth = 120;
        int labelHeight = 40;
        if (tracerPixelX - labelWidth < plotRect.left())
            labelOffset.setX(40);
        if (tracerPixelY - labelHeight < plotRect.top())
            labelOffset.setY(20);

        QPoint labelPixelPos = QPoint(tracerPixelX, tracerPixelY) + labelOffset;
        double labelCoordX = customPlot->xAxis->pixelToCoord(labelPixelPos.x());
        double labelCoordY = customPlot->yAxis->pixelToCoord(labelPixelPos.y());

        cursorLabel->position->setCoords(labelCoordX, labelCoordY);
    } else if (cursorLabel) {
        cursorLabel->setVisible(false);
    }

    customPlot->replot();
}
