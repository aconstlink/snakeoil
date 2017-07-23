//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "stb_module.h"
#include "../../property/property_map.h"

#include <snakeoil/io/system/system.h>
#include <snakeoil/property/property_sheet.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

using namespace so_imex ;

//*************************************************************************************
stb_module::stb_module( void_t ) 
{
    stbi_set_flip_vertically_on_load( 1 ) ;
}

//*************************************************************************************
stb_module::stb_module( this_rref_t rhv ) 
{
    stbi_set_flip_vertically_on_load( 1 ) ;
}

//*************************************************************************************
stb_module::~stb_module( void_t ) 
{}

//*************************************************************************************
stb_module::this_ptr_t stb_module::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
stb_module::this_ptr_t stb_module::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t stb_module::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_imex::result stb_module::initialize( init_params_cref_t params_in ) 
{    
    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result stb_module::destroy( void_t ) 
{
    this_t::destroy( this ) ;
    return so_imex::ok ;
}

//*************************************************************************************
bool_t stb_module::create_properties( property_map_inout_t io_props ) const
{
    so_imex::property_map_t::data d ;
    d.name = "stb" ;
    d.short_name = "stb" ;
    d.desc = "Imports certain image formats" ;
    
    for( auto const & f : this_t::get_file_extension_names() )
        d.extensions += f + " ";

    d.props_import = so_property::property_sheet_t::create( 
        "[stb_module::create_properties] : stb property sheet" ) ;

    d.props_export = so_property::property_sheet_t::create(
        "[stb_module::create_properties] : stb property sheet" ) ;

    io_props.add_property_sheet( "stb", std::move(d) ) ;

    return true ;
}

//*************************************************************************************
bool_t stb_module::is_format_supported( so_std::string_cref_t ext ) const
{
    if( ext == "jpg" )
    {
        return true ;
    }
    else if( ext == "png" )
    {
        return true ;
    }
    else if( ext == "gif" )
    {
        return true ;
    }

    return false ;
}

//*************************************************************************************
bool_t stb_module::is_format_supported( so_imex::image_file_format fmt ) const 
{
    switch( fmt )
    {
    case so_imex::image_file_format::jpg: 
    case so_imex::image_file_format::bmp:
    case so_imex::image_file_format::png:
    case so_imex::image_file_format::gif:
        return true ;
    default: return false ;
    }

    return false ;
}

//*************************************************************************************
so_imex::file_extension_names_t stb_module::get_file_extension_names( void_t ) const
{
    return so_imex::file_extension_names_t( { "png", "jpg", "bmp", "gif" } ) ;
}

//*************************************************************************************
bool_t stb_module::flip_vertically( byte_ptr_t dest_ptr, byte_ptr_t const src_ptr, 
    size_t const width, size_t const height, size_t const comp ) 
{
    size_t index = 0 ;

    for( size_t y = height - 1; y != size_t(-1); --y )
    {
        for( size_t x = 0; x < width; ++x )
        {
            size_t const src_index = y * width + x ;

            for( int c = 0; c < comp; ++c )
            {
                dest_ptr[index++] = src_ptr[src_index*comp + c] ;
            }
        }
    }

    return true ;
}