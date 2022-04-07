#include "mycompleter.h"
#include <QDebug>
mycompleter::mycompleter(QStringList stringList, QObject *parent)
    : QCompleter(stringList,parent)
    , cursorPos_(-1)
{

}

ExpressionLineEdit::ExpressionLineEdit(QWidget* parent)
    : QLineEdit(parent)
{
stringList << "minRoute" << "minPitch" << "minSpacing";
completer_ = new mycompleter(stringList, this);
setCompleter(completer_);

    QObject::connect(this, SIGNAL(textChanged(const QString&)),
             completer_, SLOT(onLineEditTextChanged()));

    QObject::connect(this, SIGNAL(cursorPositionChanged(int, int)),
            completer_, SLOT(onLineEditTextChanged()));
}

QString mycompleter::pathFromIndex(const QModelIndex &index) const
{
    QString newStr = index.data(Qt::EditRole).toString();
    ExpressionLineEdit *lineEdit = qobject_cast<ExpressionLineEdit*>(parent());
    QString str = lineEdit->text();
    int prevSpacePos = str.mid(0, lineEdit->cursorPosition()).lastIndexOf(' ');

    int curPos = lineEdit->cursorPosition();
    int nextSpacePos = str.indexOf(' ', curPos);
    if (nextSpacePos == -1) {
        nextSpacePos = str.size();
}

QString part1 = str.mid(0, prevSpacePos + 1);
QString pre = str.mid(prevSpacePos + 1, curPos - prevSpacePos - 1);
QString post = str.mid(curPos, nextSpacePos - curPos);
QString part2 = str.mid(nextSpacePos);

    cursorPos_ = curPos + newStr.size() - pre.size();
    return part1 + newStr + part2;
}

void mycompleter::onLineEditTextChanged()
{
    qDebug() << "Completer::onLineEditTextChanged()" << cursorPos_;
    if (cursorPos_ != -1) {
        ExpressionLineEdit *lineEdit = qobject_cast<ExpressionLineEdit*>(parent());
        lineEdit->setCursorPosition(cursorPos_);
        cursorPos_ = -1;
    }
}

QStringList mycompleter::splitPath(const QString &path) const
{
     cursorPos_ = -1;
     ExpressionLineEdit *lineEdit = qobject_cast<ExpressionLineEdit*>(parent());
     QString text = lineEdit->text();
     QStringList stringList;
     QString str;
     int index = text.mid(0,lineEdit->cursorPosition()).lastIndexOf(' ');
     str = text.mid(index, lineEdit->cursorPosition()-index);
     str.trimmed();
     str.replace(" ", "");
     stringList << str;
     return stringList;
}

