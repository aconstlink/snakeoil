//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "handle.h"
#include "../system/system.h"

#include <snakeoil/core/break_if.hpp>
#include <snakeoil/core/macros/move.h>

using namespace so_io ;

//************************************************************************************
load_handle::load_handle( so_io::internal_item_ptr_t ptr, so_io::system_ptr_t ios_ptr )
{
    _data_ptr = ptr ;
    _ios = ios_ptr ;
}

//************************************************************************************
load_handle::load_handle( void_t )
{}

//************************************************************************************
load_handle::load_handle( this_rref_t rhv )
{
    so_move_member_ptr( _data_ptr, rhv ) ;
    so_move_member_ptr( _ios, rhv ) ;
}

//************************************************************************************
load_handle::~load_handle( void_t )
{
    so_core::break_if( so_core::is_not_nullptr( _data_ptr ) ) ;
}

//************************************************************************************
load_handle::this_ref_t load_handle::operator = ( this_rref_t rhv )
{
    so_move_member_ptr( _data_ptr, rhv ) ;
    so_move_member_ptr( _ios, rhv ) ;
    return *this ;
}

//************************************************************************************
so_io::result load_handle::wait_for_operation( so_io::load_completion_funk_t funk )
{
    if( so_core::is_nullptr( _ios ) ) 
        return so_io::invalid ;

    auto * ios = _ios ;
    return ios->wait_for_operation( this_t(std::move( *this )), funk ) ;
}

//************************************************************************************
store_handle::store_handle( so_io::internal_item_ptr_t ptr, so_io::system_ptr_t ios_ptr  )
{
    _data_ptr = ptr ;
    _ios = ios_ptr ;
}

//************************************************************************************
store_handle::store_handle( void_t )
{}

//************************************************************************************
store_handle::store_handle( this_rref_t rhv )
{
    so_move_member_ptr( _data_ptr, rhv ) ;
    so_move_member_ptr( _ios, rhv ) ;
}

//************************************************************************************
store_handle::~store_handle( void_t )
{
    so_core::break_if( so_core::is_not_nullptr(_data_ptr) ) ;
}

//************************************************************************************
store_handle::this_ref_t store_handle::operator = ( this_rref_t rhv )
{
    so_move_member_ptr( _data_ptr, rhv ) ;
    so_move_member_ptr( _ios, rhv ) ;
    return *this ;
}

//************************************************************************************
so_io::result store_handle::wait_for_operation( void_t )
{
    if( so_core::is_nullptr( _ios ) )
        return so_io::invalid ;

    auto * ios = _ios ;
    return ios->wait_for_operation( this_t(std::move( *this )), [=]( so_io::result ){} ) ;
}

//************************************************************************************
so_io::result store_handle::wait_for_operation( so_io::store_completion_funk_t funk )
{
    if( so_core::is_nullptr( _ios ) )
        return so_io::invalid ;

    auto * ios = _ios ;
    return ios->wait_for_operation( this_t( std::move( *this ) ), funk ) ;
}

//************************************************************************************