#ifndef MYCOMPLETER_H
#define MYCOMPLETER_H
#include <QCompleter>
#include <QString>
#include <QStringList>
#include <QLineEdit>

class mycompleter:public QCompleter
{
Q_OBJECT
public:
    explicit mycompleter(QStringList stringList, QObject *parent=0);
    virtual QString pathFromIndex(const QModelIndex &index)const;
    virtual QStringList splitPath(const QString&)const;

public slots:
    void onLineEditTextChanged();
private:
    mutable int cursorPos_;
};


class ExpressionLineEdit: public QLineEdit
{
    Q_OBJECT
    public:
        explicit ExpressionLineEdit(QWidget *parent=0);
private:
    QStringList stringList;
    mycompleter *completer_;
};
#endif // MYCOMPLETER_H
