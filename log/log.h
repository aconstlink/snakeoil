//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_LOG_LOG_H_
#define _SNAKEOIL_LOG_LOG_H_

#include "api.h"

#include "log_level.h"
#include "protos.h"
#include "typedefs.h"

#include <iostream>
#include <cstdlib>

namespace so_log
{
    using namespace so_core::so_types ;
    
    class SNAKEOIL_LOG_API log
    {
        /// should be of type "system", so the log system could be exchanged
        /// by any other system. But since the system stores modules, that is
        /// currently not a big problem.
        static so_log::system * __default_log_system ;

    public:

        static void_t init( void_t ) ;
        static so_log::system * get_logging_system( void_t ) ;
        
        static void_t message( log_level level, string_cref_t msg ) ;
        static bool_t message( bool_t condition, log_level level, string_cref_t msg ) ;    
        static void_t status( string_cref_t msg ) ;
        static bool_t status( bool_t condition, string_cref_t msg ) ;
        static void_t warning( string_cref_t msg ) ;
        static bool_t warning( bool_t condition, string_cref_t msg ) ;
        static void_t error( string_cref_t msg ) ;
        static void_t error_and_exit( string_cref_t msg ) ;
        static void_t error_and_exit( bool_t condition, string_cref_t msg ) ;
        static bool_t error( bool_t condition, string_cref_t msg );
        static void_t critical( string_cref_t msg );
        static bool_t critical( bool_t condition, string_cref_t msg );	
    };        
}

#endif

