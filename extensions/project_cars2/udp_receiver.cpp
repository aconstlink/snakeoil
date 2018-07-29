//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "udp_receiver.h"

#include <snakeoil/network/socket/isocket_system.h>
#include <snakeoil/network/binding/ip4_binding_point.h>

#include <snakeoil/memory/guards/malloc_guard.hpp>
#include <snakeoil/memory/global.h>

#include <snakeoil/log/global.h>

using namespace so_project_cars2 ;

//**********************************************************************************
udp_receiver::udp_receiver( so_net::isocket_system_ptr_t ptr ) : _sys(ptr)
{
    so_log::global::error_and_exit( so_core::is_nullptr( ptr ), 
        "[udp_receiver::udp_receiver] : socket system must not be nullptr"  ) ;
}

//**********************************************************************************
udp_receiver::udp_receiver( this_rref_t rhv )
{
    so_move_member_ptr( _shared, rhv ) ;
    so_move_member_ptr( _sys, rhv ) ;
}

//**********************************************************************************
udp_receiver::~udp_receiver( void_t )
{}

//**********************************************************************************
udp_receiver::this_ptr_t udp_receiver::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_memory::global::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************
void_t udp_receiver::destroy( this_ptr_t ptr )
{
    so_memory::global::dealloc( ptr ) ;
}

//**********************************************************************************
bool_t udp_receiver::start( so_project_cars2::shared_data_ptr_t shd_ptr, so_thread::sync_object_ptr_t sptr )
{
    _shared = shd_ptr ;

    so_net::recv_funk_t funk = [=]( byte_cptr_t dptr, size_t const sib, so_net::sender_info_cref_t )
    {
        bool_t inform = false ;

        // Telemetry : V2
        if( sib == 538 )
        {
            so_project_cars2::car_states cs ;

            so_project_cars2::udp_receiver_t::transform_car_state_V2( ( uint8_ptr_t ) dptr, cs )  ;

            {
                so_thread::lock_guard_t lk( _shared->mtx ) ;
                memcpy( &( _shared->cs ), &cs, sizeof( so_project_cars2::car_states ) ) ;
            }
            inform = true ;
        }
        // Telemetry : V2 - Patch 5
        else if( sib == 559 )
        {
            so_project_cars2::car_states cs ;

            so_project_cars2::udp_receiver_t::transform_car_state_V2P5( ( uint8_ptr_t ) dptr, cs )  ;

            {
                so_thread::lock_guard_t lk( _shared->mtx ) ;
                memcpy( &( _shared->cs ), &cs, sizeof( so_project_cars2::car_states ) ) ;
            }

            inform = true ;
        }
        // Weather: V2 - Patch 5
        else if( sib == 24 )
        {
            so_project_cars2::weather_states ws ;
            so_project_cars2::udp_receiver_t::transform_weather_state_V2P5( ( uint8_ptr_t ) dptr, ws ) ;
            {
                so_thread::lock_guard_t lk( _shared->mtx ) ;
                memcpy( &( _shared->ws ), &ws, sizeof( so_project_cars2::weather_states ) ) ;
            }
            inform = true ;
        }

        if( inform )
        {
            {
                so_thread::lock_guard_t lk( _shared->mtx_frame ) ;
                _shared->frame++ ;
            }
            _shared->cv_frame.notify_all() ;
        }

        return size_t( 0 ) ;
    } ;

    so_net::ip4_binding_point_t bp ;
    bp.port = 5606 ;

    auto const res = _sys->recv( bp, funk, sptr ) ;
    so_log::global::error( so_net::no_success( res ), 
        "[so_project_cars2::udp_receiver::start] : failed to start recv" ) ;

    return so_core::is_not( so_net::no_success( res ) ) ;
}

//**********************************************************************************
void_t udp_receiver::transform_car_state_V2( uint8_ptr_t buffer, so_project_cars2::car_states_out_t cs ) 
{
    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 17 ) ;
        cs.car_flags = v ;
    }

    {
        short_t const v = this_t::buffer_to_short( buffer, 18 ) ;
        cs.oil_temperature = v ;
    }

    {
        ushort_t const v = this_t::buffer_to_ushort( buffer, 20 ) ;
        cs.oil_pressure = v ;
    }

    {
        short_t const v = this_t::buffer_to_short( buffer, 22 ) ;
        cs.water_temperature = v ;
    }

    {
        short_t const v = this_t::buffer_to_short( buffer, 24 ) ;
        cs.water_pressure = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 28 ) ;
        cs.fuel_capacity = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 29 ) ;
        cs.brake = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 30 ) ;
        cs.throttle = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 31 ) ;
        cs.clutch = v ;
    }

    {
        float_t const v = this_t::buffer_to_float( buffer, 32 ) ;
        cs.fuel_level = v ;
    }
    {
        // m/s -> km/h
        float_t const speed_factor = 3.6f ;
        float_t const speed = this_t::buffer_to_float( buffer, 36 ) * speed_factor ;
        cs.speed = speed ;
    }
    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 44 ) ;
        cs.steering = v ;
    }

    {
        ushort_t const v = this_t::buffer_to_ushort( buffer, 40 ) ;
        cs.rpm = v ;
    }

    {
        ushort_t const v = this_t::buffer_to_ushort( buffer, 42 ) ;
        cs.max_rpm = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 45 ) ;
        cs.gear = v & 0x0f ;
        cs.num_gears = ( v >> 4 ) & 0x0f ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 46 ) ;
        cs.boost_amount = v ;
    }

    {
        float_t const v = this_t::buffer_to_float( buffer, 48 ) ;
        cs.odometer = v ;
    }
}

//**********************************************************************************
void_t udp_receiver::transform_weather_stateV2( uint8_ptr_t, so_project_cars2::weather_states_out_t ) 
{
}
//**********************************************************************************
void_t udp_receiver::transform_car_state_V2P5( uint8_ptr_t buffer, so_project_cars2::car_states_out_t cs )
{
    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 17 ) ;
        cs.car_flags = v ;
    }

    {
        short_t const v = this_t::buffer_to_short( buffer, 18 ) ;
        cs.oil_temperature = v ;
    }

    {
        ushort_t const v = this_t::buffer_to_ushort( buffer, 20 ) ;
        cs.oil_pressure = v ;
    }

    {
        short_t const v = this_t::buffer_to_short( buffer, 22 ) ;
        cs.water_temperature = v ;
    }

    {
        short_t const v = this_t::buffer_to_short( buffer, 24 ) ;
        cs.water_pressure = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 28 ) ;
        cs.fuel_capacity = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 29 ) ;
        cs.brake = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 30 ) ;
        cs.throttle = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 31 ) ;
        cs.clutch = v ;
    }
    
    {
        float_t const v = this_t::buffer_to_float( buffer, 32 ) ;
        cs.fuel_level = v ;
    }
    {
        // m/s -> km/h
        float_t const speed_factor = 3.6f ;
        float_t const speed = this_t::buffer_to_float( buffer, 36 ) * speed_factor ;
        cs.speed = speed ;
    }
    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 44 ) ;
        cs.steering = v ;
    }

    {
        ushort_t const v = this_t::buffer_to_ushort( buffer, 40 ) ;
        cs.rpm = v ;
    }

    {
        ushort_t const v = this_t::buffer_to_ushort( buffer, 42 ) ;
        cs.max_rpm = v ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 45 ) ;
        cs.gear = v & 0x0f ;
        cs.num_gears = ( v >> 4 ) & 0x0f ;
    }

    {
        uint8_t const v = this_t::buffer_to_uint8( buffer, 46 ) ;
        cs.boost_amount = v ;
    }

    {
        float_t const v = this_t::buffer_to_float( buffer, 48 ) ;
        cs.odometer = v ;
    }
}

//**********************************************************************************
void_t udp_receiver::transform_weather_state_V2P5( uint8_ptr_t buffer, so_project_cars2::weather_states_out_t ws )
{
    {
        int8_t const v = this_t::buffer_to_int8( buffer, 458 ) ;
        ws.ambient_temp = v ;
    }
}

//**********************************************************************************
float_t udp_receiver::buffer_to_float( uint8_ptr_t buffer, size_t const pos )
{
    return *( float_ptr_t( &( buffer[ pos ] ) ) ) ;
}

//**********************************************************************************
uint8_t udp_receiver::buffer_to_uint8( uint8_ptr_t buffer, size_t const pos )
{
    return *( uint8_ptr_t( &( buffer[ pos ] ) ) ) ;
}

//**********************************************************************************
int8_t udp_receiver::buffer_to_int8( uint8_ptr_t buffer, size_t const pos )
{
    return *( int8_ptr_t( &( buffer[ pos ] ) ) ) ;
}

//**********************************************************************************
short_t udp_receiver::buffer_to_short( uint8_ptr_t buffer, size_t const pos )
{
    return *( short_ptr_t( &( buffer[ pos ] ) ) ) ;
}

//**********************************************************************************
ushort_t udp_receiver::buffer_to_ushort( uint8_ptr_t buffer, size_t const pos )
{
    return *( ushort_ptr_t( &( buffer[ pos ] ) ) ) ;
}

//**********************************************************************************
void_t udp_receiver::print_out( so_project_cars2::car_states const & cs )
{
    system( "cls" ) ;

    static size_t count = 0 ;

    so_log::global::status( "speed km/h : " + std::to_string( cs.speed_kmh() ) ) ;
    so_log::global::status( "speed mi/h : " + std::to_string( cs.speed_mph() ) ) ;
    so_log::global::status( "rpm : " + std::to_string( cs.rpm ) ) ;
    so_log::global::status( "max_rpm : " + std::to_string( cs.max_rpm ) ) ;

    so_log::global::status( "oil_temperature: " + std::to_string( cs.oil_temperature ) ) ;
    so_log::global::status( "oil_pressure : " + std::to_string( cs.oil_pressure ) ) ;
    so_log::global::status( "water_temperature : " + std::to_string( cs.water_temperature ) ) ;
    so_log::global::status( "water_pressure : " + std::to_string( cs.water_pressure ) ) ;

    so_log::global::status( "car_flags : " + std::to_string( cs.car_flags ) ) ;

    so_log::global::status( "fuel_capacity : " + std::to_string( cs.fuel_capacity ) ) ;
    so_log::global::status( "brake : " + std::to_string( cs.brake ) ) ;
    so_log::global::status( "throttle : " + std::to_string( cs.throttle ) ) ;
    so_log::global::status( "clutch : " + std::to_string( cs.clutch ) ) ;

    so_log::global::status( "steering : " + std::to_string( cs.steering ) ) ;
    so_log::global::status( "fuel_level : " + std::to_string( cs.fuel_level ) ) ;
    so_log::global::status( "gear : " + std::to_string( cs.gear ) ) ;
    so_log::global::status( "num_gears : " + std::to_string( cs.num_gears ) ) ;

    so_log::global::status( "count : " + std::to_string( ++count ) ) ;

}