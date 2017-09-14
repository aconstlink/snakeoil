//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "group.h"

#include "../visitor/ivisitor.h"

#include <snakeoil/log/global.h>
#include <algorithm>

using namespace so_scene ;
using namespace so_scene::so_node ;

//*******************************************************************
group::group( void_t ) 
{}

//*******************************************************************
group::group( iparent_ptr_t parent_ptr ) : node(parent_ptr)
{}

//*******************************************************************
group::group( this_rref_t rhv ) : base_t(std::move(rhv))
{
    _children = std::move( rhv._children ) ;
    for( auto nptr : _children )
        nptr->set_parent( this ) ;
}

//*******************************************************************
group::~group( void_t )
{
    for( auto * c : _children )
    { 
        c->set_parent(nullptr) ; 
        c->destroy() ;
    }
}

//*******************************************************************
so_scene::result group::apply( so_scene::so_visitor::ivisitor_ptr_t vptr )
{
    return this_t::apply( vptr, [=]( size_t ){ return true ; } ) ;
}

//*******************************************************************
so_scene::result group::apply( so_scene::so_visitor::ivisitor_ptr_t vptr, 
    traverse_predicate_t pred )
{
    so_scene::result r = vptr->visit( this ) ;

    switch( r ) 
    {
    case so_scene::ok:
        traverse_children( vptr, pred ) ;
        break ;
    default:
        so_log::global::warning("[group_node::apply] : unsupported case") ;
        break ;
    }
    
    return vptr->post_visit(this) ;
}

//*******************************************************************
so_scene::result group::replace( node_ptr_t which_ptr, node_ptr_t with_ptr )
{
    if( with_ptr == nullptr ) 
        return so_scene::invalid_argument ;

    if( which_ptr == with_ptr ) 
        return so_scene::ok ;

    auto found = std::find( _children.begin(), _children.end(), which_ptr ) ;
    if( found == _children.end() ) 
        return so_scene::invalid_argument ;

    which_ptr->set_parent( nullptr ) ;
    with_ptr->set_parent( this ) ;

    size_t const n = found - _children.begin() ;
    _children[n] = with_ptr ;

    return so_scene::ok ;
}

//*******************************************************************
so_scene::result group::detach( node_ptr_t which_ptr )
{
    if( which_ptr == nullptr ) 
        return so_scene::ok ;

    auto found = std::find( _children.begin(), _children.end(), which_ptr ) ;
    if( found == _children.end() ) 
        return so_scene::invalid_argument ;

    which_ptr->set_parent( nullptr ) ;
    _children.erase( found ) ;

    return so_scene::ok ;
}

//*******************************************************************
size_t group::get_num_children( void_t ) const
{
    return _children.size() ;
}

//*******************************************************************
node_ptr_t group::get_child( size_t child_i ) 
{
    if( child_i >= _children.size() ) 
        return nullptr ;

    return _children[child_i] ;
}

//*******************************************************************
size_t group::find_index( node_ptr_t nptr ) const 
{
    if( nptr == nullptr )
        return size_t(-1) ;

    auto found = std::find( _children.begin(), _children.end(), nptr ) ;
    size_t const index = found - _children.begin() ;
    
    return index < _children.size() ? index : size_t(-1) ;
}

//*******************************************************************
so_scene::result group::add_child( node_ptr_t nptr ) 
{
    if( nptr == nullptr ) 
        return so_scene::invalid_argument ;

    so_thread::lock_guard_t lk(_mtx) ;

    auto found = std::find( _children.begin(), _children.end(), nptr ) ;
    if( found != _children.end() ) 
        return so_scene::invalid_argument ;

    _children.push_back(nptr) ;
    nptr->set_parent(this) ;

    return so_scene::ok ;
}

//*******************************************************************
size_t group::remove_child( node_ptr_t nptr ) 
{
    auto found = std::find( _children.begin(), _children.end(), nptr ) ;
    if( found == _children.end() ) 
        return size_t(-1) ;
    
    size_t const index = found - _children.begin() ;
    
    (*found)->set_parent(nullptr) ;
    _children.erase(found) ;
    
    return index ;
}

//*******************************************************************
void_t group::traverse_children( so_scene::so_visitor::ivisitor_ptr_t ptr, 
    traverse_predicate_t func ) 
{
    for( size_t i=0; i<_children.size(); ++i )
    {
        if( !func( i ) ) continue ;
        
        so_scene::result r = so_scene::repeat ;
        while( r == so_scene::repeat )
        {
            auto * nptr = _children[i] ;
            r = nptr->apply( ptr ) ;
        }               
    }
}

//*******************************************************************
void_t group::destroy( void_t ) 
{
    for( auto * c_ptr : _children ) 
    {
        c_ptr->destroy() ;
    }
}
