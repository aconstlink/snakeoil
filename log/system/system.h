//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "isystem.h"

#include "../logger/store_logger.h"

#include <vector>
#include <map>
#include <string>

namespace so_log
{
    class system : public isystem
    {
        so_this_typedefs( system ) ;
        so_typedefs( std::vector< so_log::ilogger_ptr_t > , loggers ) ;

        loggers_t _loggers ;
        so_log::store_logger_t _slogger ;

    private:

        so_log::ilogger_ptr_t _default_logger ;
        so_log::ilogger_ptr_t _this_default_logger ;

    public:

        static this_ptr_t create( void_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        system( void_t ) ;
        system( this_rref_t ) ;
        virtual ~system( void_t ) ;

        virtual result log( so_log::log_level ll, std::string const & msg ) ;
        virtual result add_logger( so_log::ilogger_ptr_t lptr ) ;
        virtual so_log::ilogger_ptr_t set_default_logger( so_log::ilogger_ptr_t lptr ) ;

        so_log::store_logger_cptr_t get_store( void_t ) const ;
    };
    so_typedef( system ) ;
}