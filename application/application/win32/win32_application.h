//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_APPLICATION_WIN32_WIN32_APPLICATION_H_
#define _SNAKEOIL_APPLICATION_APPLICATION_WIN32_WIN32_APPLICATION_H_

#include "../../api.h"
#include "../window_application.h"

#include <snakeoil/device/protos.h>

namespace so_app
{
    namespace so_win32
    {
        class SNAKEOIL_APPLICATION_API win32_application : public window_application
        {
            typedef window_application base_t ;
            so_this_typedefs( win32_application ) ;

        private:

            /// can be used for special escape key pattern
            size_t _escape_sequence = 0 ;

            //so_device::so_module::so_win32::rawinput_ptr_t _rawinput_module_ptr = nullptr ;

        public:

            win32_application( void_t ) ;
            win32_application( this_rref_t ) ;
            virtual ~win32_application( void_t ) ;

        public:

            static this_ptr_t create( void_t ) ;
            static this_ptr_t create( this_rref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual so_app::result destroy( void_t ) ;
            virtual so_app::result exec_derived( void_t ) ;
        };
    }
    
}

#endif

