//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_TIMELINE_TIMELINE_VIEW_H_
#define _SNAKEOIL_QT_TIMELINE_TIMELINE_VIEW_H_

#include "../../typedefs.h"

#include <QGraphicsView>

namespace so_qt
{
    class timeline_view : public QGraphicsView
    {    
        Q_OBJECT

    private:

        QPoint _mouse_point ;
        QPoint _mouse_point_global ;

        // in x-coord points.
        ulonglong_t _player_pos = 0 ;
        ulonglong_t _max_x = 0 ;
    public:

        timeline_view( ulonglong_t max_x ) ;
        virtual ~timeline_view( void ) ;

    public slots:

        void change_player_position( unsigned long long ) ;

    private slots:

        void horz_triggered( int value ) ;

    private:

        /// returns ( scale_x, 1/scale_x )
        QPointF scaleX( void ) const ;

    protected:

        virtual void drawBackground( QPainter * painter, const QRectF & rect ) override ;
        virtual void wheelEvent( QWheelEvent * event ) ;
        virtual void mouseMoveEvent( QMouseEvent * event ) ;
        virtual void mouseReleaseEvent( QMouseEvent * event ) ;


    signals:

        void milli_clicked( unsigned long long ) ;
    };
}

#endif
