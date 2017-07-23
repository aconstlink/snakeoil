//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "countdown_clock.h"
#include "../time_converter/time_converter.h"

#include <snakeoil/animation/utility/time_converter.h>

#include <QHBoxLayout>

using namespace so_qt ;

//**************************************************************************************************************
void countdown_clock::construct( void ) 
{
    _text = new QLabel( this ) ;
    
    QHBoxLayout * l = new QHBoxLayout( this ) ;
    l->addWidget(_text) ;
}

//**************************************************************************************************************
void countdown_clock::connect( so_qt::timer * ptr ) 
{
    _timer = ptr ;

    QObject::connect(
        ptr, SIGNAL(time_changed( unsigned long long )),
        this, SLOT(timer_changed( unsigned long long ))
        ) ;

    QObject::connect(
        this, SIGNAL(count_down_expired()),
        ptr, SLOT(stop())
        ) ;
}

//**************************************************************************************************************
countdown_clock::countdown_clock( so_qt::timer * timer, QWidget * parent ) : QWidget( parent )
{
    construct() ;
    connect( timer ) ;
}

//**************************************************************************************************************
countdown_clock::countdown_clock( uint_t h, uint_t m, uint_t s, uint_t mi, so_qt::timer * timer, QWidget * parent ) : QWidget( parent )
{
    construct() ;
    connect( timer ) ;
    set_time( h, m, s, mi ) ;
}

//**************************************************************************************************************
countdown_clock::~countdown_clock( void ) 
{
}

//**************************************************************************************************************
void countdown_clock::set_time( uint_t h, uint_t m, uint_t s, uint_t mi )
{
    _max_milli = so_ani::time_converter::to_milli( h, m, s, mi ) ;
}

//**************************************************************************************************************
void countdown_clock::timer_changed( unsigned long long  milli ) 
{    
    if( _max_milli < milli )
    {
        emit count_down_expired() ;
        return ;
    }

    ulonglong_t reverse_milli = _max_milli - milli ;

    QString const hrs = so_qt::time_converter::to_hrs(reverse_milli) ;
    QString const min = so_qt::time_converter::to_min(reverse_milli) ;
    QString const sec = so_qt::time_converter::to_sec(reverse_milli) ;
    QString const mil = so_qt::time_converter::to_mil(reverse_milli) ;

    _text->setText( hrs + " : " + min + " : " + sec + " : " + mil ) ;    
}

//**************************************************************************************************************
void countdown_clock::setBackgroundColor( QColor const & color ) 
{
    this->setAutoFillBackground(true) ;
    QPalette p = this->palette() ;
    p.setColor( QPalette::Window, color ) ;
    this->setPalette( p ) ;
}

//**************************************************************************************************************
void countdown_clock::setTextColor( QColor const & color ) 
{
    
    QPalette p = this->palette() ;
    p.setColor( QPalette::WindowText, color ) ;
    this->setPalette( p ) ;
}
