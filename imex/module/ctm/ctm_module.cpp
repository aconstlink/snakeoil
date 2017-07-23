//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "ctm_module.h"
#include "../../property/property_map.h"

#include <snakeoil/property/property_sheet.h>
#include <snakeoil/log/log.h>

using namespace so_imex ;

//*************************************************************************************
ctm_module::ctm_module( void_t ) 
{}

//*************************************************************************************
ctm_module::ctm_module( this_rref_t ) 
{}

//*************************************************************************************
ctm_module::~ctm_module( void_t )
{}

//*************************************************************************************
ctm_module::this_ptr_t ctm_module::create( so_memory::purpose_cref_t purp )
{
    return so_imex::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************************
void_t ctm_module::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_imex::result ctm_module::initialize( init_params_cref_t params_in )
{
    return so_imex::ok ;
}

//*************************************************************************************
bool_t ctm_module::create_properties( property_map_inout_t io_props ) const 
{
    so_imex::property_map_t::data d ;
    d.name = "OpemCTM" ;
    d.short_name = "ctm" ;
    d.desc = "Imports OpenCTM format" ;    
    d.extensions += "ctm";

    d.props_import = so_property::property_sheet_t::create(
        "[stb_module::create_properties] : ctm property sheet" ) ;

    d.props_export = so_property::property_sheet_t::create(
        "[stb_module::create_properties] : ctm property sheet" ) ;

    io_props.add_property_sheet( "ctm", std::move( d ) ) ;

    return true ;
}

//*************************************************************************************
so_imex::result ctm_module::destroy( void_t )
{
    this_t::destroy( this ) ;
    return so_imex::ok ;
}

//*************************************************************************************
bool_t ctm_module::is_format_supported( so_std::string_cref_t ext ) const 
{
    return ext == "ctm" ? true : false ;
}

//*************************************************************************************
so_imex::file_extension_names_t ctm_module::get_file_extension_names( void_t ) 
{
    return so_imex::file_extension_names_t( { "ctm" } ) ;
}
