/* Modifier: Yuriy Kuzin
 * as I told you got somwhere on stackoverflow
 */
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include "qplaintexteditcursor.h"

QPlainTextEditCursor::QPlainTextEditCursor( QWidget* parent )
    : QPlainTextEdit( parent )
{
    bIsCursorVisible = true;
    CursorTimer = new QTimer( this );
    CursorTimer->setInterval( 750 );
    connect( CursorTimer, SIGNAL( timeout() ), this, SLOT( slot_BlinkCursor() ));

    CursorTimer->start();
}

QPlainTextEditCursor::~QPlainTextEditCursor()
{

}

void QPlainTextEditCursor::slot_BlinkCursor( )
{
    if( bIsCursorVisible )
        {
            bIsCursorVisible = false;
        }
    else
        {
            bIsCursorVisible = true;
        }
    viewport()->update();

}

void QPlainTextEditCursor::paintEvent( QPaintEvent* pEvent )
{
    QPainter oPainter( viewport() );
    if( bIsCursorVisible )
        {
            QRect r = cursorRect();
            r.setWidth(8);
            oPainter.fillRect( r, QBrush(Qt::green) );
        }
    QPlainTextEdit::paintEvent( pEvent );

}

void QPlainTextEditCursor::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event);
}

