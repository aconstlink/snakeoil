//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "timeline_item.h"

#include <qrect>
#include <qpainter>
#include <qstyleoption>

using namespace playground ;

//**************************************************************************************************************
timeline_item::timeline_item( void )
{

}

//**************************************************************************************************************
timeline_item::~timeline_item( void ) 
{

}

//**************************************************************************************************************
QRectF timeline_item::boundingRect() const 
{
    return QRectF( 0, 0, 10000, 10) ;
}

//**************************************************************************************************************
QPainterPath timeline_item::shape() const 
{
    return QGraphicsItem::shape() ;
}

//**************************************************************************************************************
void timeline_item::paint( QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget ) 
{
    float x = painter->clipRegion().boundingRect().x() ;
    painter->drawRoundedRect(x, 0, 50, 5, 5, 5);
}

//**************************************************************************************************************
