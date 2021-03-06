//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "null_api.h"

using namespace so_audio ;

//*************************************************************************************
null_api::null_api( void_t ) 
{
}

//*************************************************************************************
null_api::null_api( this_rref_t )
{
}

//*************************************************************************************
null_api::~null_api( void_t )
{
}

//*************************************************************************************
null_api::this_ptr_t null_api::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_audio::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t null_api::destroy( this_ptr_t ptr )
{
    so_audio::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t null_api::initialize( void_t ) 
{
    return true ;
}

//*************************************************************************************
void_t null_api::release( void_t ) 
{
}

//*************************************************************************************
api_type null_api::get_type( void_t ) const
{
    return so_audio::api_type::null ;
}

//*************************************************************************************
void_t null_api::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************