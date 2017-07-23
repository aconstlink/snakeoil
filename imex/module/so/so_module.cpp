//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "so_module.h"

using namespace so_imex ;

//**********************************************************************************************
snakeoil_module::snakeoil_module( void_t ) 
{}

//**********************************************************************************************
snakeoil_module::snakeoil_module( this_rref_t )
{}

//**********************************************************************************************
snakeoil_module::~snakeoil_module( void_t )
{}

//**********************************************************************************************
snakeoil_module::this_ptr_t snakeoil_module::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
snakeoil_module::this_ptr_t snakeoil_module::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t snakeoil_module::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
so_imex::result snakeoil_module::initialize( init_params_cref_t )
{
    return so_imex::ok ;
}

//**********************************************************************************************
so_imex::result snakeoil_module::destroy( void_t )
{
    this_t::destroy( this ) ;
    return so_imex::ok ;
}

//**********************************************************************************************
bool_t snakeoil_module::create_properties( property_map_inout_t ) const
{
    return false ;
}

//**********************************************************************************************
bool_t snakeoil_module::is_format_supported( so_std::string_cref_t ext ) const
{
    return ext == "jso" ;
}

//**********************************************************************************************
so_imex::file_extension_names_t snakeoil_module::get_file_extension_names( void_t )
{
    return file_extension_names_t( {"jso"} ) ;
}

//**********************************************************************************************
