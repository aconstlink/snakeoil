//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "node_output_policy.h"

#include "../../slot/ioutput_slot.h"

#include <snakeoil/log/log.h>

using namespace so_flow ;
using namespace so_flow::so_policy ;

//***********************************************************************
node_output_policy::node_output_policy( void_t )
{}

//***********************************************************************
node_output_policy::node_output_policy( this_rref_t rhv ) 
{
    _outputs = std::move( rhv._outputs ) ;    
}

//***********************************************************************
node_output_policy::~node_output_policy( void_t ) 
{
    for(auto & item : _outputs)
    {
        so_log::log::warning( item.second.touched != 0,
            "[so_flow::node_output_policy::~node_output_policy] : slot is touched but will be deleted" ) ;

        item.second.sptr->destroy() ;
    }
}

//***********************************************************************
so_flow::result node_output_policy::add_slot( so_flow::key_cref_t key, so_flow::ioutput_slot_ptr_t os_ptr )
{
    so_thread::lock_guard_t lk( _mtx ) ;

    if(so_core::is_nullptr( os_ptr ))
        return so_flow::invalid_argument ;

    auto iter = _outputs.find( key ) ;
    if(iter != _outputs.end())
        return so_flow::name_taken ;

    store_data_t sd ;
    sd.sptr = os_ptr ;
    sd.touched = 0 ;

    _outputs[key] = sd ;

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node_output_policy::add_slot_and_touch( so_flow::key_in_t key_in, 
    so_flow::ioutput_slot_ptr_t sptr, this_t::handle_out_t hnd_out ) 
{
    so_thread::lock_guard_t lk( _mtx ) ;

    if(so_core::is_nullptr( sptr ))
        return so_flow::invalid_argument ;

    auto iter = _outputs.find( key_in ) ;
    if(iter != _outputs.end())
        return so_flow::name_taken ;

    store_data_t sd ;
    sd.sptr = sptr ;
    sd.touched = 1 ;

    _outputs[key_in] = sd ;

    hnd_out = this_t::handle_t( key_in, sptr, this ) ;

    return so_flow::ok ;
}

//***********************************************************************
so_flow::result node_output_policy::destroy_output_slot( so_flow::key_cref_t key )
{
    this_t::slot_ptr_t sptr = nullptr ;

    {
        so_thread::lock_guard_t lk( _mtx ) ;
        auto iter = _outputs.find( key )  ;
        if(iter == _outputs.end())
            return so_flow::invalid_argument ;

        if(so_log::log::warning( iter->second.touched != 0,
            "[so_flow::node_input_policy::destroy_input_slot] : touched slot will not be destroyed : " +
            key )) return so_flow::slot_is_touched ;

        sptr = iter->second.sptr ;

        _outputs.erase( iter ) ;
    }

    sptr->destroy() ;

    return so_flow::ok ;
}

//***********************************************************************
bool_t node_output_policy::find_and_touch( so_flow::key_cref_t key_in, this_t::handle_ref_t hnd_out ) 
{
    so_thread::lock_guard_t lk( _mtx ) ;

    auto iter = _outputs.find( key_in )  ;
    if(iter == _outputs.end())
        return false ;

    iter->second.touched++ ;
    hnd_out = this_t::handle_t( key_in, iter->second.sptr, this ) ;

    return true ;
}

//***********************************************************************
void_t node_output_policy::for_each( for_each_funk_t funk ) 
{
    for( auto & item : _outputs )
    {
        item.second.touched++ ;
        funk( item.first, item.second ) ;
        item.second.touched-- ;
    }
}

//***********************************************************************
bool_t node_output_policy::untouch( so_flow::key_in_t key_in ) 
{
    so_thread::lock_guard_t lk( _mtx ) ;

    auto iter = _outputs.find( key_in )  ;
    if(iter == _outputs.end())
        return false ;

    so_log::log::error_and_exit( iter->second.touched == 0,
        "[so_flow::node_input_policy::untouch] : handle can not be untouched. programming error" ) ;

    iter->second.touched-- ;

    return true ;
}
