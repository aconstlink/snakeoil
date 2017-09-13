//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "midi_device.h"

#include "../../component/iinput_component.h"
#include "../../component/ioutput_component.h"
//#include "../../api/imidi_api.h"

#include "../notification/iuser_notify.h"

#include <snakeoil/log/log.h>

#include <algorithm>

using namespace so_device ;

//****************************************************************************************
midi_device::midi_device( device_info_cref_t info ) : _di(info)
{

}

//****************************************************************************************
midi_device::midi_device( this_rref_t rhv )
{
    this_t::clear() ;

    _di = std::move( rhv._di ) ;
    _in_datas = std::move( rhv._in_datas ) ;
    _changed_comps = std::move( rhv._changed_comps ) ;

    _out_datas = std::move( rhv._out_datas ) ;
}

//****************************************************************************************
midi_device::~midi_device( void_t )
{
    for( auto & item : _in_datas )
    {
        item.cptr->destroy() ;
    }

    for( auto & item : _out_datas )
    {
        item.cptr->destroy() ;
    }

    for( auto * nptr : _notifys )
    {
        nptr->destroy() ;
    }
}

//****************************************************************************************
midi_device::this_ref_t midi_device::operator = ( this_rref_t rhv )
{
    this_t::clear() ;

    _di = std::move( rhv._di ) ;
    _in_datas = std::move( rhv._in_datas ) ;
    _changed_comps = std::move( rhv._changed_comps ) ;

    _out_datas = std::move( rhv._out_datas ) ;

    return *this ;
}

//****************************************************************************************
midi_device::this_ptr_t midi_device::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_device::memory::alloc( std::move(rhv), p ) ;
}

//****************************************************************************************
void_t midi_device::destroy( this_ptr_t ptr ) 
{
    so_device::memory::dealloc( ptr ) ;
}

//****************************************************************************************
midi_device::device_info_cref_t midi_device::get_device_info( void_t ) const 
{
    return _di ;
}

//****************************************************************************************
void_t midi_device::clear( void_t )
{
    _di = device_info_t() ;

    for( auto & item : _in_datas )
    {
        item.cptr->destroy() ;
    }

    for( auto & item : _out_datas )
    {
        item.cptr->destroy() ;
    }

    for( auto * nptr : _notifys )
    {
        nptr->destroy() ;
    }
}

//****************************************************************************************
so_device::result midi_device::receive_message( midi_message_cref_t msg ) 
{
    so_thread::lock_guard_t lk(_mtx_in) ;

    _in_msgs.push_back( msg ) ;


    return so_device::ok ;
}

//****************************************************************************************
so_device::result midi_device::transmit_message( midi_message_cref_t msg ) 
{
    so_thread::lock_guard_t lk(_mtx_out) ;
    _out_msgs.push_back( msg ) ;

    return so_device::ok ;
}

//****************************************************************************************
so_device::result midi_device::trigger_component( so_device::key_cref_t key, 
    trigger_funk_t funk ) 
{
    so_thread::lock_guard_t lk(_trigger_data_mtx) ;
    
    trigger_data_t td ;
    td.key = key ;
    td.funk = funk ;

    _trigger_datas.push_back( td ) ;

    return so_device::ok ;
}

//****************************************************************************************
void_t midi_device::update( midi_messages_ref_t msgs_out ) 
{
    //
    // . process all midi messages
    //
    {
        midi_messages_t midi_msgs ;
        {
            so_thread::lock_guard_t lk( _mtx_in ) ;
            midi_msgs = std::move( _in_msgs ) ;
        }

        for(auto const & msg : midi_msgs)
        {
            for(auto & item : _in_datas)
            {
                if(item.logic.midi_message_funk( item.cptr, msg ))
                {
                    auto iter = std::find_if( _changed_comps.begin(), _changed_comps.end(),
                        [&]( store_in_data_cref_t inner_item )
                    {
                        return item.cptr == inner_item.cptr ;
                    } )  ;

                    if(iter == _changed_comps.end())
                    {
                        _changed_comps.push_back( item ) ;
                    }

                    break ;
                }
            }
        }
    }

    //
    // . do user update
    // the user needs to check the current state before the follow-up state!
    //
    {
        for( auto & item : _changed_comps )
        {
            for( auto * nptr : _notifys )
            {
                nptr->on_change( this, item.name, item.cptr ) ;
            }
        }
    }

    //
    // . do all user triggered components
    // 
    {
        trigger_datas_t tds ;
        {
            so_thread::lock_guard_t lk(_trigger_data_mtx) ;
            tds = std::move(_trigger_datas) ;
        }

        for( auto & item : tds )
        {
            auto iter = std::find_if( _out_datas.begin(), _out_datas.end(), 
                [&]( store_out_data_cref_t item_inner )
            {
                return item_inner.name == item.key ;
            } ) ;

            if( iter == _out_datas.end() )
            {
                so_log::log::error( "[so_device::midi_device::update] : \
                                     Item does not exist: " + item.key ) ;
                continue ;
            }

            // . call user funk
            {
                auto const res = item.funk( iter->cptr ) ;
                if( so_core::is_not(res) )
                {
                    so_log::log::error( so_core::is_not( res ),
                        "[so_device::midi_device::update] : component found but trigger funk failed : " +
                        item.key ) ;
                    continue ;
                }                
            }

            {
                auto iter_inner = std::find_if( _triggered_component.begin(), _triggered_component.end(),
                    [&]( store_out_data_cref_t item_inner )
                {
                    return item_inner.name == item.key ;
                } ) ;

                if( iter_inner == _triggered_component.end() )
                    _triggered_component.push_back( *iter ) ;
            }
        }
    }

    // . handle output message which were triggered
    {
        auto triggered = std::move( _triggered_component ) ;
        for( auto & item : triggered )
        {
            so_device::midi_message_t msg ;
            auto const res = item.logic.handle_funk( item.cptr, msg ) ;

            if( res == so_device::midi_output_result::transmit_and_queue ||
                res == so_device::midi_output_result::transmit_and_dismiss )
            {
                this_t::transmit_message( msg ) ;
            }

            if( res == so_device::midi_output_result::transmit_and_queue ||
                res == so_device::midi_output_result::queue )
            {
                auto iter_inner = std::find_if( _triggered_component.begin(), _triggered_component.end(),
                    [&]( store_out_data_cref_t item_inner )
                {
                    return item_inner.name == item.name ;
                } ) ;

                if( iter_inner == _triggered_component.end() )
                    _triggered_component.push_back( item ) ;
            }
        }
    }

    //
    // . transmit all outgoing messages
    //
    {
        so_thread::lock_guard_t lk( _mtx_out ) ;
        msgs_out = std::move( _out_msgs ) ;
    }

    //
    // . do follow up state of incoming messages
    // -> for now, lets do the follow up for input messages 
    // at the end of the end
    //
    {
        auto tmp = std::move( _changed_comps ) ;

        for(size_t i = 0; i < tmp.size(); ++i)
        {
            auto & item = tmp.at( i ) ;
            if( item.logic.follow_up_funk( item.cptr ) )
            {
                _changed_comps.push_back( item ) ;
            }
        }
    }
}

//****************************************************************************************
so_device::result midi_device::add_component( so_device::key_cref_t name, input_component_logic_cref_t logic,
    so_device::so_input::iinput_component_ptr_t cptr ) 
{
    so_thread::lock_guard_t lk(_mtx_in) ;

    auto iter = std::find_if( _in_datas.begin(), _in_datas.end(), [&]( this_t::store_in_data_cref_t item )
    {
        return item.name == name || item.cptr == cptr ;
    } ) ;
    
    if( iter != _in_datas.end() )
    {
        so_log::log::warning( "[so_device::midi_device::add_component] : component already added" ) ;
        return so_device::invalid_argument ;
    }

    this_t::store_in_data_t sd ;
    sd.name = name ;
    sd.cptr = cptr ;
    sd.logic = logic ;

    _in_datas.push_back( sd ) ;

    return so_device::ok ;
}

//****************************************************************************************
so_device::result midi_device::add_component( so_device::key_cref_t name, 
    output_component_logic_cref_t logic, so_device::so_output::ioutput_component_ptr_t cptr ) 
{
    so_thread::lock_guard_t lk(_mtx_out) ;

    auto iter = std::find_if( _out_datas.begin(), _out_datas.end(), 
        [&]( this_t::store_out_data_cref_t item )
    {
        return item.name == name || item.cptr == cptr ;
    } ) ;

    if( iter != _out_datas.end() )
    {
        so_log::log::warning( "[so_device::midi_device::add_component] : component already added" ) ;
        return so_device::invalid_argument ;
    }

    this_t::store_out_data_t sd ;
    sd.name = name ;
    sd.cptr = cptr ;
    sd.logic = logic ;

    _out_datas.push_back( sd ) ;

    return so_device::ok ;
}

//****************************************************************************************
so_device::result midi_device::register_notification( iuser_notify_ptr_t nptr ) 
{
    if( so_core::is_nullptr(nptr) )
        return so_device::invalid_argument ;

    auto iter = std::find( _notifys.begin(), _notifys.end(), nptr ) ;
    if( iter != _notifys.end() )
        return so_device::ok ;

    _notifys.push_back( nptr ) ;

    return so_device::ok ;
}

//****************************************************************************************
void_t midi_device::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
