//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "service.h"
#include "iuser_data.h"

#include <algorithm>

using namespace so_audiox ;

//**********************************************************************************************
service::service( void_t ) 
{
}

//**********************************************************************************************
service::service::service( this_rref_t rhv )
{
    _uds = std::move( rhv._uds ) ;
}

//**********************************************************************************************
service::~service( void_t )
{
    for( auto & ud : _uds ) 
    {
        ud.ud_ptr->destroy() ;
    }
}

//**********************************************************************************************
service::this_ptr_t service::create( so_memory::purpose_cref_t p )
{
    return so_audiox::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
void_t service::destroy( this_ptr_t ptr )
{
    so_audiox::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
bool_t service::register_user_data( so_std::string_cref_t name, so_audiox::iuser_data_ptr_t ud_ptr )
{
    if( so_core::is_nullptr(ud_ptr) )
        return false ;

    so_thread::lock_guard_t lk( _mtx_ud ) ;

    auto const iter = std::find_if( _uds.begin(), _uds.end(), [&]( user_data_cref_t ud )
    {
        return ud.name == name ;
    } ) ;
    if( iter != _uds.end() )
        return false ;

    this_t::user_data_t ud ;
    ud.name = name ;
    ud.ud_ptr = ud_ptr ;

    _uds.push_back( ud ) ;

    return true ;
}

//**********************************************************************************************
bool_t service::acquire_user_data( so_std::string_cref_t name, so_audiox::iuser_data_ptr_t & ud_ptr_out )
{
    so_thread::lock_guard_t lk( _mtx_ud ) ;

    auto const iter = std::find_if( _uds.begin(), _uds.end(), [&]( user_data_cref_t ud )
    {
        return ud.name == name ;
    } ) ;
    if( iter == _uds.end() ) 
        return false ;

    ud_ptr_out = iter->ud_ptr ;

    return true ;
}

//**********************************************************************************************
void_t service::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************