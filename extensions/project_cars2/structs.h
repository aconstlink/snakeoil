//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/thread/mutex.h>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/core/types.hpp>

#include <cstring>

namespace so_project_cars2
{
    using namespace so_core::so_types ;

    enum class car_flag
    {
        CAR_HEADLIGHT = 1,
        CAR_ENGINE_ACTIVE = 2,
        CAR_ENGINE_WARNING = 4,
        CAR_SPEED_LIMITER = 8,
        CAR_ABS = 16,
        CAR_HANDBRAKE = 32,
        CAR_STABILITY = 64,
        CAR_TRACTION_CONTROL = 128
    };

    struct car_states
    {
        so_this_typedefs( car_states ) ;

        short_t oil_temperature ;
        short_t oil_pressure ;
        short_t water_temperature ;
        ushort_t water_pressure ;
        ushort_t fuel_pressure ;
        uint8_t car_flags ;
        uint8_t fuel_capacity ;
        uint8_t brake ;
        uint8_t throttle ;
        uint8_t clutch ;
        uint8_t steering ;
        float_t fuel_level ;
        float_t speed ;
        ushort_t rpm ;
        ushort_t max_rpm ;
        uint8_t gear ;
        uint8_t num_gears ;
        uint8_t boost_amount ;
        float_t odometer ;

        so_math::vec3f_t orientation ;
        so_math::vec3f_t local_velocity ;
        so_math::vec3f_t world_velocity ;
        so_math::vec3f_t angular_velocity ;
        so_math::vec3f_t local_acceleration ;
        so_math::vec3f_t world_acceleration ;
        so_math::vec3f_t extends_center ;

        car_states( void_t ){}
        car_states( this_rref_t rhv ){ memcpy( this, &rhv, sizeof( this_t ) ) ; }
        car_states( this_cref_t rhv ){ memcpy( this, &rhv, sizeof( this_t ) ) ; }
        ~car_states( void_t ){}

        bool_t is_headlight( void_t ) const
        {
            return ( car_flags & uint8_t( car_flag::CAR_HEADLIGHT ) ) != 0 ;
        }

        bool_t is_engine_active( void_t ) const
        {
            return ( car_flags & uint8_t( car_flag::CAR_ENGINE_ACTIVE ) ) != 0 ;
        }

        bool_t is_engine_warning( void_t ) const
        {
            return ( car_flags & uint8_t( car_flag::CAR_ENGINE_WARNING ) ) != 0 ;
        }

        bool_t is_engine_speed_limiter( void_t ) const
        {
            return ( car_flags & uint8_t( car_flag::CAR_SPEED_LIMITER ) ) != 0 ;
        }

        bool_t is_abs( void_t ) const
        {
            return ( car_flags & uint8_t( car_flag::CAR_ABS ) ) != 0 ;
        }

        bool_t is_handbreak( void_t ) const
        {
            return ( car_flags & uint8_t( car_flag::CAR_HANDBRAKE ) ) != 0 ;
        }

        bool_t is_stability( void_t ) const
        {
            return ( car_flags & uint8_t( car_flag::CAR_STABILITY ) ) != 0 ;
        }

        bool_t is_traction_control( void_t ) const
        {
            return ( car_flags & uint8_t( car_flag::CAR_TRACTION_CONTROL ) ) != 0 ;
        }

        float_t speed_kmh( void_t ) const
        {
            return speed ;
        }

        float_t speed_mph( void_t ) const
        {
            return speed / 1.60934f ;
        }
    };
    so_typedef( car_states ) ;

    struct weather_states
    {
        so_this_typedefs( weather_states ) ;

        int8_t ambient_temp ;

        weather_states( void_t ){}
        weather_states( this_rref_t rhv ){ memcpy( this, &rhv, sizeof( this_t ) ) ; }
        weather_states( this_cref_t rhv ){ memcpy( this, &rhv, sizeof( this_t ) ) ; }
        ~weather_states( void_t ){}

    };
    so_typedef( weather_states ) ;

    struct shared_data
    {
        so_this_typedefs( shared_data ) ;

        so_thread::mutex_t mtx ;
        car_states cs ;
        weather_states ws ;

        so_thread::mutex_t mtx_frame ;
        so_thread::condition_variable_t cv_frame ;
        size_t frame = 0 ;

        shared_data( void_t ) {}
        shared_data( this_rref_t rhv ) :
            cs( rhv.cs ), ws( rhv.ws ), frame(rhv.frame)
        {}

        ~shared_data( void_t ) {}
    };
    so_typedef( shared_data ) ;
}
