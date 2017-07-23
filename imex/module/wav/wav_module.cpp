//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "wav_module.h"

using namespace so_imex ;

//**********************************************************************************************
wav_module::wav_module( void_t ) 
{
}

//**********************************************************************************************
wav_module::wav_module( this_rref_t )
{
}

//**********************************************************************************************
wav_module::~wav_module( void_t )
{
}

//**********************************************************************************************
wav_module::this_ptr_t wav_module::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
void_t wav_module::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
so_imex::result wav_module::initialize( init_params_cref_t )
{
    return so_imex::ok ;
}

//**********************************************************************************************
bool_t wav_module::create_properties( property_map_inout_t ) const
{
    return false ;
}

//**********************************************************************************************
bool_t wav_module::is_format_supported( so_std::string_cref_t fmt ) const
{
    return fmt == "wav" ;
}

//**********************************************************************************************
so_imex::file_extension_names_t wav_module::get_file_extension_names( void_t ) const
{
    return so_imex::file_extension_names_t( {"wav"} ) ;
}

//**********************************************************************************************
so_imex::result wav_module::destroy( void_t )
{
    this_t::destroy( this ) ;
    return so_imex::ok ;
}

//**********************************************************************************************