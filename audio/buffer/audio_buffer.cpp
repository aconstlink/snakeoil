//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "audio_buffer.h"
#include "pcm/ipcm_buffer.h"

using namespace so_audio ;

//*************************************************************************************
audio_buffer::audio_buffer( void_t )
{}

//*************************************************************************************
audio_buffer::audio_buffer( so_audio::api_object_ptr_t drv, so_audio::ipcm_buffer_ptr_t ptr ) :
    so_audio::object(drv), _pcm_ptr( ptr )
{}

//*************************************************************************************
audio_buffer::audio_buffer( this_rref_t rhv ) : object( std::move( rhv ) )
{
    so_move_member_ptr( _pcm_ptr, rhv ) ;
}

//*************************************************************************************
audio_buffer::~audio_buffer( void_t )
{
    if( so_core::is_not_nullptr( _pcm_ptr ) )
        _pcm_ptr->destroy()  ;
}

//*************************************************************************************
audio_buffer::this_ref_t audio_buffer::operator = ( this_rref_t rhv )
{
    base_t::operator=( std::move( rhv ) ) ;
    so_move_member_ptr( _pcm_ptr, rhv ) ;

    return *this ;
}

//*************************************************************************************
audio_buffer::this_ptr_t audio_buffer::create( so_memory::purpose_cref_t p )
{
    return so_audio::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
audio_buffer::this_ptr_t audio_buffer::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_audio::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t audio_buffer::destroy( this_ptr_t ptr )
{
    so_audio::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_audio::ipcm_buffer_ptr_t audio_buffer::get_pcm_buffer( void_t )
{
    return _pcm_ptr ;
}

//*************************************************************************************
so_audio::ipcm_buffer_cptr_t audio_buffer::get_pcm_buffer( void_t ) const
{
    return _pcm_ptr ;
}

//*************************************************************************************
void_t audio_buffer::destroy( void_t )
{
    this_t::destroy( this ) ;
}