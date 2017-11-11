//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "group.h"

#include "../visitor/ivisitor.h"

#include <snakeoil/log/global.h>
#include <algorithm>

using namespace so_ui ;
using namespace so_ui::so_node ;

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
    for( auto & item : _children )
        item.node_ptr->set_parent( this ) ;
}

//*******************************************************************
group::~group( void_t )
{
    for( auto & item : _children )
    { 
        item.node_ptr->set_parent(nullptr) ; 
        item.node_ptr->destroy() ;

        if( so_core::is_not_nullptr( item.usr_ptr ) )
            item.usr_ptr->destroy() ;
    }
}

//*******************************************************************
so_ui::result group::apply( so_ui::so_visitor::ivisitor_ptr_t vptr )
{
    return this_t::apply( vptr, [=]( size_t ){ return true ; } ) ;
}

//*******************************************************************
so_ui::result group::apply( so_ui::so_visitor::ivisitor_ptr_t vptr, 
    traverse_predicate_t pred )
{
    so_ui::result r = vptr->visit( this ) ;

    switch( r ) 
    {
    case so_ui::ok:
        traverse_children( vptr, pred ) ;
        break ;
    default:
        //so_log::global::warning("[group_node::apply] : unsupported case") ;
        break ;
    }
    
    return vptr->post_visit(this) ;
}

#if 0
//*******************************************************************
so_ui::result group::replace( node_ptr_t which_ptr, node_ptr_t with_ptr )
{
    if( with_ptr == nullptr ) 
        return so_ui::invalid_argument ;

    if( which_ptr == with_ptr ) 
        return so_ui::ok ;

    auto found = std::find_if( _children.begin(), _children.end(), [&]( this_t::per_child_data_cref_t d ) 
        { return d.node_ptr == which_ptr ; } ) ;

    if( found == _children.end() ) 
        return so_ui::invalid_argument ;

    which_ptr->set_parent( nullptr ) ;
    with_ptr->set_parent( this ) ;

    size_t const n = found - _children.begin() ;
    _children[n].node_ptr = with_ptr ;

    return so_ui::ok ;
}

//*******************************************************************
so_ui::result group::detach( node_ptr_t which_ptr )
{
    if( which_ptr == nullptr ) 
        return so_ui::ok ;

    auto found = std::find_if( _children.begin(), _children.end(), [&] ( this_t::per_child_data_cref_t d )
        { return d.node_ptr == which_ptr ; } ) ;

    if( found == _children.end() ) 
        return so_ui::invalid_argument ;

    which_ptr->set_parent( nullptr ) ;

    if( so_core::is_not_nullptr( found->usr_ptr ) )
        found->usr_ptr->destroy() ;

    _children.erase( found ) ;

    return so_ui::ok ;
}
#endif

//*******************************************************************
size_t group::get_num_children( void_t ) const
{
    return _children.size() ;
}

//*******************************************************************
node_ptr_t group::get_child( size_t const child_i ) 
{
    if( child_i >= _children.size() ) 
        return nullptr ;

    return _children[child_i].node_ptr ;
}

//*******************************************************************
group::iuser_data_ptr_t group::get_user_data( size_t const child_i )
{
    if( child_i >= _children.size() )
        return nullptr ;

    return _children[ child_i ].usr_ptr ;
}

//*******************************************************************
group::iuser_data_cptr_t group::get_user_data( size_t const child_i ) const
{
    if( child_i >= _children.size() )
        return nullptr ;

    return _children[ child_i ].usr_ptr ;
}

//*******************************************************************
group::per_child_data_t group::get_child_data( size_t const child_i ) const
{
    if( child_i >= _children.size() )
        return this_t::per_child_data() ;

    return _children[ child_i ] ;
}

//*******************************************************************
size_t group::find_index( node_ptr_t nptr ) const 
{
    if( nptr == nullptr )
        return size_t(-1) ;

    auto const found = std::find_if( _children.begin(), _children.end(), [&] ( this_t::per_child_data_cref_t d )
        { return d.node_ptr == nptr ; } ) ;
    size_t const index = found - _children.begin() ;
    
    return index < _children.size() ? index : size_t(-1) ;
}

//*******************************************************************
so_ui::result group::add_child( node_ptr_t nptr ) 
{
    return this_t::add_child( nptr, nullptr ) ;
}

//*******************************************************************
so_ui::result group::add_child( node_ptr_t nptr, this_t::iuser_data_ptr_t uptr )
{
    if( nptr == nullptr )
        return so_ui::invalid_argument ;

    auto const found = std::find_if( _children.begin(), _children.end(), [&] ( this_t::per_child_data_cref_t d )
    { return d.node_ptr == nptr ; } ) ;

    if( found != _children.end() )
        return so_ui::invalid_argument ;

    this_t::per_child_data_t pcd ;
    pcd.node_ptr = nptr ;
    pcd.usr_ptr = uptr ;

    _children.push_back( pcd ) ;
    nptr->set_parent( this ) ;

    return so_ui::ok ;
}

//*******************************************************************
size_t group::remove_child( node_ptr_t nptr ) 
{
    auto found = std::find_if( _children.begin(), _children.end(), [&] ( this_t::per_child_data_cref_t d )
        { return d.node_ptr == nptr ; } ) ;

    if( found == _children.end() ) 
        return size_t(-1) ;
    
    size_t const index = found - _children.begin() ;
    
    found->node_ptr->set_parent(nullptr) ;
    found->node_ptr->destroy() ;

    if( so_core::is_not_nullptr( found->usr_ptr ) )
        found->usr_ptr->destroy() ;

    _children.erase(found) ;
    
    return index ;
}

//*******************************************************************
void_t group::traverse_children( so_ui::so_visitor::ivisitor_ptr_t ptr, 
    traverse_predicate_t func ) 
{
    for( size_t i=0; i<_children.size(); ++i )
    {
        if( !func( i ) ) continue ;
        
        so_ui::result r = so_ui::repeat ;
        while( r == so_ui::repeat )
        {
            auto * nptr = _children[i].node_ptr ;
            r = nptr->apply( ptr ) ;
        }               
    }
}

//*******************************************************************
void_t group::traverse_children( so_ui::so_visitor::ivisitor_ptr_t ptr,
    traverse_predicate_t func, so_ui::result const res )
{
    if( so_ui::success( res ) )
        this_t::traverse_children( ptr, func ) ;
}

//*******************************************************************
void_t group::destroy( void_t ) 
{
    for( auto & item : _children ) 
    {
        item.node_ptr->destroy() ;
        if( so_core::is_not_nullptr( item.usr_ptr ) )
            item.usr_ptr->destroy() ;
    }
}
