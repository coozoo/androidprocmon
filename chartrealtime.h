/* Author: Yuriy Kuzin
 * sure it's based on example provided by qcustomplot
 */
#ifndef CHARTREALTIME_H
#define CHARTREALTIME_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QDockWidget>
#include <QColor>
#include <limits>
#include "qcustomplot.h"

class chartRealTime : public QMainWindow
{
    Q_OBJECT
    //window(dock) title
    Q_PROPERTY(QString windowTitle
           READ getWindowTitle
           WRITE setWindowTitle
        )
    //range how much data range on chart
    Q_PROPERTY(int range
           READ getRange
           WRITE setRange
        )
    //range how much data range on chart
    Q_PROPERTY(bool isflow
           READ getisflow
           WRITE setisflow
        )
public:
    explicit chartRealTime(QMainWindow *parent = 0);
    ~chartRealTime();
    QGridLayout *main_gridlayout;
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    QCustomPlot *customPlot;
    QDockWidget *chart_dockwidget;
    void dataSlot(QDateTime dateTime, QList<double> data);
    void addGraphs(QStringList graphs);
    void setYAxisLabel(QString yAxisLabel);
    void setPlotTitle(QString plotTitle);
    void saveImage(QString imagePath, QString imageType, int width, int height);

    //list of colors to use by graphs tried to select unique
    QList<QColor> uniColor={ QColor(40,110,255),  QColor(0,183,48),    QColor(255,0,0),
                          QColor(0,128,64),    QColor(255,200,20),  QColor(0,155,175),
                          QColor(145,0,140),   QColor(205,125,0),   QColor(0,0,0)};

    void setWindowTitle(QString m_windowTitle)
    {
        windowTitle = m_windowTitle;
        emit on_setWindowTitle();
    }
    QString getWindowTitle() const
    { return windowTitle; }

    void setRange(int m_range)
    {
        range = m_range;
        emit on_setRange();
    }
    int getRange() const
    { return range; }

    void setisflow(int m_isflow)
    {
        isflow = m_isflow;
        emit on_setisflow();
    }
    int getisflow() const
    { return isflow; }


private:
  QString demoName;
  QString windowTitle;
  QTimer dataTimer;
  QElapsedTimer timer;
  int range;
  bool isflow;
  bool timeReset;

  QCPItemStraightLine *cursorLine = nullptr;
  QCPItemText *cursorLabel = nullptr;
  QList<QCPItemTracer*> tracers;

  qint64 lastTracerTime = -1;

signals:
  void datacoming(QDateTime dateTime, QList<double> data);
  void dockLocationChanged(bool state);
  void tracerTimeChanged(qint64 time);
  void tracerIndexChanged(int foundDataIndex);

public slots:
  void reset();
  void showTracerAtTime(qint64 time);
  void showTracerAtIndex(int index);

private slots:
  //void realtimeDataSlot();
  void on_setWindowTitle();
  void on_datacoming(QDateTime dateTime, QList<double> data);
  void on_visibilityChanged(bool);
  void showPointToolTip(QMouseEvent *event);
  void on_dockLocationChanged(bool state);
  void on_setRange();
  void on_setisflow();

protected:
     bool eventFilter(QObject* obj, QEvent *event) override;

};

#endif // CHARTREALTIME_H
