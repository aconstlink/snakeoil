//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_DATA_GENERIC_DATA_FUNK_H_
#define _SNAKEOIL_FLOW_DATA_GENERIC_DATA_FUNK_H_

#include "../typedefs.h"
#include "../protos.h"

#include "data.hpp"

#include <functional>

namespace so_flow
{
    
    struct generic_funk
    {
        typedef std::function< bool_t( so_flow::idata_cptr_t, so_flow::idata_cptr_t ) >
            compatibility_funk_t ;

        typedef std::function< void_t( so_flow::idata_ptr_t, so_flow::idata_cptr_t ) >
            exchange_funk_t ;

        template< typename T >
        static compatibility_funk_t compatibility_funk( void_t )
        {
            so_typedefs( data<T>, data ) ;

            return [=]( so_flow::idata_cptr_t a, so_flow::idata_cptr_t b )
            {
                return  so_core::is_not_nullptr( dynamic_cast<data_cptr_t>( a ) ) &&
                    so_core::is_not_nullptr( dynamic_cast<data_cptr_t>( b ) ) ;
            } ;
        }

        template< typename T >
        static exchange_funk_t exchange_value_funk_no_check( void_t )
        {
            so_typedefs( data<T>, data ) ;

            return [=]( so_flow::idata_ptr_t a, so_flow::idata_cptr_t b )
            {
                data_ptr_t a_ptr = static_cast<data_ptr_t>( a ) ;
                data_cptr_t b_ptr = static_cast<data_cptr_t>( b ) ;

                a_ptr->set( b_ptr->get() ) ;
            } ;
        }
    };
}

#endif
