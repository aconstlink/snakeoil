//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "property_construction_visitor.h"

#include "../properties/group_property.h"
#include "../properties/decorator_property.h"
#include "../properties/generic_property.hpp"
#include "../properties/multi_choice_property.h"
#include "../properties/single_choice_property.h"

#include <snakeoil/log/global.h>

using namespace so_property ;

//*************************************************************************************
property_construction_visitor::property_construction_visitor( so_property::property_map_ptr_t mptr )
{
    _map = mptr ;
}

//*************************************************************************************
property_construction_visitor::property_construction_visitor( this_rref_t rhv )
{
    so_move( _name_stack, rhv )  ;
    so_move( _cur_eval, rhv ) ;
    so_move_member_ptr( _map, rhv ) ;
}

//*************************************************************************************
property_construction_visitor::~property_construction_visitor( void_t ) 
{}

//*************************************************************************************
so_property::property_map_ref_t property_construction_visitor::map( void_t )
{
    return *_map ;
}

//*************************************************************************************
void_t property_construction_visitor::visit( so_property::iproperty_ptr_t pptr )
{
    so_std::utf8_t cur_name = this_t::construct_name( pptr->get_name() ) ;
    this_t::insert( cur_name, pptr ) ;
}

//*************************************************************************************
void_t property_construction_visitor::visit( group_property_ptr_t pptr )
{
    auto const & name = pptr->get_name() ;

    _name_stack.push( name ) ;
    _cur_eval = this_t::construct_name() ;
}

//*************************************************************************************
void_t property_construction_visitor::post_visit( group_property_ptr_t pptr )
{
    _name_stack.pop() ;
    _cur_eval = this_t::construct_name() ;
}

//*************************************************************************************
void_t property_construction_visitor::visit( decorator_property_ptr_t pptr )
{
    auto const & name = pptr->get_name() ;

    _name_stack.push( name ) ;
    _cur_eval = this_t::construct_name() ;
}

//*************************************************************************************
void_t property_construction_visitor::post_visit( decorator_property_ptr_t pptr )
{
    _name_stack.pop() ;
    _cur_eval = this_t::construct_name() ;
}

//*************************************************************************************
void_t property_construction_visitor::visit( single_choice_property_ptr_t pptr )
{
    auto const & name = pptr->get_name() ;

    _name_stack.push( name ) ;
    _cur_eval = this_t::construct_name() ;
}

//*************************************************************************************
void_t property_construction_visitor::post_visit( single_choice_property_ptr_t )
{
    _name_stack.pop() ;
    _cur_eval = this_t::construct_name() ;
}

//*************************************************************************************
void_t property_construction_visitor::visit( multi_choice_property_ptr_t pptr )
{
    auto const & name = pptr->get_name() ;

    _name_stack.push( name ) ;
    _cur_eval = this_t::construct_name() ;
}

//*************************************************************************************
void_t property_construction_visitor::post_visit( multi_choice_property_ptr_t pptr )
{
    _name_stack.pop() ;
    _cur_eval = this_t::construct_name() ;
}

//*************************************************************************************
so_std::utf8_t property_construction_visitor::construct_name( void_t )
{
    so_std::utf8_t ret_name ;

    _name_stack.bottom_to_top( 
        [&]( so_std::utf8_cref_t name, bool_t const is_bottom, bool_t const is_top )
    {
        ret_name += name ;
        ret_name += so_core::is_not( is_top ) ? so_std::utf8_t( "." ) : so_std::utf8_t( "" ) ;
    } ) ;

    return std::move( ret_name ) ;
}

//*************************************************************************************
so_std::utf8_t property_construction_visitor::construct_name( so_std::utf8_cref_t end_name )
{
    return _cur_eval + "." + end_name ;
}

//*************************************************************************************
bool_t property_construction_visitor::insert( so_std::utf8_cref_t name, so_property::iproperty_ptr_t ptr )
{
    auto const iter = this_t::map().find( name ) ;
    if( iter != this_t::map().end() )
    {
        so_log::global::warning( "[property_construction_visitor::insert] : "
            "can not insert name : " + name ) ;
        return false ;
    }

    this_t::map()[ name ] = ptr ;

    return true ;
}