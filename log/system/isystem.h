//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_LOG_SYSTEM_H_
#define _SNAKEOIL_LOG_SYSTEM_H_

#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"
#include "../log_level.h"

#include <string>

namespace so_log
{
    class isystem
    {
    public:

        virtual result log( so_log::log_level ll, std::string const & msg ) = 0 ;
        virtual result add_logger( so_log::ilogger_ptr_t lptr ) = 0 ;
        virtual so_log::ilogger_ptr_t set_default_logger( so_log::ilogger_ptr_t lptr ) = 0 ;

    };
}

#endif

