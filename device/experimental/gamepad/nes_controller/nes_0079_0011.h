
#pragma once

#include "ines_controller.h"

#include <array>

namespace so_device
{
    namespace so_experimental
    {
        class nes_0079_0011_controller : public ines_controller
        {
            so_this_typedefs( nes_0079_0011_controller ) ;

            // [0,3] : left, right, up, down
            // [4,7] : select, start, b, a
            byte_t _values ;

        public:

            nes_0079_0011_controller( void_t ) {}
            nes_0079_0011_controller( this_cref_t rhv ) : _values( rhv._values ) {}

            virtual void_t update( std::array< byte_t, 8 > const & v )
            {
                byte_t values = 0 ;

                if( v[ 3 ] == 0 ) values |= 1 << 7 ;
                else if( v[ 3 ] == 255 ) values |= 1 << 6 ;

                if( v[ 4 ] == 0 ) values |= 1 << 5 ;
                else if( v[ 4 ] == 255 ) values |= 1 << 4 ;

                if( v[ 6 ] == 16 ) values |= 1 << 3 ;
                if( v[ 6 ] == 32 ) values |= 1 << 2 ;

                // 16 : 0001 0000 : select
                // 32 : 0010 0000 : start
                {
                    auto const btn = v[ 6 ] >> 4 ;

                    if( btn == 1 ) values |= 1 << 3 ;
                    else if( btn == 2 ) values |= 1 << 2 ;
                    else if( btn == 3 ) values |= 3 << 2 ;
                }


                // 47 : 0010 1111 : a
                // 79 : 0100 1111 : b
                {
                    auto const btn = ( v[ 5 ] >> 5 ) & 3 ;

                    if( btn == 2 ) values |= 1 << 1 ;
                    else if( btn == 1 ) values |= 1 << 0 ;
                    else if( btn == 3 ) values |= 3 << 0 ;
                }
                _values = values ;
            }

            virtual bool_t is_left( void_t ) const { return ( _values & ( 1 << 7 ) ) > 0 ; }
            virtual bool_t is_right( void_t ) const { return ( _values & ( 1 << 6 ) ) > 0 ; }
            virtual bool_t is_up( void_t ) const { return ( _values & ( 1 << 5 ) ) > 0 ; }
            virtual bool_t is_down( void_t ) const { return ( _values & ( 1 << 4 ) ) > 0 ; }

            virtual bool_t is_select( void_t ) const { return ( _values & ( 1 << 3 ) ) > 0 ; }
            virtual bool_t is_start( void_t ) const { return ( _values & ( 1 << 2 ) ) > 0 ; }
            virtual bool_t is_b( void_t ) const { return ( _values & ( 1 << 1 ) ) > 0 ; }
            virtual bool_t is_a( void_t ) const { return ( _values & ( 1 << 0 ) ) > 0 ; }
        };
        so_typedef( nes_0079_0011_controller ) ;
    }
}