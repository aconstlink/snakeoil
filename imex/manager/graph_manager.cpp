//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "graph_manager.h"
#include "../node/node.h"

#include <snakeoil/log/log.h>

using namespace so_imex ;


//*************************************************************************************
graph_manager::graph_manager( void_t ) 
{

}

//*************************************************************************************
graph_manager::graph_manager( this_rref_t rhv ) : base_resource_manager( std::move(rhv) )
{
}

//*************************************************************************************
graph_manager::~graph_manager( void_t )
{
    this_t::do_destruction( [&]( base_t::manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->data_ptr ) )
            item_ptr->data_ptr->destroy() ;
    } ) ;
}

//*************************************************************************************
graph_manager::this_ptr_t graph_manager::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
graph_manager::this_ptr_t graph_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t graph_manager::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t graph_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
