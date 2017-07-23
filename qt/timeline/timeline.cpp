//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "timeline.h"

#include "view/timeline_view.h"
#include "../timer/timer.h"

#include <snakeoil/animation/utility/time_converter.h>

#include <QBoxLayout>
#include <QKeyEvent>

using namespace so_qt ;

//**************************************************************************************************************
timeline::timeline( ulonglong_t max_ms, so_qt::timer * timer ) 
{
    _gs = new QGraphicsScene() ;
    _gv = new timeline_view(max_ms) ;

    /*
    _gv->setRenderHint(QPainter::Antialiasing, false);
    _gv->setDragMode(QGraphicsView::RubberBandDrag);
    _gv->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    _gv->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    _gv->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    */

    _gv->setSceneRect( QRectF(0,0,max_ms+2000,10) ) ;
    _gv->setScene( _gs ) ;
    _gv->centerOn( QPoint(0,0) ) ;

    auto * layout = new QVBoxLayout() ;
    layout->addWidget( _gv ) ;

    this->setLayout( layout ) ;

    if( timer != nullptr )
    {
        connect( timer ) ;
    }
}

//**************************************************************************************************************
timeline::timeline( uint_t min, uint_t sec, uint_t mil, so_qt::timer * timer ) : 
    timeline(so_ani::time_converter::to_milli( min, sec, mil ), timer )
{
}

//**************************************************************************************************************
timeline::~timeline( void ) 
{

}

//**************************************************************************************************************
void timeline::connect( timer * timer_ptr ) 
{
    QObject::connect(
            timer_ptr, SIGNAL( time_changed( unsigned long long ) ),
            this, SLOT( change_time( unsigned long long ) )
        ) ;
    
    QObject::connect(
            _gv, SIGNAL( milli_clicked( unsigned long long ) ),
            timer_ptr, SLOT( set_time( unsigned long long ) )
        ) ;

    timer_ptr->update_delta_milli(0) ;


}

//**************************************************************************************************************
void timeline::change_time( unsigned long long milli ) 
{
    ulonglong_t pos = milli ;
    _gv->change_player_position( pos ) ;
}
