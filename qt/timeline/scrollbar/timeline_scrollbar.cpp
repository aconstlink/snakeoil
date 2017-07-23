//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "timeline_scrollbar.h"

#include <QPaintEvent>
#include <QPainter>
#include <QFont>

using namespace so_qt ;

//**************************************************************************************************************
timeline_scrollbar::timeline_scrollbar( void ) 
{
}

//**************************************************************************************************************
timeline_scrollbar::~timeline_scrollbar( void ) 
{
}

//**************************************************************************************************************
void timeline_scrollbar::paintEvent( QPaintEvent * event ) 
{
    QScrollBar::paintEvent( event ) ;

    QPainter p(this) ;
    p.setPen(QPen(Qt::black, 3));
    QFont font = p.font() ;
    font.setPointSize(7) ;
    p.setFont(font) ;

    {
        p.drawText( QPoint(20,13), tr("00:00:000") ) ;
    }
    event->accept() ;    
}
