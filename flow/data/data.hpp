//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_DATA_DATA_HPP_
#define _SNAKEOIL_FLOW_DATA_DATA_HPP_

#include "idata.h"

namespace so_flow
{
    template< typename T >
    class data : public idata
    {
        so_this_typedefs( data<T> ) ;

        so_typedefs( T, type ) ;

    private:

        type_ptr_t _data ;

    public:

        data( void_t ) : _data(0)
        {}

        data( type_ptr_t d ) : _data(d)
        {}

        virtual ~data( void_t ) 
        {}

    public:

        type_cptr_t get_ptr( void_t ) const
        {
            return _data ;
        }

        bool_t get_checked( type_ref_t d ) const
        {
            if( so_core::is_not_nullptr(_data) )
            {
                d = *_data ;
                return true ;
            }

            return false ;
        }

        type_cref_t get( void_t ) const
        {
            return *_data ;
        }

        void_t set_ptr( type_ptr_t d ) 
        {
            _data = d ;
        }

        bool_t set( type_cref_t d )
        {
            if( so_core::is_not_nullptr(_data) )
            {
                *_data = d ;
                return true ;
            }
            return false ;
        }

    };
}

#endif
