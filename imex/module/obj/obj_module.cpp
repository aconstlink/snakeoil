//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "obj_module.h"
#include "../../property/property_map.h"

#include <snakeoil/property/property_sheet.h>

using namespace so_imex ;


//*************************************************************************************
obj_module::this_ptr_t obj_module::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
void_t obj_module::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_imex::result obj_module::initialize( init_params_cref_t )
{
    return so_imex::ok ;
}

//*************************************************************************************
bool_t obj_module::create_properties( property_map_inout_t io_props ) const
{
    so_imex::property_map_t::data d ;
    d.name = "Wavefront obj" ;
    d.short_name = "obj" ;
    d.desc = "Wavefront Obj File Format" ;
    d.extensions = "obj" ;

    d.props_import = so_property::property_sheet_t::create(
        "[obj_module::create_properties] : property sheet" ) ;

    d.props_export = so_property::property_sheet_t::create(
        "[obj_module::create_properties] : property sheet" ) ;

    io_props.add_property_sheet( "wavefront_obj", std::move( d ) ) ;

    return true ;
}

//*************************************************************************************
bool_t obj_module::is_format_supported( so_std::string_cref_t ext ) const
{
    return ext == "obj" ;
}

//*************************************************************************************
so_imex::file_extension_names_t obj_module::get_file_extension_names( void_t )
{
    return file_extension_names_t( { "obj" } ) ;
}

//*************************************************************************************
so_imex::result obj_module::destroy( void_t )
{
    this_t::destroy( this ) ;
    return so_imex::ok ;
}