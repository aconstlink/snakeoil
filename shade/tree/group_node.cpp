//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "group_node.h"

using namespace so_shade ;
using namespace so_shade::so_tree ;

//**********************************************************************************************
group::group( void_t ) 
{

}

//**********************************************************************************************
group::group( this_rref_t rhv )
{
    _nodes = std::move( rhv._nodes ) ;
}

//**********************************************************************************************
group::~group( void_t )
{
    for( auto * nptr : _nodes )
    {
        nptr->destroy() ;
    }
}

//**********************************************************************************************
bool_t group::find( node_ptr_t nptr ) const 
{
    if( so_core::is_nullptr(nptr) )
        return false ;

    auto const iter = std::find( _nodes.begin(), _nodes.end(), nptr ) ;
    return iter != _nodes.end() ? true : false ;
}

//**********************************************************************************************
so_shade::result group::add_node( node_ptr_t nptr )
{
    if( so_core::is_nullptr( nptr ) )
        return so_shade::invalid_argument ;

    auto iter = std::find( _nodes.begin(), _nodes.end(), nptr ) ;
    if( iter != _nodes.end() )
        return so_shade::ok ;

    _nodes.push_back( nptr ) ;

    return so_shade::ok ;
}

//**********************************************************************************************
so_shade::result group::exchange( node_ptr_t which_ptr, node_ptr_t with_ptr ) 
{
    if( so_core::is_nullptr(which_ptr) )
        return so_shade::invalid_argument ;

    if( so_core::is_nullptr(with_ptr) )
        return so_shade::invalid_argument ;    

    auto iter = std::find( _nodes.begin(), _nodes.end(), which_ptr ) ;
    if( iter == _nodes.end() )
        return so_shade::invalid_argument ;

    *iter = with_ptr ;

    return so_shade::ok ;
}
