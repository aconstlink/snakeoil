

#pragma once

#include "../../../api.h"
#include "../../../typedefs.h"

namespace so_device
{
    namespace so_experimental
    {
        class ines_controller
        {
        public:

            virtual void_t update( std::array< byte_t, 8 > const & ) = 0 ;
            virtual bool_t is_left( void_t ) const = 0 ;
            virtual bool_t is_right( void_t ) const = 0 ;
            virtual bool_t is_up( void_t ) const = 0 ;
            virtual bool_t is_down( void_t ) const = 0 ;

            virtual bool_t is_select( void_t ) const = 0 ;
            virtual bool_t is_start( void_t ) const = 0 ;
            virtual bool_t is_b( void_t ) const = 0 ;
            virtual bool_t is_a( void_t ) const = 0 ;
        };
        so_typedef( ines_controller ) ;
    }    
}