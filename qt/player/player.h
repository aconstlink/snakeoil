//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_PLAYER_PLAYER_H_
#define _SNAKEOIL_QT_PLAYER_PLAYER_H_

#include "../typedefs.h"

#include "../timer/timer.h"

#include <QWidget>
#include <QPushButton>

namespace so_qt
{
    class player : public QWidget
    {
        Q_OBJECT

    private:

        /// if true, the play button need to be shown
        /// if false, the pause button need to be shown
        bool_t _play_state = true ;

        QPushButton * _play = nullptr ;
        QPushButton * _stop = nullptr ;

    public:

        player( QWidget * parent = nullptr ) ;
        player( so_qt::timer * timer, QWidget * parent = nullptr ) ;
        
        virtual ~player( void_t ) ;

    public:

        /// connects a timer to this player control.
        void connect( so_qt::timer * timer ) ;

    protected slots:

        void play_toggled( void ) ;
        void stop_toggled( void ) ;

    signals:

        void play( void ) ;
        void stop( void ) ;
        void pause( void ) ;
    };
}

#endif
