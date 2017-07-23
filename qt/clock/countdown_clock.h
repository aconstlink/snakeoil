//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_CLOCKCOUNT_DOWN_CLOCK_H_
#define _SNAKEOIL_QT_CLOCKCOUNT_DOWN_CLOCK_H_

#include "../typedefs.h"
#include "../timer/timer.h"

#include <QWidget>
#include <QLabel>

namespace so_qt
{
    class countdown_clock : public QWidget
    {
        Q_OBJECT

    private:

        ulonglong_t _max_milli = 0 ;

        so_qt::timer * _timer ;

        QLabel * _text ;

    private:

        void construct( void ) ;
        void connect( so_qt::timer * ptr ) ;

    public:

        countdown_clock( so_qt::timer * timer, QWidget * parent = nullptr ) ;
        countdown_clock( uint_t h, uint_t m, uint_t s, uint_t mi, so_qt::timer * timer, QWidget * parent=nullptr ) ;
        
        virtual ~countdown_clock( void ) ;

    public:

        void setBackgroundColor( QColor const & color ) ;
        void setTextColor( QColor const & color ) ;

    public:

        void set_time( uint_t h, uint_t m, uint_t s, uint_t mi ) ;


    protected slots:

        void timer_changed( unsigned long long  milli ) ;

    signals:

        /// the count down reached its end
        void count_down_expired( void ) ;


    };
}

#endif
