//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROJECT_CARS_UDP_RECEIVER_H_
#define _SNAKEOIL_PROJECT_CARS_UDP_RECEIVER_H_

#include "api.h"
#include "typedefs.h"

#include "structs.h"

#include <snakeoil/network/protos.h>
#include <snakeoil/thread/primitive/sync_object.hpp>
#include <snakeoil/memory/typedefs.h>
#include <snakeoil/core/types.hpp>
#include <snakeoil/core/macros/typedef.h>

namespace so_project_cars
{
    class SNAKEOIL_PROJECT_CARS_API udp_receiver
    {
        so_this_typedefs( udp_receiver ) ;

    private:

        so_project_cars::shared_data_ptr_t _shared = nullptr ;
        so_net::isocket_system_ptr_t _sys = nullptr ;
        
    public:

        udp_receiver( so_net::isocket_system_ptr_t ) ;
        udp_receiver( this_cref_t ) = delete ;
        udp_receiver( this_rref_t ) ;
        ~udp_receiver( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        bool_t start( so_project_cars::shared_data_ptr_t, so_thread::sync_object_ptr_t ) ;

    private:

        static void_t transform_car_state( uint8_ptr_t, so_project_cars::car_states_out_t ) ;
        static void_t transform_weather_state( uint8_ptr_t, so_project_cars::weather_states_out_t ) ;

        static float_t buffer_to_float( uint8_ptr_t, size_t const ) ;
        static uint8_t buffer_to_uint8( uint8_ptr_t, size_t const ) ;
        static int8_t buffer_to_int8( uint8_ptr_t, size_t const ) ;
        static short_t buffer_to_short( uint8_ptr_t, size_t const ) ;
        static ushort_t buffer_to_ushort( uint8_ptr_t, size_t const ) ;

    private:

        static void_t print_out( so_project_cars::car_states const & ) ;

    };
    so_typedef( udp_receiver ) ;
}

#endif