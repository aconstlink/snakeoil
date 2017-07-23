//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "timer.h"

#include <chrono>

using namespace so_qt ;

//**************************************************************************************************************
void timer::start_thread( bool with_thread ) 
{
    if( with_thread == false ) return ;

    _thread = std::move( std::thread( [&]( void_t )
        {
            while( this->_run ) 
            {
                auto t1 = std::chrono::system_clock::now() ;
                std::this_thread::sleep_for( std::chrono::milliseconds(10) ) ;
                auto t2 = std::chrono::system_clock::now() ;
                unsigned long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() ;
                this->update_delta_milli( ms ) ;
            }
        }
        ) ) ;
}

//**************************************************************************************************************
timer::timer( bool with_thread ) 
{
    start_thread(with_thread) ;
}

//**************************************************************************************************************
timer::timer( state s, bool with_thread ) : _cur_state(s)
{
    start_thread(with_thread) ;
}

//**************************************************************************************************************
timer::timer( state s, ulonglong_t milli, bool with_thread ) : _cur_state(s), _cur_milli(milli)
{
    start_thread(with_thread) ;
}

//**************************************************************************************************************
timer::timer( timer && rhv ) 
{
    _thread = std::move(rhv._thread) ;
    rhv._run = false ;
    _cur_state = rhv._cur_state ;
    _cur_milli = rhv._cur_milli ;
}

//**************************************************************************************************************
timer::~timer( void_t )  
{
    if( _run )
    {
        this->_run = false ;
        _thread.join() ;
    }
}

//**************************************************************************************************************
void timer::play( void )  
{
    _cur_state = state::play ;
}

//**************************************************************************************************************
void timer::pause( void )  
{
    _cur_state = state::pause ;
}

//**************************************************************************************************************
void timer::stop( void )  
{
    _cur_state = state::stop ;
    _cur_milli = 0 ;
    emit time_changed( _cur_milli ) ;
}

//**************************************************************************************************************
void timer::set_time( unsigned long long  milli )  
{
    _cur_milli = milli ;
    emit time_changed( _cur_milli ) ;
}

//**************************************************************************************************************
void timer::update_delta_sec( qreal dt )  
{
    this_t::update_delta_milli( unsigned long long ( dt * qreal(1000.0) ) ) ;
}

//**************************************************************************************************************
void timer::update_delta_milli( unsigned long long  milli ) 
{
    if( _cur_state == state::pause || _cur_state == state::stop ) return ;

    _cur_milli += milli ;
    emit time_changed( _cur_milli ) ;
}
