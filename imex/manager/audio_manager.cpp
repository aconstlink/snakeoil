//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "audio_manager.h"

using namespace so_imex ;

//**********************************************************************************
audio_manager::audio_manager( void_t ) 
{
}

//**********************************************************************************
audio_manager::audio_manager( this_rref_t )
{
}

//**********************************************************************************
audio_manager::~audio_manager( void_t )
{
}

//**********************************************************************************
audio_manager::this_ptr_t audio_manager::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************
audio_manager::this_ptr_t audio_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************
void_t audio_manager::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//**********************************************************************************
void_t audio_manager::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************