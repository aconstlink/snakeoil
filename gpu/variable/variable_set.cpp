//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "variable_set.h"

using namespace so_gpu ;

//*****************************************************************************************************
variable_set::variable_set( void_t ) 
{

}

//*****************************************************************************************************
variable_set::variable_set( variable_set && rhv ) : base_t( std::move( rhv ) )
{
    _data_variables = std::move( rhv._data_variables ) ;
    _texture_variables = std::move( rhv._texture_variables ) ;
    _data_buffer_variables = std::move( rhv._data_buffer_variables ) ;
}

//*****************************************************************************************************
variable_set::~variable_set( void_t ) 
{
    for( auto item : _data_variables )
    {
        so_gpu::memory::dealloc( item.first ) ;
    }

    for( auto item : _texture_variables )
    {
        so_gpu::memory::dealloc( item.first ) ;
    }

    for( auto item : _data_buffer_variables )
    {
        so_gpu::memory::dealloc( item.first ) ;
    }
}

//*****************************************************************************************************
variable_set::this_ptr_t variable_set::create( void_t ) 
{
    return this_t::create( "[so_gpu::variable_set::create]" ) ;
}

//*****************************************************************************************************
variable_set::this_ptr_t variable_set::create( std::string const & purpose ) 
{
    return so_gpu::memory::alloc( this_t(), purpose ) ;
}

//*****************************************************************************************************
void_t variable_set::destroy( this_ptr_t ptr ) 
{
    so_gpu::memory::dealloc( ptr ) ;
}

//*****************************************************************************************************
void_t variable_set::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

