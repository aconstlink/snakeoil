//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "imex_pcm_wrapper.h"

#include <snakeoil/log/global.h>

using namespace so_audio ;

//*************************************************************************************
imex_pcm_wrapper::imex_pcm_wrapper( so_imex::iaudio_ptr_t aptr )
{
    so_log::global::error_and_exit( so_core::is_nullptr( aptr ), 
        "[imex_pcm_wrapper::imex_pcm_wrapper] : must not be nullptr" ) ;

    _imex_audio_ptr = aptr ;
}

//*************************************************************************************
imex_pcm_wrapper::imex_pcm_wrapper( this_rref_t rhv )
{
    so_move_member_ptr( _imex_audio_ptr, rhv ) ;
}

//*************************************************************************************
imex_pcm_wrapper::~imex_pcm_wrapper( void_t )
{}

//*************************************************************************************
imex_pcm_wrapper::this_ptr_t imex_pcm_wrapper::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_audio::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t imex_pcm_wrapper::destroy( this_ptr_t ptr )
{
    so_audio::memory::dealloc( ptr ) ;
}

//*************************************************************************************
double_t imex_pcm_wrapper::get_time_in_sec( void_t ) const
{
    return _imex_audio_ptr->get_time_in_sec() ;
}

//*************************************************************************************
so_audio::buffer_type imex_pcm_wrapper::get_buffer_type( void_t ) const
{
    auto const type = _imex_audio_ptr->get_type() ;
    switch( type )
    {
    case so_imex::pcm_buffer_type::int8:
        return so_audio::buffer_type::int8 ;
    case so_imex::pcm_buffer_type::int16:
        return so_audio::buffer_type::int16 ;
    case so_imex::pcm_buffer_type::int32:
        return so_audio::buffer_type::int32 ;
    case so_imex::pcm_buffer_type::ieee_float:
        return so_audio::buffer_type::ieee_float ;
    }

    return so_audio::buffer_type::invalid ;
}

//*************************************************************************************
so_audio::range_t imex_pcm_wrapper::copy_from( ipcm_buffer_ptr_t pcm_in, 
    size_t const start, size_t const num_samples )
{
    return this_t::copy_from( pcm_in->get_buffer_type(),
        static_cast< byte_cptr_t >( pcm_in->get_buffer_ptr_at( start ) ), num_samples ) ;
}

//*************************************************************************************
so_audio::range_t imex_pcm_wrapper::copy_from( byte_cptr_t dptr, size_t const num_samples )
{
    return this_t::copy_from( this_t::get_buffer_type(), dptr, num_samples ) ;
}

//*************************************************************************************
so_audio::range_t imex_pcm_wrapper::copy_from( so_audio::buffer_type, byte_cptr_t, size_t const )
{
    return so_audio::range_t( 0, 0 ) ;
}

//*************************************************************************************
void_t imex_pcm_wrapper::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
void_cptr_t imex_pcm_wrapper::get_buffer_ptr_at( size_t const smp ) const
{
    byte_cptr_t mem_ptr = _imex_audio_ptr->get_memory_at( smp ) ;
    return void_cptr_t(mem_ptr) ;
}

//*************************************************************************************
void_cptr_t imex_pcm_wrapper::get_buffer_ptr( void_t ) const
{
    byte_cptr_t mem_ptr = _imex_audio_ptr->get_memory_at( 0 ) ;
    return void_cptr_t( mem_ptr ) ;
}

//*************************************************************************************
size_t imex_pcm_wrapper::get_num_samples( void_t ) const
{
    return _imex_audio_ptr->get_num_samples() ;
}

//*************************************************************************************
size_t imex_pcm_wrapper::get_num_samples_per_second( void_t ) const
{
    return _imex_audio_ptr->get_num_samples_per_second() ;
}

//*************************************************************************************
size_t imex_pcm_wrapper::get_num_channels( void_t ) const
{
    return _imex_audio_ptr->get_num_channels() ;
}