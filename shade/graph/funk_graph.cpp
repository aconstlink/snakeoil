//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "funk_graph.h"
#include "funk_node.h"

#include <algorithm>

using namespace so_shade ;
using namespace so_shade::so_graph ;

//**********************************************************************************************
funk_graph::funk_graph( void_t )
{
}

//**********************************************************************************************
funk_graph::funk_graph( this_rref_t rhv )
{
    _nodes = rhv._nodes ;
}

//**********************************************************************************************
funk_graph::~funk_graph( void_t )
{
    for( auto & item : _nodes )
    {
        item.funk_nptr->destroy() ;
    }
}

//**********************************************************************************************
funk_graph::this_ptr_t funk_graph::create( so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
funk_graph::this_ptr_t funk_graph::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t funk_graph::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
so_shade::result funk_graph::add_node( so_shade::so_graph::funk_node_ptr_t funk_ptr )
{
    if( so_core::is_nullptr(funk_ptr) )
        return so_shade::invalid_pointer ;

    so_std::string_t const name = std::to_string( _counter++ ) + "." + funk_ptr->get_name() ;  

    funk_item_t fi ;
    fi.funk_nptr = funk_ptr ;
    fi.name = std::move(name) ;

    _nodes.push_back( std::move(fi) ) ;

    return so_shade::ok ;
}

//**********************************************************************************************
so_shade::so_graph::funk_node_ptr_t funk_graph::find_node( so_std::string_cref_t name )
{
    return const_cast<funk_node_ptr_t>( const_cast<this_cptr_t>(this)->find_node( name ) ) ;
}

//**********************************************************************************************
so_shade::so_graph::funk_node_cptr_t funk_graph::find_node( so_std::string_cref_t name ) const
{
    auto iter = std::find_if( _nodes.begin(), _nodes.end(), [&] ( funk_item_cref_t item )
    {
        return item.name == name;
    } );

    return iter == _nodes.end() ? nullptr : iter->funk_nptr ;
}

//**********************************************************************************************
void_t funk_graph::for_each( iter_funk_t funk ) 
{
    for( auto & item : _nodes )
    {
        funk( item ) ;
    }
}
