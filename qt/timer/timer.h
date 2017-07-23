//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_TIMER_TIMER_H_
#define _SNAKEOIL_QT_TIMER_TIMER_H_

#include "../typedefs.h"

#include <QObject>

#include <thread>

namespace so_qt
{
    /// contains an absolute time in milli seconds
    /// helps to drive other time based widgets.
    class timer : public QObject
    {
        Q_OBJECT

    private:

        typedef timer this_t ;

    public:

        enum class state
        {
            play,
            stop,
            pause
        };

    private:

        ulonglong_t _cur_milli = 0 ;
        state _cur_state = state::stop ;

        std::thread _thread ;
        bool_t _run = true ;

    private:

        void start_thread( bool with_thread ) ;

    public:

        timer( bool with_thread=true ) ;
        timer( state, bool with_thread=true ) ;
        timer( state, ulonglong_t milli, bool with_thread=true ) ;
        timer( timer && rhv ) ;
        virtual ~timer( void_t ) ;

    public slots:

        void play( void ) ;
        void pause( void ) ;
        void stop( void ) ;

        void set_time( unsigned long long milli ) ;

        void update_delta_sec( qreal dt ) ;
        void update_delta_milli( unsigned long long delta_milli ) ;

    signals:

        void time_changed( unsigned long long  milli ) ;
    };
}

#endif
