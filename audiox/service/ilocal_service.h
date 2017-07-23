//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_SERVICE_ILOCAL_SERVICE_H_
#define _SNAKEOIL_AUDIOX_SERVICE_ILOCAL_SERVICE_H_

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"

namespace so_audiox
{
    class SNAKEOIL_AUDIOX_API ilocal_service
    {
    public:

        virtual bool_t register_user_data( so_std::string_cref_t, so_audiox::iuser_data_ptr_t ) = 0 ;
        virtual bool_t acquire_user_data( so_std::string_cref_t, so_audiox::iuser_data_ptr_t & ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif