//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_LOG_LOGGER_H_
#define _SNAKEOIL_LOG_LOGGER_H_

#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"
#include "../log_level.h"

namespace so_log
{
    class logger
    {

        public:
            
            logger( void_t ) {}
            virtual ~logger( void_t ) {}
            
    public:

        virtual so_log::result log( so_log::log_level ll, std::string const & msg ) = 0 ;
    };
}

#endif

