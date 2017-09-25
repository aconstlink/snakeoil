//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"
#include "../vector/vector2.hpp"

namespace so_math
{
    template< typename T >
    class index_1d_to_2d
    {
        so_this_typedefs( index_1d_to_2d< T > ) ;
        
    public:

        so_typedefs( T, type ) ;
        so_typedefs( so_math::vector2< T >, vec2 ) ;

    private:

        type_t _width ;
        type_t _height ;

    public:

        index_1d_to_2d( type_t const width, type_t const height ) : _width( width ), _height( height ) 
        {}

        index_1d_to_2d( this_cref_t rhv ) 
        {
            _width = rhv._width ;
            _height = rhv._height ;
        }

        index_1d_to_2d( this_rref_t rhv )
        {
            _width = rhv._width ;
            _height = rhv._height ;
        }

        ~index_1d_to_2d( void_t ) {}

    public:

        vec2_t to_index( type_cref_t i ) const
        {
            return this_t::vec2_t( i % _width, (i / _width) % _height ) ;
        }
    };
}