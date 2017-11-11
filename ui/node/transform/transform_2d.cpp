//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "transform_2d.h"

#include "../../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*******************************************************************
transform_2d::transform_2d( void_t )
{
}

//*******************************************************************
transform_2d::transform_2d( this_t::trafo_cref_t trafo ) : _trafo( trafo )
{
    
}

//*******************************************************************
transform_2d::transform_2d( iparent_ptr_t parent_ptr ) : base_t( parent_ptr )
{
}

//*******************************************************************
transform_2d::transform_2d( node_ptr_t transformee_ptr, iparent_ptr_t parent_ptr ) : base_t( parent_ptr )
{
    this_t::set_decorated( transformee_ptr ) ;
}

//*******************************************************************
transform_2d::transform_2d( this_rref_t rhv ) : base_t( std::move( rhv ) )
{
    _trafo = rhv._trafo ;
}

//*******************************************************************
transform_2d::transform_2d( this_cref_t rhv ) : base_t()
{
    _trafo = rhv._trafo ;
}

//*******************************************************************
transform_2d::~transform_2d( void_t )
{}

//*******************************************************************
transform_2d::this_ptr_t transform_2d::create( so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*******************************************************************
transform_2d::this_ptr_t transform_2d::create( this_rref_t rhv, so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( std::move( rhv ), purp ) ;
}

//*******************************************************************
void_t transform_2d::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*******************************************************************
transform_2d::this_ref_t transform_2d::set_decorated( node_ptr_t transformee_ptr )
{
    base_t::set_decorated( transformee_ptr ) ;
    return *this ;
}

//*******************************************************************
void_t transform_2d::set_transformation( trafo_cref_t trafo )
{
    _trafo = trafo ;
}

//*******************************************************************
transform_2d::trafo_t transform_2d::compute_trafo( void_t ) const
{
    return _trafo ;
}

//*******************************************************************
transform_2d::trafo_cref_t transform_2d::get_transformation( void_t ) const
{
    return _trafo ;
}

//*******************************************************************
so_ui::result transform_2d::apply( so_ui::so_visitor::ivisitor_ptr_t vptr )
{
    auto res = vptr->visit( this ) ;
    so_log::global::error( so_ui::no_success( res ),
        "[so_ui::transform::apply] : visit" ) ;

    this_t::traverse_decorated( vptr ) ;

    return vptr->post_visit( this ) ;
}

//*******************************************************************
void_t transform_2d::destroy( void_t )
{
    this_t::destroy( this ) ;
}
