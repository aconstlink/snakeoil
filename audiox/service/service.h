//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_SERVICE_SERVICE_H_
#define _SNAKEOIL_AUDIOX_SERVICE_SERVICE_H_

#include "iglobal_service.h"
#include "ilocal_service.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/vector.hpp>

namespace so_audiox
{
    class SNAKEOIL_AUDIOX_API service : public iglobal_service, public ilocal_service
    {
        so_this_typedefs( service ) ;

    private:

        struct user_data
        {
            so_std::string_t name ;
            so_audiox::iuser_data_ptr_t ud_ptr ;
        };
        so_typedef( user_data ) ;
        so_typedefs( so_std::vector<user_data_t>, user_datas ) ;

        so_thread::mutex_t _mtx_ud ;
        user_datas_t _uds ;

    public:

        service( void_t ) ;
        service( this_cref_t ) = delete ;
        service( this_rref_t ) ;
        virtual ~service( void_t ) ;

    public:
    
        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public: // interface

        virtual bool_t register_user_data( so_std::string_cref_t, so_audiox::iuser_data_ptr_t ) ;
        virtual bool_t acquire_user_data( so_std::string_cref_t, so_audiox::iuser_data_ptr_t & ) ;
        virtual void_t destroy( void_t ) ;
    };
    so_typedef( service ) ;
}

#endif