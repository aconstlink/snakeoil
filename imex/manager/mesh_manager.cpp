//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "mesh_manager.h"

#include <snakeoil/geometry/mesh/imesh.h>
#include <snakeoil/log/log.h>

using namespace so_imex ;


//*************************************************************************************
mesh_manager::mesh_manager( void_t ) 
{

}

//*************************************************************************************
mesh_manager::mesh_manager( this_rref_t )
{

}

//*************************************************************************************
mesh_manager::~mesh_manager( void_t )
{
    this_t::do_destruction( [&]( base_t::manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->data_ptr ) )
            item_ptr->data_ptr->destroy() ;
    } ) ;
}

//*************************************************************************************
mesh_manager::this_ptr_t mesh_manager::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
void_t mesh_manager::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t mesh_manager::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
