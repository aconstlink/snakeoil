//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_SERVICE_IGLOBAL_SERVICE_H_
#define _SNAKEOIL_GPX_SERVICE_IGLOBAL_SERVICE_H_

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"

namespace so_gpx
{
    class SNAKEOIL_GPX_API iglobal_service
    {

    public:

        template< typename T >
        bool_t acquire_user_data( so_std::string_cref_t name, T * & ud_out )
        {
            so_gpx::iuser_data_ptr_t iud_ptr ;
            auto const res = this->acquire_user_data( name, iud_ptr ) ;
            if( so_core::is_not( res ) ) return false ;

            ud_out = static_cast< T* >( iud_ptr ) ;

            return true ;
        }

    public:

        virtual bool_t register_user_data( so_std::string_cref_t, so_gpx::iuser_data_ptr_t ) = 0 ;
        virtual bool_t acquire_user_data( so_std::string_cref_t, so_gpx::iuser_data_ptr_t & ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif