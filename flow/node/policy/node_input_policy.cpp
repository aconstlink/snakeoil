//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "node_input_policy.h"


#include "../../slot/iinput_slot.h"

#include <snakeoil/log/log.h>

using namespace so_flow ;
using namespace so_flow::so_policy ;

//***********************************************************************
node_input_policy::node_input_policy( void_t ) 
{}

//***********************************************************************
node_input_policy::node_input_policy( this_rref_t rhv )
{
    _inputs = std::move( rhv._inputs ) ;
}

//***********************************************************************
node_input_policy::~node_input_policy( void_t )
{
    for( auto & item : _inputs )
    {
        so_log::log::warning( item.second.touched != 0, 
            "[so_flow::node_input_policy::~node_input_policy] : slot is touched but will be deleted" ) ;
        item.second.is_ptr->destroy() ;
    }
}

//***********************************************************************    
so_flow::result node_input_policy::add_slot( so_flow::key_cref_t key, so_flow::iinput_slot_ptr_t is_ptr )
{
    so_thread::lock_guard_t lk(_mtx) ;

    if( so_core::is_nullptr( is_ptr ) )
        return so_flow::invalid_argument ;

    auto iter = _inputs.find( key ) ;
    if( iter != _inputs.end() )
        return so_flow::name_taken ;

    this_t::store_data_t sd ;
    sd.is_ptr = is_ptr ;
    sd.touched = 0 ;

    _inputs[key] = sd ;

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node_input_policy::add_slot_and_touch( so_flow::key_in_t key_in, 
    so_flow::iinput_slot_ptr_t is_ptr, this_t::handle_out_t hnd_out ) 
{
    so_thread::lock_guard_t lk( _mtx ) ;

    if(so_core::is_nullptr( is_ptr ))
        return so_flow::invalid_argument ;

    auto iter = _inputs.find( key_in ) ;
    if( iter != _inputs.end() )
        return so_flow::name_taken ;

    this_t::store_data_t sd ;
    sd.is_ptr = is_ptr ;
    sd.touched = 1 ;

    _inputs[key_in] = sd ;

    hnd_out = this_t::handle_t( key_in, is_ptr, this ) ;

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node_input_policy::destroy_input_slot( so_flow::key_cref_t key )
{
    so_flow::iinput_slot_ptr_t is_ptr = nullptr ;

    {
        so_thread::lock_guard_t lk( _mtx ) ;
        auto iter = _inputs.find( key )  ;
        if(iter == _inputs.end())
            return so_flow::invalid_argument ;

        if( so_log::log::warning( iter->second.touched != 0, 
            "[so_flow::node_input_policy::destroy_input_slot] : touched slot will not be destroyed : " +
            key ) ) return so_flow::slot_is_touched ;

        is_ptr = iter->second.is_ptr ;

        _inputs.erase( iter ) ;
    }

    is_ptr->destroy() ;

    return so_flow::ok ;
}

//***********************************************************************
bool_t node_input_policy::find_and_touch( so_flow::key_cref_t key_in, this_t::handle_ref_t hnd_out ) 
{
    so_thread::lock_guard_t lk( _mtx ) ;

    auto iter = _inputs.find( key_in )  ;
    if( iter == _inputs.end() )
        return false ;

    iter->second.touched++ ;
    hnd_out = this_t::handle_t( key_in, iter->second.is_ptr, this ) ;

    return true ;
}

//***********************************************************************
bool_t node_input_policy::untouch( so_flow::key_in_t key_in ) 
{
    so_thread::lock_guard_t lk( _mtx ) ;

    auto iter = _inputs.find( key_in )  ;
    if( iter == _inputs.end() )
        return false ;

    so_log::log::error_and_exit( iter->second.touched == 0, 
        "[so_flow::node_input_policy::untouch] : handle can not be untouched. programming error" ) ;

    iter->second.touched-- ;

    return true ;

}

//***********************************************************************
void_t node_input_policy::for_each( for_each_funk_t funk ) 
{
    so_thread::lock_guard_t lk( _mtx ) ;

    for( auto & item : _inputs )
    {
        funk( item.second.is_ptr ) ;
    }
}

//***********************************************************************
size_t node_input_policy::get_num_slots( void_t ) const
{
    return _inputs.size() ;
}
