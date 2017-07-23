//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "c4d_module.h"
#include "../../property/property_map.h"

#include <snakeoil/property/property_sheet.h>
#include <snakeoil/memory/guards/malloc_guard.hpp>
//#include <c4d_all.h>

#define DONT_INCLUDE_MEMORY_OVERLOADS
#include <default_alien_overloads.h>

using namespace so_imex ;

// memory allocation functions inside melange namespace 
// (if you have your own memory management you can overload these functions)
namespace melange
{
    // alloc memory no clear
    void *MemAllocNC( Int size )
    {
        void *mem = malloc( size );
        return mem;
    }

    // alloc memory set to 0
    void *MemAlloc( Int size )
    {
        void *mem = MemAllocNC( size );
        memset( mem, 0, size );
        return mem;
    }

    // realloc existing memory
    void *MemRealloc( void* orimem, Int size )
    {
        void *mem = realloc( orimem, size );
        return mem;
    }

    // free memory and set pointer to null
    void MemFree( void *&mem )
    {
        if( !mem )
            return;

        free( mem );
        mem = nullptr;
    }
}


// overload this function and fill in your own unique data
void GetWriterInfo( melange::Int32 &id, melange::String &appname )
{
    // register your own pluginid once for your exporter and enter it here under id
    // this id must be used for your own unique ids
    // 	Bool AddUniqueID(Int32 appid, const Char *const mem, Int32 bytes);
    // 	Bool FindUniqueID(Int32 appid, const Char *&mem, Int32 &bytes) const;
    // 	Bool GetUniqueIDIndex(Int32 idx, Int32 &id, const Char *&mem, Int32 &bytes) const;
    // 	Int32 GetUniqueIDCount() const;
    id = 54868781;
    appname = "snakeoil c4d module";
}

//**********************************************************************************************
c4d_module::c4d_module( void_t )
{}

//**********************************************************************************************
c4d_module::c4d_module( this_rref_t )
{}

//**********************************************************************************************
c4d_module::~c4d_module( void_t )
{}

//**********************************************************************************************
c4d_module::this_ptr_t c4d_module::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
void_t c4d_module::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
so_imex::result c4d_module::initialize( init_params_cref_t )
{
    return so_imex::ok ;
}

//**********************************************************************************************
bool_t c4d_module::create_properties( property_map_inout_t io_props ) const
{
    so_imex::property_map_t::data d ;
    d.name = "Maxon Cinema4D" ;
    d.short_name = "c4d" ;
    d.desc = "C4D File Format Service via Maxon Melange SDK" ;
    d.extensions = "c4d" ;

    d.props_import = so_property::property_sheet_t::create(
        "[c4d_module::create_properties] : property sheet" ) ;

    d.props_export = so_property::property_sheet_t::create(
        "[c4d_module::create_properties] : property sheet" ) ;

    io_props.add_property_sheet( "maxon_melange_c4d", std::move( d ) ) ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_module::is_format_supported( so_std::string_cref_t ext ) const
{
    return ext == "c4d" ;
}

//**********************************************************************************************
so_imex::file_extension_names_t c4d_module::get_file_extension_names( void_t )
{
    return file_extension_names_t( {"c4d"} ) ;
}

//**********************************************************************************************
so_imex::result c4d_module::destroy( void_t )
{
    this_t::destroy( this ) ;
    return so_imex::ok ;
}

//**********************************************************************************************
so_std::string_t c4d_module::to_string( melange::String const & mstring ) const
{
    melange::Int32 const name_len = mstring.GetCStringLen() + 1;
    so_memory::malloc_guard<char_t> mg( size_t( name_len + 0 ) );

    mstring.GetCString( mg.get(), name_len, melange::STRINGENCODING_UTF8 );

    return std::move(so_std::string( mg.get() )) ;    
}

//**********************************************************************************************
so_math::vec3f_t c4d_module::to_vector( melange::Vector const & vec_in ) const
{
    return so_math::vec3f_t( vec_in.x, vec_in.y, vec_in.z ) ;
}
