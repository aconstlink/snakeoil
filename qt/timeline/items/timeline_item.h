//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _TIMELINE_TIMELINE_ITEM_H_
#define _TIMELINE_TIMELINE_ITEM_H_

#include <qgraphicsitem>

namespace playground
{
    class timeline_item : public QGraphicsItem
    {
    public:

        timeline_item( void ) ;
        virtual ~timeline_item( void ) ;

    public:

        QRectF boundingRect() const Q_DECL_OVERRIDE;
        QPainterPath shape() const Q_DECL_OVERRIDE;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;
    };
}

#endif
