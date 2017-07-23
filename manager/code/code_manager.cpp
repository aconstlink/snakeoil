//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "code_manager.h"

#include <snakeoil/log/log.h>

using namespace so_manager ;

//*****************************************************************************************************
code_manager::this_ptr_t code_manager::create( so_memory::purpose_cref_t p ) 
{
    return this_t::create( this_t(), p ) ;
}

//*****************************************************************************************************
code_manager::this_ptr_t code_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_manager::memory::alloc( std::move(rhv), p ) ;
}

//*****************************************************************************************************
void_t code_manager::destroy( this_ptr_t ptr ) 
{
    so_manager::memory::dealloc( ptr ) ;
}

//*****************************************************************************************************
code_manager::code_manager( void_t )
{

}

//*****************************************************************************************************
code_manager::code_manager( this_cref_t ) 
{
    // private - no nothing.
}

//*****************************************************************************************************
code_manager::code_manager( this_rref_t rhv ) 
{
    _key_to_code = std::move( rhv._key_to_code ) ;
}

//*****************************************************************************************************
code_manager::~code_manager( void_t ) 
{

}

//*****************************************************************************************************
bool_t code_manager::has_code( so_manager::key_cref_t key_in ) 
{
    so_thread::lock_t lk(_mtx) ;

    return _key_to_code.find( key_in ) != _key_to_code.end() ;
}

//*****************************************************************************************************
so_manager::result code_manager::add_code( so_manager::key_cref_t key_in,  so_manager::string_cref_t code_in ) 
{
    so_thread::lock_t lk(_mtx) ;

    auto iter = _key_to_code.find( key_in ) ;
    if( so_log::log::error( iter != _key_to_code.end(), 
        "[code_manager::add_code] : the key <"+std::string(key_in)+"> already exists." ) )
        return so_manager::invalid_argument ;
    
    code_data data ;
    data.code = code_in ;

    _key_to_code[key_in] = std::move(data) ;

    return so_manager::ok ;
}

//*****************************************************************************************************
so_manager::result code_manager::replace_code( so_manager::key_cref_t key_in, so_manager::string_cref_t code_in ) 
{
    so_thread::lock_t lk(_mtx) ;

    auto iter = _key_to_code.find( key_in ) ;
    if( iter == _key_to_code.end() )
    {
        return this_t::add_code(key_in, code_in) ;
    }

    iter->second.code = code_in ;

    return so_manager::ok ;
}

//*****************************************************************************************************
so_manager::result code_manager::compose( so_manager::key_list_cref_t keys, so_manager::string_ref_t code_out ) 
{
    so_thread::lock_t lk(_mtx) ;

    for( auto key_in : keys )
    {
        auto iter = _key_to_code.find( key_in ) ;
        if( so_log::log::error( iter == _key_to_code.end(), 
            "[code_manager::compose] : the key <"+key_in+"> does not exist.") ) 
            return so_manager::invalid_argument ;

        code_out += iter->second.code ;
    }   

    return so_manager::ok ;
}

//*****************************************************************************************************
void_t code_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
