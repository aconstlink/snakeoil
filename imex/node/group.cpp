//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "group.h"

#include <snakeoil/log/global.h>

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
group::group( void_t ) 
{}

//*************************************************************************************
group::group( this_rref_t rhv ) : node( std::move(rhv) )
{
    _children = std::move( rhv._children ) ;
}

//*************************************************************************************
group::~group( void_t ) 
{
    for( auto * ptr : _children )
    {
        ptr->destroy() ;
    }
}

//*************************************************************************************
size_t group::get_num_children( void_t ) const 
{
    return _children.size() ;
}

//*************************************************************************************
node_ptr_t group::get_child( size_t i ) 
{
    return i < _children.size() ? _children[i] : nullptr ;
}

//*************************************************************************************
bool_t group::contains( node_ptr_t nptr ) const 
{
    auto const found = std::find( _children.begin(), _children.end(), nptr ) ;
    return found != _children.end() ;
}

//*************************************************************************************
bool_t group::add_child( node_ptr_t nptr ) 
{
    if( nptr == nullptr ) return false ;

    auto found = std::find( _children.begin(), _children.end(), nptr ) ;
    if( found != _children.end() ) return false ;
        
    nptr->set_parent( this ) ;

    _children.push_back( nptr ) ;

    return true ;
}

//*************************************************************************************
bool_t group::remove_child( node_ptr_t nptr ) 
{
    if(nptr == nullptr) return false ;

    auto found = std::find( _children.begin(), _children.end(), nptr ) ;
    if(found == _children.end()) return true ;

    _children.erase( found ) ;

    return true ;
}

//*************************************************************************************
bool_t group::exchange_child( node_ptr_t old_ptr, node_ptr_t new_ptr ) 
{
    if( old_ptr == nullptr )
        return this_t::add_child( new_ptr ) ;    

    auto found = std::find( _children.begin(), _children.end(), old_ptr ) ;
    if( found == _children.end() )
        return this_t::add_child( new_ptr ) ;

    size_t const index = found - _children.begin() ;
    _children[index] = new_ptr ;

    return true ;
}
