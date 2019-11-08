
#include "log_transition.h"

#include <snakeoil/log/global.h>
#include <snakeoil/memory/global.h>

using namespace sox_presentation ;


//**************************************************
log_transition::log_transition( size_t const i ) noexcept 
{
    _i = i ;
}

//**************************************************
log_transition::log_transition( this_rref_t rhv ) noexcept
{
    _i = rhv._i ;
    _render = rhv._render ;
    _update = rhv._update ;
}

//**************************************************
log_transition::~log_transition( void_t ) noexcept
{
}

//**************************************************
log_transition::this_ptr_t log_transition::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_memory::global_t::alloc( std::move( rhv ), p ) ;
}

//**************************************************
void_t log_transition::destroy( this_utr_t ptr )
{
    so_memory::global_t::dealloc( ptr ) ;
}

//**************************************************
bool_t log_transition::on_load() noexcept
{
    so_log::global_t::status( "[log_transition] : on_load " + std::to_string(_i) ) ;
    
    _update = false ;
    _render = false ;

    return true ;
}

//**************************************************
bool_t log_transition::on_unload() noexcept
{
    so_log::global_t::status( "[log_transition] : on_unload " + std::to_string(_i) ) ;

    _update = false ;
    _render = false ;

    return true ;
}

//**************************************************
bool_t log_transition::on_init() noexcept 
{
    so_log::global_t::status( "[log_transition] : on_init " + std::to_string( _i ) ) ;  
    return true ;
}

//**************************************************
bool_t log_transition::on_release() noexcept 
{
    so_log::global_t::status( "[log_transition] : on_release " + std::to_string( _i ) ) ;
    return true ;
}

//**************************************************
std::chrono::milliseconds log_transition::get_duration( void_t ) noexcept 
{
    return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::seconds( 2 ) ) ;
}

//**************************************************
void_t log_transition::on_update( sox_presentation::update_data_in_t ) noexcept
{
    if( so_core::is_not( _update  ) )
    {
        so_log::global_t::status( "[log_transition] : on_update " + std::to_string(_i) ) ;
        _update = true ;
    }
}

//**************************************************
void_t log_transition::on_render( render_type const, sox_presentation::render_data_in_t ) noexcept
{
    if( so_core::is_not( _render ) )
    {
        so_log::global_t::status( "[log_transition] : on_render " + std::to_string( _i ) ) ;
        _render = true ;
    }
}

//**************************************************
void_t log_transition::destroy( void_t ) noexcept
{
    this_t::destroy( this ) ;
}

//**************************************************