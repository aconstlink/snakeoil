//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "timeline_view.h"
#include "../scrollbar/timeline_scrollbar.h"

#include "../../time_converter/time_converter.h"

#include <QScrollbar>
#include <QWheelEvent>
#include <QPen>
#include <QRect>

#include <iostream>

using namespace so_qt ;

//**************************************************************************************************************
timeline_view::timeline_view( ulonglong_t max_x ) : _max_x(max_x)
{
    this->setMouseTracking( true ) ;
    //this->scale( qreal(0.25), qreal(1) ) ;
    
    this->setHorizontalScrollBar( new timeline_scrollbar() ) ;

    QObject::connect( 
        this->horizontalScrollBar(), SIGNAL( actionTriggered(int) ),
        this, SLOT( horz_triggered(int) )
        ) ;
}

//**************************************************************************************************************
timeline_view::~timeline_view( void ) 
{

}

//**************************************************************************************************************
void timeline_view::horz_triggered( int value ) 
{
    //this->repaint() ;
    //this->update() ;
    this->viewport()->update() ;
}

//**************************************************************************************************************
QPointF timeline_view::scaleX( void ) const 
{
    return QPointF( this->transform().m11(), qreal(1) / this->transform().m11() ) ;
}

//**************************************************************************************************************
void timeline_view::wheelEvent( QWheelEvent * event ) 
{
    if( event->modifiers() & Qt::ControlModifier )
    {
        // I would like to move the horizontal scrollbar into the direction
        // of the mouse position in the view.
        {
            // the unscaled page step.
            // even if the scene is scaled, the page step will stay the same.
            // it is not required to correct the zoom.
            int const page_width = this->horizontalScrollBar()->pageStep() ;
            //std::cout << page_width << std::endl ;

            // so it is possible to just add the delta of mouse x() to the page center
            // to the current horizontal sbar value.
            int const dx = _mouse_point.x() - page_width / 2 ;
            //std::cout << dx << std::endl ;

            this->horizontalScrollBar()->setValue(
                this->horizontalScrollBar()->value() + dx
                ) ;
        }

        int d = event->delta() ;
        if( d < 0 )
        {                    
            this->scale( qreal(0.5), qreal(1) ) ;
        }
        else // d > 0
        {            
            this->scale( qreal(2), qreal(1) ) ;
        }
        
        event->accept() ;
    }
    else if( event->modifiers() & Qt::ShiftModifier )
    {
        int sb = this->horizontalScrollBar()->value() ;
        this->horizontalScrollBar()->setValue( sb-event->delta() ) ;
        event->accept() ;
    }
    else
    {
        QGraphicsView::wheelEvent( event ) ;
    }
    this->viewport()->update() ;
}

//**************************************************************************************************************
void timeline_view::mouseMoveEvent( QMouseEvent * event ) 
{
    _mouse_point = event->pos() ;
    _mouse_point_global = _mouse_point ;
    _mouse_point_global.setX( _mouse_point.x() + this->horizontalScrollBar()->value() ) ;

    if( event->buttons() & Qt::RightButton )
    {
        // the unscaled page step.
        // even if the scene is scaled, the page step will stay the same.
        // it is not required to correct the zoom.
        int const page_width = this->horizontalScrollBar()->pageStep() ;
        //std::cout << page_width << std::endl ;

        // so it is possible to just add the delta of mouse x() to the page center
        // to the current horizontal sbar value.
        int const dx = _mouse_point.x() - page_width / 2 ;
        //std::cout << dx << std::endl ;

        this->horizontalScrollBar()->setValue(
            this->horizontalScrollBar()->value() + dx
            ) ;
    }
    
    QGraphicsView::mouseMoveEvent( event ) ;
    this->viewport()->update() ;
}

//**************************************************************************************************************
void timeline_view::mouseReleaseEvent( QMouseEvent * event ) 
{
    _mouse_point = event->pos() ;
    _mouse_point_global = _mouse_point ;
    _mouse_point_global.setX( _mouse_point.x() + this->horizontalScrollBar()->value() ) ;

    auto buttons = event->buttons() ;
    if( buttons & Qt::LeftButton || event->button() == Qt::LeftButton )
    {
        ulonglong_t milli = _mouse_point_global.x() * scaleX().y() ;
        emit milli_clicked(milli) ;
    }
}

//**************************************************************************************************************
void timeline_view::change_player_position( unsigned long long pos ) 
{
    _player_pos = pos ;
    this->viewport()->update() ;
}

//**************************************************************************************************************
void timeline_view::drawBackground( QPainter * painter, const QRectF & rect_in )
{    
    QTransform t = this->transform() ;
    QPointF scale( t.m11(), t.m22() ) ;
    QPointF scale_inv( qreal(1)/t.m11(), qreal(1)/t.m22() ) ;

    // scale the painter for accounting the zoom.
    // the pen width should stay constant across zooms.
    {
        painter->scale( scale_inv.x(), scale_inv.y() ) ;
    }

    QRectF rect = rect_in ;

    // adjust the rect for the scaling factor due to zooming.
    {
        rect.setLeft( rect.x() * scale.x() ) ;
        rect.setRight( (rect.x()+rect.width()) * scale.x() ) ;
    }
    
    // begin text
    // draw debug number to the begin of the viewport
    /*
    {
        int x_scaled = rect.x() ;
        int x_nonscaled = rect_in.x() ;
        painter->drawText( QPoint(QPoint(x_scaled, 30)), QString::number(x_nonscaled) ) ;
    }

    // end text
    // draw debug number to the end of the viewport
    {
        int x_scaled = rect.x() + rect.width() - 50 ;
        int x_nonscaled = rect_in.x() + rect_in.width() ;
        painter->drawText( QPoint(QPoint(x_scaled, 30)), QString::number(x_nonscaled) ) ;
    }
    */
    
    // small lines
    {
        QPen old_pen = painter->pen() ;
        painter->setPen( QPen(Qt::gray, 1) ) ;        
        {
            float dx = 10.0f ;

            float x = rect.x() / dx ;
            float x_floor = std::floor(x) ;
            float x_frac = x - x_floor ;

            float start_x = rect.x() + (1.0 - x_frac) * dx ;
            float start_y = 0.0 ;

            for( int i=0; i<int(rect.width()/dx)+1; ++i )
            {
                float cur_x = start_x + float(i) * dx ;
                painter->drawLine( QLine(QPoint(cur_x, start_y), QPoint(cur_x, start_y+10)) ) ;
            }
        }
        painter->setPen(old_pen) ;
    }
    
    // big lines
    {
        float dx = 50.0f ;

        float x = rect.x() / dx ;
        float x_floor = std::floor(x) ;
        float x_frac = x - x_floor ;

        float start_x = rect.x() + (1.0 - x_frac) * dx ;
        float start_y = 0.0 ;

        for( int i=0; i<int(rect.width()/dx)+1; ++i )
        {
            float cur_x = start_x + float(i) * dx ;
            painter->drawLine( QLine(QPoint(cur_x, start_y), QPoint(cur_x, start_y+15)) ) ;
            //painter->drawText( QPoint(QPoint(cur_x+1, start_y)), QString::number(i) ) ;
        }
    }

    // time points
    {
        QFont font_old = painter->font() ;
        QFont font = font_old ;
        font.setPointSize(7) ;
        painter->setFont( font ) ;

        float dx = 100.0f ;

        float x = rect.x() / dx ;
        float x_floor = std::floor(x) ;
        float x_frac = x - x_floor ;

        float start_x = rect.x() + (1.0 - x_frac) * dx ;
        float start_y = 25.0f ;

        for( int i=-1; i<int(rect.width()/dx)+1; ++i )
        {
            float cur_x = start_x + float(i) * dx ;
            
            // for calculating the time, the original x value at 
            // the current point is required.
            float orig_x = cur_x * scale_inv.x() ;
                              
            {
                painter->drawText( 
                    QPoint(QPoint(cur_x+1, start_y)), 
                    time_converter().to_min( orig_x ) + tr(":") + 
                    time_converter().to_sec( orig_x ) + tr(":") +
                    time_converter().to_mil( orig_x ) ) ;
            }
        }

        painter->setFont( font_old ) ;
    }

    // mouse
    {
        QFont font_old = painter->font() ;
        QFont font = font_old ;
        font.setPointSize(6) ;
        painter->setFont( font ) ;

        int x = rect.x() + _mouse_point.x() ;
        int y = 0 ;
        painter->drawLine( QLine(x,y-10,x,y+10) ) ;

        {
            int x_scale = x * scale_inv.x() ;
            painter->drawText( 
                QPoint(QPoint(x+3, y-4)), 
                time_converter().to_min( x_scale ) + tr(":") + 
                time_converter().to_sec( x_scale ) + tr(":") +
                time_converter().to_mil( x_scale ) ) ;
        }
        painter->setFont( font_old ) ;
    }

    // player
    {
        QFont font_old = painter->font() ;
        QPen old_pen = painter->pen() ;

        {
            painter->setPen( QPen( QColor( 0, 155, 0 ) ) ) ;
            painter->setFont( QFont( font_old.family(), 6 ) ) ;
        }

        int const x = _player_pos * scale.x() ;
        int const y = 0 ;
        
        
        int const text_x = _player_pos ;
        int const draw_x = _player_pos * scale.x() ;

        // draw
        {
            painter->drawLine( QLine(draw_x,y-18,draw_x,y+10) ) ;
            
            painter->drawText( 
                QPoint(QPoint(draw_x+3, y-13)), 
                time_converter().to_min( text_x ) + tr(":") + 
                time_converter().to_sec( text_x ) + tr(":") +
                time_converter().to_mil( text_x ) ) ;
        }

        painter->setPen(old_pen) ;
        painter->setFont( font_old ) ;
    }

    // end line at max_x
    if( (_max_x * scale.x() - 50) < rect.right() )
    {
        QFont font_old = painter->font() ;
        QPen old_pen = painter->pen() ;

        {
            painter->setPen( QPen( QColor( 200, 0, 0 ) ) ) ;
            painter->setFont( QFont( font_old.family(), 6 ) ) ;
        }
                
        int const y = 0 ;
                
        int const text_x = _max_x ;
        int const draw_x = _max_x * scale.x() ;
        // draw
        {
            painter->drawLine( QLine(draw_x,y-18,draw_x,y+10) ) ;
            
            painter->drawText( 
                QPoint(QPoint(draw_x-38, y-13)), 
                time_converter().to_min( text_x ) + tr(":") + 
                time_converter().to_sec( text_x ) + tr(":") +
                time_converter().to_mil( text_x ) ) ;
        }

        painter->setPen(old_pen) ;
        painter->setFont( font_old ) ;
    }

    painter->scale( qreal(1), qreal(1) ) ;
}
