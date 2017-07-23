//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "serial_node_walker.h"

using namespace so_flow ;

//**********************************************************************************************
serial_node_walker::serial_node_walker( void_t ) 
{}

//**********************************************************************************************
serial_node_walker::serial_node_walker( this_rref_t )
{}

//**********************************************************************************************
serial_node_walker::~serial_node_walker( void_t )
{}

//**********************************************************************************************
serial_node_walker::this_ptr_t serial_node_walker::create( so_memory::purpose_cref_t p )
{
    return so_flow::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
void_t serial_node_walker::destroy( this_ptr_t ptr )
{
    so_flow::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
void_t serial_node_walker::walk( so_flow::inode::nodes_rref_t init_nodes ) 
{
    // trigger
    {
        so_flow::inode::nodes_t nodes = init_nodes ;
        while(nodes.size() != 0)
        {
            so_flow::inode::nodes_t inner_nodes = std::move( nodes ) ;
            for(so_flow::inode_ptr_t nptr : inner_nodes)
            {
                nptr->on_trigger( nodes ) ;
            }
        }
    }

    // update
    {
        so_flow::inode::nodes_t nodes = init_nodes ;
        while(nodes.size() != 0)
        {
            so_flow::inode::nodes_t inner_nodes = std::move( nodes ) ;
            for(so_flow::inode_ptr_t nptr : inner_nodes)
            {
                nptr->on_update( nodes ) ;
            }
        }
    }
}

//**********************************************************************************************
void_t serial_node_walker::walk( so_flow::inode::nodes_cref_t init_nodes ) 
{
    so_flow::inode::nodes_t nodes = init_nodes ;
    this_t::walk( std::move(nodes) ) ;
}

//**********************************************************************************************
void_t serial_node_walker::destroy( void_t )
{
    this_t::destroy( this ) ;
}
