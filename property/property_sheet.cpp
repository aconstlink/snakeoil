//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "property_sheet.h"

#include "iproperty.h"
#include "visitor/property_construction_visitor.h"

using namespace so_property ;


//*************************************************************************************
property_sheet::property_sheet( void_t )
{}

//*************************************************************************************
property_sheet::property_sheet( so_property::iproperty_ptr_t ptr )
{
    this_t::process_and_take_graph( ptr ) ;
}

//*************************************************************************************
property_sheet::property_sheet( this_rref_t rhv )
{
    _props = std::move( rhv._props ) ;
    so_move_member_ptr( _graph, rhv ) ;
}

//*************************************************************************************
property_sheet::~property_sheet( void_t )
{
    if( so_core::is_not_nullptr( _graph ) )
    {
        _graph->destroy() ;

    }
}

//*************************************************************************************
property_sheet::this_ptr_t property_sheet::create( so_memory::purpose_cref_t p )
{
    return so_property::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
property_sheet::this_ptr_t property_sheet::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_property::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t property_sheet::destroy( this_ptr_t ptr )
{
    so_property::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t property_sheet::process_and_take_graph( so_property::iproperty_ptr_t graph )
{
    if( so_core::is_nullptr( graph ) )
        return ;

    so_property::property_construction_visitor v( &_props ) ;
    graph->apply( &v ) ;

    _graph = graph ;
}

//*************************************************************************************
void_t property_sheet::apply( so_property::ivisitor_ptr_t vptr )
{
    if( so_core::is_nullptr( _graph ) )
        return ;

    _graph->apply( vptr ) ;
}