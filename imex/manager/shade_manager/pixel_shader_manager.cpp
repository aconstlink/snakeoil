//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "pixel_shader_manager.h"

#include <snakeoil/shade/shader/pixel_shader.h>

using namespace so_imex ;


//*************************************************************************************
pixel_shader_manager::pixel_shader_manager( void_t ) 
{
}

//*************************************************************************************
pixel_shader_manager::pixel_shader_manager( this_rref_t rhv ) : base_t( std::move(rhv) )
{
}

//*************************************************************************************
pixel_shader_manager::~pixel_shader_manager( void_t )
{
    this_t::do_destruction( [&]( base_t::manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->data_ptr ) )
            item_ptr->data_ptr->destroy() ;
    } ) ;
}

//*************************************************************************************
pixel_shader_manager::this_ptr_t pixel_shader_manager::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
void_t pixel_shader_manager::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t pixel_shader_manager::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
