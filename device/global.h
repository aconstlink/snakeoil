//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_GLOBAL_H_
#define _SNAKEOIL_DEVICE_GLOBAL_H_

#include "typedefs.h"
#include "protos.h"
#include "api.h"

namespace so_device
{
    class SNAKEOIL_DEVICE_API global
    {
        so_this_typedefs( global ) ;

    private:

        so_device::device_system_ptr_t _dsys_ptr = nullptr ;

    private: // singleton

        static this_ptr_t _ptr ;

    public:

        global( void_t ) ;
        global( this_cref_t ) = delete ;
        global( this_rref_t ) ;
        ~global( void_t ) ;

    public: // singleton functions

        static bool_t init( void_t ) ;
        static void_t deinit( void_t ) ;

        static this_ptr_t get( void_t ) ;

    public:

        static idevice_system_ptr_t device_system( void_t ) ;

    private:

        idevice_system_ptr_t __get_device_system( void_t ) ;

    };
    so_typedef( global ) ;
}

#endif