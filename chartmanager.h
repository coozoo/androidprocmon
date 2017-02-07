/* Author: Yuriy Kuzin
 * Widget to place graphs inside... it's good to place it onto scrollarea
 * count all data and transfer it to appropriate chart
 * import charts as images everything is here
 */
#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QJsonDocument>
#include <QJsonObject>
#include "chartrealtime.h"

class chartManager : public QWidget
{
    Q_OBJECT
    //string with datetime it used to be folder name
    Q_PROPERTY(QString strDateTimeFile
           READ getstrDateTimeFile
           WRITE setstrDateTimeFile
        )
    //json rules fo composing charts
    Q_PROPERTY(QString jsonChartRuleObject
           READ getjsonChartRuleObject
           WRITE setjsonChartRuleObject
        )
    //here should be another property to store stats folder name
    //currently it's hardcoded
public:
    explicit chartManager(QWidget *parent = 0);
    ~chartManager();
    QGridLayout *main_gridlayout;
    //all created charts list
    QList<chartRealTime*> listCharts;

    int getValueByHeader(QString header, QString headersString, QString dataString);
    void saveAllCharts(QString imagePath, QString imageType, int width, int height);

    void setjsonChartRuleObject(QString m_jsonChartRuleObject)
    {
        jsonChartRuleObject = m_jsonChartRuleObject;
        emit jsonChartRuleObjectChanged();
    }
    QString getjsonChartRuleObject() const
    { return jsonChartRuleObject; }

    void setstrDateTimeFile(QString m_strDateTimeFile)
    {
        strDateTimeFile = m_strDateTimeFile;
    }
    QString getstrDateTimeFile() const
    { return strDateTimeFile; }

private:
    QString jsonChartRuleObject;
    QString strDateTimeFile;

signals:
    void jsonChartRuleObjectChanged();

public slots:
    void dataIncome(QString headersString, QString dataString);
    void on_jsonChartRuleObjectChanged();
    void resetAllCharts();
    void setAllRanges(int range);

protected:
     bool eventFilter(QObject* obj, QEvent *event);

};

#endif // CHARTMANAGER_H
