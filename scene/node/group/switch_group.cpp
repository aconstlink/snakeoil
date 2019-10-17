//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "switch_group.h"

#include "../../visitor/ivisitor.h"

using namespace so_scene ;
using namespace so_scene::so_node ;

//*******************************************************************
switch_group::this_ptr_t switch_group::create( so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( this_t(), purp ) ;
}

//*******************************************************************
switch_group::this_ptr_t switch_group::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( std::move(rhv), purp ) ;
}

//*******************************************************************
void_t switch_group::destroy( this_ptr_t ptr ) 
{
    so_scene::memory::dealloc( ptr ) ;
}

//*******************************************************************
switch_group::switch_group( void_t ) 
{
}

//*******************************************************************
switch_group::switch_group( iparent_ptr_t ptr ) : base_t(ptr) 
{
}

//*******************************************************************
switch_group::switch_group( this_rref_t rhv ) : base_t( std::move(rhv))
{
    _actives = std::move(rhv._actives) ;
}

//*******************************************************************
switch_group::~switch_group( void_t ) 
{
}

//*******************************************************************
so_scene::result switch_group::switch_child( size_t child_i, bool_t active ) 
{
    if( child_i >= base_t::get_num_children() ) 
        return so_scene::result::invalid_argument ;

    _actives[child_i] = active ;

    return so_scene::result::ok ;
}

//*******************************************************************
so_scene::result switch_group::switch_off_all( void_t )  
{
    for( size_t i=0; i<_actives.size(); ++i ) _actives[i] = false ;
    return so_scene::result::ok ;
}

//*******************************************************************
so_scene::result switch_group::switch_on( node_ptr_t nptr ) 
{
    if( nptr == nullptr ) 
        return so_scene::result::invalid_argument ;

    size_t const index = base_t::find_index( nptr ) ;
    if( index == size_t(-1) ) 
        return so_scene::result::failed ;

    _actives[index] = true ;

    return so_scene::result::ok ;
}

//*******************************************************************
so_scene::result switch_group::add_child( node_ptr_t nptr, bool_t active ) 
{
    if( nptr == nullptr ) 
        return so_scene::result::invalid_argument ;

    if( so_scene::no_success( base_t::add_child( nptr ) ) ) 
        return so_scene::result::failed ;
        
    _actives.push_back( active ) ;

    return so_scene::result::ok ;
}

//*******************************************************************
so_scene::result switch_group::remove_child( node_ptr_t nptr ) 
{
    if( nptr == nullptr ) 
        return so_scene::result::invalid_argument ;

    size_t const index = base_t::remove_child( nptr ) ;
    if( index == size_t(-1) ) 
        return so_scene::result::ok ;
    
    _actives.erase( _actives.begin() + index ) ;

    return so_scene::result::ok ;
}

//*******************************************************************
so_scene::result switch_group::apply( so_scene::so_visitor::ivisitor_ptr_t vptr ) 
{
    vptr->visit( this ) ;
    base_t::apply( vptr, [=]( size_t i ) { return this->_actives[i] ; } ) ;
    return vptr->post_visit( this ) ;
}

//*******************************************************************
so_scene::result switch_group::detach( node_ptr_t which_ptr ) 
{
    size_t index = base_t::remove_child( which_ptr ) ;
    if( index == size_t(-1) ) 
        return so_scene::result::failed ;

    _actives.erase( _actives.begin() + index ) ;
    return so_scene::result::ok ;
}

//*******************************************************************
void_t switch_group::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
