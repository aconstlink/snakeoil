//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "decorator.h"

#include "../visitor/ivisitor.h"

#include <snakeoil/core/macros/move.h>

using namespace so_scene ;
using namespace so_scene::so_node ;

//*************************************************************************
decorator::decorator( void_t ) 
{}

//*************************************************************************
decorator::decorator( this_rref_t rhv ) : base_t( std::move(rhv) )
{
    so_move_member_ptr( _decorated, rhv ) ;

    if(_decorated != nullptr )
        _decorated->set_parent(this) ;
}

//*************************************************************************
decorator::decorator( iparent_ptr_t parent_ptr ) : node( parent_ptr )
{}

//*************************************************************************
decorator::~decorator( void_t )
{
   if( _decorated != nullptr )
   {
       _decorated->set_parent(nullptr) ;
       _decorated->destroy() ;
   }
       
}

//*************************************************************************
so_scene::result decorator::apply( so_scene::so_visitor::ivisitor_ptr_t vptr )
{
    so_scene::result r = vptr->visit( this ) ;
    if( so_scene::success(r) )
    {
        r = traverse_decorated( vptr ) ;
    }
    
    return vptr->post_visit(this) ;
}

//*************************************************************************
so_scene::result decorator::replace( node_ptr_t which_ptr, node_ptr_t with_ptr )
{
    if( which_ptr == with_ptr ) 
        return so_scene::ok ;

    if( which_ptr == nullptr ) 
        return so_scene::invalid_argument ;

    if( which_ptr != _decorated ) 
        return so_scene::invalid_argument ;

    if( _decorated != nullptr ) 
        _decorated->set_parent( nullptr ) ;

    _decorated = with_ptr ;

    if( _decorated != nullptr ) 
        _decorated->set_parent( this ) ;

    return so_scene::ok ;
}

//*************************************************************************
so_scene::result decorator::detach( node_ptr_t which_ptr )
{
    if( which_ptr == nullptr ) 
        return so_scene::ok ;

    if( which_ptr != _decorated ) 
        return so_scene::invalid_argument ;

    _decorated->set_parent( nullptr ) ;
    _decorated = nullptr ;

    return so_scene::ok ;
}

//*************************************************************************
void_t decorator::set_decorated( node_ptr_t nptr, bool_t reset_parent )
{
    if( _decorated != nullptr && reset_parent ) 
        _decorated->set_parent(nullptr) ;

    _decorated = nptr ;

    if( _decorated != nullptr ) 
        _decorated->set_parent(this) ;
}

//*************************************************************************
void_t decorator::set_decorated( node_ptr_t nptr ) 
{
    set_decorated( nptr, true ) ;
}

//*************************************************************************
node_ptr_t decorator::get_decorated( void_t )
{
    return _decorated ;
}

//*************************************************************************
node_cptr_t decorator::get_decorated( void_t ) const 
{
    return _decorated ;
}

//*************************************************************************
so_scene::result decorator::traverse_decorated( so_scene::so_visitor::ivisitor_ptr_t ptr )
{
    so_scene::result r = so_scene::repeat ;
    while( r == so_scene::repeat )
    {
        if( _decorated == nullptr ) break ;
        r = _decorated->apply(ptr) ;
    }
    return r ;
}

//*************************************************************************
decorator::this_ref_t decorator::operator = ( this_rref_t rhv ) 
{
    base_t::operator=(std::move(rhv)) ;
    so_move_member_ptr( _decorated, rhv ) ;
    return *this ;
}
