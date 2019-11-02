
#include "log_page.h"

#include <snakeoil/log/global.h>
#include <snakeoil/memory/global.h>

using namespace sox_presentation ;


//**************************************************
log_page::log_page( size_t const i ) noexcept 
{
    _i = i ;
}

//**************************************************
log_page::log_page( this_rref_t rhv ) noexcept
{
    _i = rhv._i ;
    _render = rhv._render ;
    _update = rhv._update ;
}

//**************************************************
log_page::~log_page( void_t ) noexcept
{
}

//**************************************************
log_page::this_ptr_t log_page::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_memory::global_t::alloc( std::move( rhv ), p ) ;
}

//**************************************************
void_t log_page::destroy( this_utr_t ptr )
{
    so_memory::global_t::dealloc( ptr ) ;
}

//**************************************************
bool_t log_page::on_load() noexcept
{
    so_log::global_t::status( "[log_page] : on_load " + std::to_string(_i) ) ;
    
    _update = false ;
    _render = false ;

    return true ;
}

//**************************************************
bool_t log_page::on_unload() noexcept
{
    so_log::global_t::status( "[log_page] : on_unload " + std::to_string(_i) ) ;

    _update = false ;
    _render = false ;

    return true ;
}

//**************************************************
void_t log_page::on_update( sox_presentation::update_data_in_t ) noexcept
{
    if( so_core::is_not( _update  ) )
    {
        so_log::global_t::status( "[log_page] : on_update " + std::to_string(_i) ) ;
        _update = true ;
    }
}

//**************************************************
void_t log_page::on_render( sox_presentation::render_data_in_t ) noexcept
{
    if( so_core::is_not( _render ) )
    {
        so_log::global_t::status( "[log_page] : on_render " + std::to_string( _i ) ) ;
        _render = true ;
    }
}

//**************************************************
void_t log_page::destroy( void_t ) noexcept
{
    this_t::destroy( this ) ;
}

//**************************************************