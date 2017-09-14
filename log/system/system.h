//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_LOG_DEFAULT_SYSTEM_H_
#define _SNAKEOIL_LOG_DEFAULT_SYSTEM_H_

#include "isystem.h"

#include <vector>
#include <map>
#include <string>

namespace so_log
{
    class system : public isystem
    {
        so_this_typedefs( system ) ;
        so_typedefs( std::vector< logger_ptr_t > , loggers ) ;

        loggers_t _loggers ;

    private:

        logger_ptr_t _default_logger ;
        logger_ptr_t _this_default_logger ;

    public:

        static this_ptr_t create( void_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        system( void_t ) ;
        virtual ~system( void_t ) ;

        virtual result log( so_log::log_level ll, std::string const & msg ) ;
        virtual result add_logger( logger_ptr_t lptr ) ;
        virtual logger_ptr_t set_default_logger( logger_ptr_t lptr ) ;
    };
    so_typedef( system ) ;
}

#endif

