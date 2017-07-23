//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_SYSTEM_H_
#define _SNAKEOIL_APPLICATION_SYSTEM_H_

#include "protos.h"
#include "typedefs.h"
#include "result.h"
#include "api.h"

namespace so_app
{
    /// rserved for later use
    class /*SNAKEOIL_APPLICATION_API*/ system
    {
    private:

        static system_ptr_t _system_ptr ;

    private: // made private

        /// initializes the application layer system for providing application
        /// layer internal singletons.
        static so_app::result init( void_t ) ;
        static so_app::result deinit( void_t ) ;

    };
}

#endif

