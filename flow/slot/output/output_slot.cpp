//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "output_slot.h"
#include "../iinput_slot.h"

#include <snakeoil/log/global.h>

using namespace so_flow ;

//***********************************************************************
output_slot::output_slot( void_t ) 
{}

//***********************************************************************
output_slot::output_slot( this_rref_t rhv )
{
    so_log::global::error_and_exit( _inputs.size() != 0,
        "[output_slot] : can not move when inputs connected" ) ;
    
    so_move_member_ptr( _owner_ptr, rhv ) ;
}

//***********************************************************************
output_slot::~output_slot( void_t )
{
    this_t::disconnect_all() ;
}

//***********************************************************************
so_flow::result output_slot::connect( so_flow::iinput_slot_ptr_t is_ptr )
{
    {
        so_thread::lock_guard_t lk(_mtx) ;
        auto iter = std::find( _inputs.begin(), _inputs.end(), is_ptr ) ;
        if( iter != _inputs.end() )
            return so_flow::already_connected ;
    }

    if( so_core::is_nullptr(is_ptr) )
        return so_flow::invalid_argument ;

    if( so_core::is_not( this->is_compatible(is_ptr) ) )
        return so_flow::invalid_argument ;

    {
        so_thread::lock_guard_t lk( _mtx ) ;
        _inputs.push_back( is_ptr ) ;
    }

    {
        auto const res = is_ptr->connect( this ) ;
        if( so_log::global::error( so_flow::no_success(res), 
            "[so_flow::output_slot::connect] : can not connect to input slot" ) )
        {
            so_thread::lock_guard_t lk( _mtx ) ;
            auto iter = std::find( _inputs.begin(), _inputs.end(), is_ptr ) ;
            _inputs.erase(iter) ;
            return res ;
        }
    }

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result output_slot::disconnect( so_flow::iinput_slot_ptr_t is_ptr )
{
    if( so_core::is_nullptr(is_ptr) )
        return so_flow::invalid_argument ;

    {
        so_thread::lock_guard_t lk(_mtx) ;
        auto iter = std::find( _inputs.begin(), _inputs.end(), is_ptr ) ;
        if( iter == _inputs.end() )
            return so_flow::already_disconnected ;
        
        _inputs.erase( iter ) ;    
    }

    {
        auto const res = is_ptr->disconnect( this ) ;
        if( so_log::global::error( so_flow::no_success(res), 
            "[so_flow::output_slot::disconnect] : disconnection failed" ) )
            return res ;
    }

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result output_slot::disconnect( void ) 
{
    this_t::disconnect_all() ;
    return so_flow::ok ;
}

//***********************************************************************
so_flow::result output_slot::update( void_t ) 
{
    so_thread::lock_guard_t lk( _mtx ) ;

    auto ex_funk = this->exchange_funk() ;
    for(auto * sptr : _inputs)
    {
        ex_funk( sptr->get_data_ptr(), this->get_data_ptr() ) ;
    }

    return so_flow::ok ;
}

//***********************************************************************
void_t output_slot::get_all_connected_owner( so_flow::inode::nodes_ref_t owner_out ) 
{
    so_thread::lock_guard_t lk(_mtx) ;

    for( so_flow::iinput_slot_ptr_t sptr : _inputs )
    {
        owner_out.push_back( sptr->get_owner() ) ;
    }
}

//***********************************************************************
void_t output_slot::disconnect_all( void_t ) 
{
    input_slots_t inputs ;
    {
        so_thread::lock_guard_t lk(_mtx) ;
        inputs = std::move(_inputs) ;
    }

    for( auto * sptr : inputs )
    {
        sptr->disconnect() ;
    }
}

//***********************************************************************
void_t output_slot::set_owner( so_flow::inode_ptr_t new_owner ) 
{
    _owner_ptr = new_owner ;
}

//***********************************************************************
so_flow::inode_ptr_t output_slot::get_owner( void_t ) 
{
    return _owner_ptr ;
}
