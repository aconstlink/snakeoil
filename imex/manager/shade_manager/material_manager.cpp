//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "material_manager.h"

#include <snakeoil/shade/material/imaterial.h>

using namespace so_imex ;


//*************************************************************************************
material_manager::material_manager( void_t ) 
{

}

//*************************************************************************************
material_manager::material_manager( this_rref_t rhv ) : base_t( std::move(rhv) )
{

}

//*************************************************************************************
material_manager::~material_manager( void_t )
{
    this_t::do_destruction( [&]( base_t::manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->data_ptr ) )
            item_ptr->data_ptr->destroy() ;
    } ) ;
}

//*************************************************************************************
material_manager::this_ptr_t material_manager::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
void_t material_manager::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t material_manager::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
