//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "column_layout.h"

#include "area_2d.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*******************************************************************
void_t column_layout::user_data::destroy( void_t )
{
    so_ui::memory::dealloc( this ) ;
}

//*******************************************************************
column_layout::this_ptr_t column_layout::create( so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*******************************************************************
column_layout::this_ptr_t column_layout::create( this_rref_t rhv, so_memory::purpose_cref_t purp )
{
    return so_ui::memory::alloc( std::move( rhv ), purp ) ;
}

//*******************************************************************
void_t column_layout::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*******************************************************************
column_layout::column_layout( void_t )
{}

//*******************************************************************
column_layout::column_layout( iparent_ptr_t ptr ) : base_t( ptr )
{}

//*******************************************************************
column_layout::column_layout( this_rref_t rhv ) : base_t( std::move( rhv ) )
{}

//*******************************************************************
column_layout::~column_layout( void_t )
{}

//*******************************************************************
so_ui::result column_layout::add_child( node_ptr_t nptr, float_t const weight )
{
    auto * uptr = so_ui::memory::alloc( this_t::user_data_t(),
        "[column_layout::add_child] : user_data" ) ;

    uptr->weight = std::abs( weight ) ;
    uptr->nptr = nptr ;

    {
        auto * tptr = so_ui::so_node::area_2d::create(
            so_ui::so_node::area_2d(), "[row_layout::add_child] : transform_2d" ) ;

        tptr->set_decorated( nptr ) ;

        auto const res = base_t::add_child( tptr, uptr ) ;
        if( so_ui::no_success( res ) )
            so_ui::memory::dealloc( uptr ) ;
    }

    return so_ui::ok ;
}

//*******************************************************************
float_t column_layout::get_max_weight( void_t ) const
{
    float_t max_weight = 0.0f ;

    for( size_t i = 0; i < this_t::get_num_children(); ++i )
    {
        auto const pcd = this_t::get_child_data( i ) ;
        auto * uptr = reinterpret_cast< this_t::user_data_ptr_t >( pcd.usr_ptr ) ;
        max_weight += uptr->weight ;
    }

    return max_weight ;
}

//*******************************************************************
float_t column_layout::get_weight( size_t const child_i ) const
{
    return reinterpret_cast< this_t::user_data_cptr_t >(
        this_t::get_user_data( child_i ) )->weight ;
}

//*******************************************************************
size_t column_layout::remove_child( node_ptr_t nptr )
{
    return base_t::remove_child( nptr ) ;
}

//*******************************************************************
so_ui::result column_layout::apply( so_ui::so_visitor::ivisitor_ptr_t vptr )
{
    auto res = vptr->visit( this ) ;

    this_t::traverse_children( vptr, [=] ( size_t ) { return true ; }, res ) ;

    return vptr->post_visit( this ) ;
}

//*******************************************************************
void_t column_layout::destroy( void_t )
{
    this_t::destroy( this ) ;
}
