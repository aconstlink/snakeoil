//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "std_out_visitor.h"
#include "../properties/group_property.h"
#include "../properties/decorator_property.h"
#include "../properties/multi_choice_property.h"
#include "../properties/single_choice_property.h"
#include "../properties/generic_property.hpp"

#include <snakeoil/log/global.h>

using namespace so_property ;

//*************************************************************************************
std_out_visitor::std_out_visitor( void_t )
{}

//*************************************************************************************
std_out_visitor::std_out_visitor( this_rref_t rhv )
{
    _level = rhv._level ;
}

//*************************************************************************************
std_out_visitor::~std_out_visitor( void_t )
{}

//*************************************************************************************
void_t std_out_visitor::visit( group_property_ptr_t pptr )
{
    this_t::print_property_name( pptr, "group" ) ;
    ++_level ;
}

//*************************************************************************************
void_t std_out_visitor::post_visit( group_property_ptr_t pptr )
{
    --_level ;
    this_t::print_property_name( pptr, "group" ) ;
}

//*************************************************************************************
void_t std_out_visitor::visit( decorator_property_ptr_t pptr )
{
    this_t::print_property_name( pptr, "decorator" ) ;
    ++_level ;
}

//*************************************************************************************
void_t std_out_visitor::post_visit( decorator_property_ptr_t pptr )
{
    --_level ;
    this_t::print_property_name( pptr, "decorator" ) ;
}

//*************************************************************************************
void_t std_out_visitor::visit( single_choice_property_ptr_t pptr )
{
    this_t::print_property_name( pptr, "single_choice" ) ;
    ++_level ;
}

//*************************************************************************************
void_t std_out_visitor::post_visit( single_choice_property_ptr_t pptr )
{
    --_level ;
    this_t::print_property_name( pptr, "single_choice" ) ;
}

//*************************************************************************************
void_t std_out_visitor::visit( multi_choice_property_ptr_t pptr )
{
    this_t::print_property_name( pptr, "multi_choice" ) ;
    ++_level ;
}

//*************************************************************************************
void_t std_out_visitor::post_visit( multi_choice_property_ptr_t pptr )
{
    --_level ;
    this_t::print_property_name( pptr, "multi_choice" ) ;
}

//*************************************************************************************
void_t std_out_visitor::visit( so_property::iproperty_ptr_t pptr )
{
    this_t::print_property_name( pptr, "iproperty" ) ;
}

//*************************************************************************************
void_t std_out_visitor::print_property_name( so_property::iproperty_ptr_t pptr, 
    so_std::utf8_cref_t clss_name ) const
{
    so_std::utf8_t const string = this_t::level_to_string() + pptr->get_name() + " ["+ clss_name+"]" ;
    so_log::global::status( string ) ;
}

//*************************************************************************************
so_std::utf8_t std_out_visitor::level_to_string( void_t ) const
{
    so_std::utf8_t str ;
    for( size_t i=0; i<_level; ++i )
    {
        str += "    " ;
    }
    return std::move( str ) ;
}

//*************************************************************************************