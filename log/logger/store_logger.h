//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "logger.h"

#include <snakeoil/thread/typedefs.h>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/math/vector/vector2.hpp>

#include <functional>
#include <vector>

namespace so_log
{
    class SNAKEOIL_LOG_API store_logger : public  so_log::ilogger
    {
        so_this_typedefs( store_logger ) ;

    public:

        struct store_data
        {
            so_log::log_level ll ;
            std::string msg ;
        };
        so_typedef( store_data );

    private:

        size_t const max_elems = 10000 ;
        so_typedefs( std::vector< store_data_t >, stores );

        mutable so_thread::mutex_t _mtx ;
        stores_t _stores ;
        
        size_t _cp_start = 0 ;
        size_t _cp_end = 0 ;

    public:

        typedef std::function < void_t ( store_data_cref_t ) > funk_t ;

    public:

        store_logger( void_t ) ;
        store_logger( this_cref_t ) = delete ;
        store_logger( this_rref_t ) ;
        virtual ~store_logger( void_t ) ;

        this_ref_t operator = ( this_rref_t ) ;

    public:

        virtual so_log::result log( so_log::log_level ll, std::string const & msg ) ;

        size_t get_max_items( void_t ) const ;
        size_t get_num_items( void_t ) const ;
        void_t for_each( size_t const begin, size_t const end, funk_t ) const ;
        void_t copy( size_t const begin, size_t const end, stores_ref_t ) const ;

    };
    so_typedef( store_logger ) ;
}