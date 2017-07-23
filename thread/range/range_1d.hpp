//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_RANGE_RANGE_1D_HPP_
#define _SNAKEOIL_THREAD_RANGE_RANGE_1D_HPP_

#include "../typedefs.h"

namespace so_thread
{
    template< typename T >
    class range_1d
    {
        so_this_typedefs( range_1d< T > ) ;
        so_typedefs( T, type ) ;


    private:

        type_t _begin ;
        type_t _end ;

    public:

        range_1d( type_t b, type_t e ) : _begin(b), _end(e){}
        range_1d( this_cref_t rhv )
        {
            _begin = rhv._begin ;
            _end = rhv._end ;
        }

        range_1d( this_rref_t rhv )
        {
            _begin = rhv._begin ;
            _end = rhv._end ;
        }

        ~range_1d( void_t ){}

    public:

        type_t begin( void_t ) const { return _begin ; }
        type_t end( void_t ) const { return _end ; }

        type_t difference( void_t ) const { return _end - _begin ; }

    };
}

#endif