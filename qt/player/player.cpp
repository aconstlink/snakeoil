//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "player.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

using namespace so_qt ;

//**************************************************************************************************************
player::player( QWidget * parent ) : QWidget( parent )
{
    _play = new QPushButton( tr("Play"), this ) ;
    _stop = new QPushButton( tr("Stop"), this ) ;

    QHBoxLayout * l = new QHBoxLayout( this ) ;
    l->addWidget( _play ) ;
    l->addWidget( _stop ) ;

    QObject::connect(
            _play, SIGNAL( released( void ) ),
            this, SLOT( play_toggled( void ) )
        ) ;
    QObject::connect(
            _stop, SIGNAL( released( void ) ),
            this, SLOT( stop_toggled( void ) )
        ) ;
}

//**************************************************************************************************************
player::player( so_qt::timer * timer, QWidget * parent ) : player(parent)
{
    connect( timer ) ;
}

//**************************************************************************************************************
player::~player( void_t ) 
{

}

//**************************************************************************************************************
void player::connect( so_qt::timer * timer ) 
{
    if( timer == nullptr ) return ;

    QObject::connect(
            this, SIGNAL( play( void ) ),
            timer, SLOT( play( void ) )
        ) ;
    QObject::connect(
            this, SIGNAL( pause( void ) ),
            timer, SLOT( pause( void ) )
        ) ;
    QObject::connect(
            this, SIGNAL( stop( void ) ),
            timer, SLOT( stop( void ) )
        ) ;
}

//**************************************************************************************************************
void player::play_toggled( void ) 
{
    _play_state = !_play_state ;
    if( !_play_state ) 
    {
        _play->setText("Pause") ;
        emit play() ;
    }
    else 
    {
        _play->setText("Play") ;
        emit pause() ;
    }
}

//**************************************************************************************************************
void player::stop_toggled( void ) 
{
    _play_state = true ;
    _play->setText("Play") ;
    emit stop() ;
}
