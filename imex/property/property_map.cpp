//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "property_map.h"

#include <snakeoil/property/property_sheet.h>

using namespace so_imex ;

//*************************************************************************************
property_map::property_map( void_t ) 
{}

//*************************************************************************************
property_map::property_map( this_rref_t rhv )
{
    *this = std::move( rhv ) ;
}

//*************************************************************************************
property_map::~property_map( void_t )
{
    for( auto & item : _props)
    { 
        so_property::property_sheet::destroy( item.second.props_import ) ;
        so_property::property_sheet::destroy( item.second.props_export ) ;
    }
}

//*************************************************************************************
property_map::this_ref_t property_map::operator = ( this_rref_t rhv )
{
    _props = std::move( rhv._props ) ;
    return *this ;
}

//*************************************************************************************
bool_t property_map::add_property_sheet( so_std::string_cref_t key, so_std::utf8_cref_t name,
    so_std::utf8_cref_t desc, so_property::property_sheet_ptr_t imp_ptr, 
    so_property::property_sheet_ptr_t exp_ptr )
{
    if( so_core::is_nullptr( imp_ptr ) ||
        so_core::is_nullptr( exp_ptr ) )
        return false ;

    {
        auto const iter = _props.find( key ) ;
        if( iter != _props.end() )
        {
            return false ;
        }
    }

    this_t::data_t d ;
    d.desc = desc ;
    d.name = name ;
    d.props_import = imp_ptr ;
    d.props_export = exp_ptr ;

    _props[ key ] = d ;

    return true ;
}

//*************************************************************************************
bool_t property_map::add_property_sheet( so_std::string_cref_t key, this_t::data_rref_t d )
{
    if( so_core::is_nullptr( d.props_import ) ||
        so_core::is_nullptr( d.props_export ) )
        return false ;

    {
        auto const iter = _props.find( key ) ;
        if( iter != _props.end() )
        {
            return false ;
        }
    }

    _props[ key ] = std::move(d) ;

    return true ;
}

//*************************************************************************************
void_t property_map::for_each( for_each_funk_t funk )
{
    for( auto & item : _props )
    {
        funk( item.first, item.second ) ;
    }
}

//*************************************************************************************