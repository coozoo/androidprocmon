/* Modifier: Yuriy Kuzin
 * widget based on QPlainTextEdit but with some features to look like terminal
 * I've got it somwhere from stack overflow and little bit modified for mine purposes
 */
#ifndef QPLAINTEXTEDITCURSOR_H
#define QPLAINTEXTEDITCURSOR_H

#include <QPlainTextEdit>

class QPainter;
class QTimer;

class QPlainTextEditCursor : public QPlainTextEdit
{
    Q_OBJECT
public:
    QPlainTextEditCursor( QWidget* parent = 0);
    ~QPlainTextEditCursor();

public slots:
    void slot_BlinkCursor( );

signals:
    void keyPressed(QKeyEvent *e);

protected:
    void paintEvent( QPaintEvent* pEvent );
    void keyPressEvent(QKeyEvent * e);

private:
    bool bIsCursorVisible;
    QTimer* CursorTimer;
};


#endif // QPLAINTEXTEDITCURSOR_H
