//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_LOG_STD_CERR_LOGGER_H_
#define _SNAKEOIL_LOG_STD_CERR_LOGGER_H_

#include "logger.h"

#include <snakeoil/thread/typedefs.h>
#include <snakeoil/thread/mutex.h>

namespace so_log
{
    class std_cerr_logger : public logger
    {
        so_thread::mutex_t _mtx ;

    public:

        std_cerr_logger( void_t ) ;
        virtual ~std_cerr_logger( void_t ) ;

    public:

        virtual so_log::result log( so_log::log_level ll, std::string const & msg ) ;
    };
}

#endif

