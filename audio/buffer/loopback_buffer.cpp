//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "loopback_buffer.h"

#include "pcm/ipcm_buffer.h"

#include <snakeoil/log/log.h>

using namespace so_audio ;

//*************************************************************************************
loopback_buffer::loopback_buffer( void_t ) 
{
}

//*************************************************************************************
loopback_buffer::loopback_buffer( this_rref_t rhv )
{
    so_move_member_ptr( _pcm_ptr, rhv ) ;
}

//*************************************************************************************
loopback_buffer::~loopback_buffer( void_t )
{
    so_log::log::error_and_exit( so_core::is_not_nullptr(_pcm_ptr), 
        "[loopback_buffer::~loopback_buffer] : please release before destroying the buffer" ) ;
}

//*************************************************************************************
loopback_buffer::this_ptr_t loopback_buffer::create( so_memory::purpose_cref_t p )
{
    return so_audio::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
void_t loopback_buffer::destroy( this_ptr_t ptr )
{
    so_audio::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t loopback_buffer::set_pcm_buffer( ipcm_buffer_ptr_t bptr )
{
    if( so_core::is_not_nullptr(_pcm_ptr) )
    {
        _pcm_ptr->destroy() ;
    }
    _pcm_ptr = bptr ;
}

//*************************************************************************************
ipcm_buffer_ptr_t loopback_buffer::get_pcm_buffer( void_t )
{
    return _pcm_ptr ;
}

//*************************************************************************************
void_t loopback_buffer::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************