//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "render_state.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/log/log.h>

using namespace so_scene ;
using namespace so_scene::so_node ;

//*************************************************************************************
render_state::render_state( void_t ) 
{}

//*************************************************************************************
render_state::render_state( this_rref_t rhv )  : base_t( std::move(rhv) )
{
    _af = rhv._af ;
    _ss = rhv._ss ;
}

//*************************************************************************************
render_state::render_state( this_cref_t rhv ) : base_t()
{
    _af = rhv._af ;
    _ss = rhv._ss ;
}

//*************************************************************************************
render_state::render_state( iparent_ptr_t pptr ) :  base_t(pptr)
{}

//*************************************************************************************
render_state::render_state( so_gpu::attribute_state_field_cref_t af, so_gpu::state_sets_cref_t ss ) :
    _af(af), _ss(ss)
{}

//*************************************************************************************
render_state::render_state( so_gpu::attribute_state_field_cref_t af, 
    so_gpu::state_sets_cref_t ss, iparent_ptr_t pptr ) : base_t(pptr), _af(af), _ss(ss)
{}

//*************************************************************************************
render_state::~render_state( void_t ) 
{}

//*************************************************************************************
render_state::this_ptr_t render_state::create( so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************************
render_state::this_ptr_t render_state::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************************
void_t render_state::destroy( this_ptr_t ptr ) 
{
    so_scene::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_scene::result render_state::apply( so_scene::so_visitor::ivisitor_ptr_t ptr ) 
{
    auto res = ptr->visit( this ) ;
    if( so_scene::no_success( res ) )
    {
        so_log::log::error( "[so_scene::render_config::apply] : visit" ) ;
    }

    base_t::traverse_decorated( ptr ) ;

    return ptr->post_visit( this ) ;
}

//*************************************************************************************
so_scene::result render_state::replace( node_ptr_t which, node_ptr_t with ) 
{
    return base_t::replace( which, with ) ;
}

//*************************************************************************************
so_scene::result render_state::detach( node_ptr_t which_ptr ) 
{
    return base_t::detach( which_ptr ) ;
}

//*************************************************************************************
so_gpu::attribute_state_field_cref_t render_state::get_attributes( void_t ) const 
{
    return _af ;
}

//*************************************************************************************
so_gpu::state_sets_cptr_t render_state::get_states( void_t ) const 
{
    return &_ss ;
}

//*************************************************************************************
void_t render_state::set_decorated( so_scene::so_node::node_ptr_t nptr ) 
{
    base_t::set_decorated( nptr ) ;
}

//*************************************************************************************
render_state::this_ref_t render_state::operator = ( this_rref_t rhv ) 
{
    base_t::operator=(std::move(rhv)) ;

    so_move( _af, rhv ) ;
    so_move( _ss, rhv ) ;

    return *this ;
}

//*************************************************************************************
void_t render_state::destroy( void_t ) 
{
    this_t::destroy( this )  ;
}
