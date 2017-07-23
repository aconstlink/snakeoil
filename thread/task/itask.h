//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_TASK_ITASK_H_
#define _SNAKEOIL_THREAD_TASK_ITASK_H_

#include "../protos.h"
#include "../api.h"
#include "../typedefs.h"
#include "../result.h"

#include "../container/vector.hpp"

#include <vector>

namespace so_thread
{
    class SNAKEOIL_THREAD_API itask
    {
        so_this_typedefs( itask ) ;

    public:

        so_typedefs( so_std::vector<itask_ptr_t>, tasks ) ;
        so_typedefs( so_thread::vector<itask_ptr_t>, locked_tasks ) ;

    protected:

        struct connection_accessor
        {
            static size_t connect( itask_ptr_t from, itask_ptr_t to ) 
            {
                return to->connect_incoming( from ) ;
            }

            static size_t disconnect( itask_ptr_t from, itask_ptr_t to ) 
            {
                return to->disconnect_incoming( from ) ;
            }
        };

    public: // dependencies

        virtual itask_ptr_t then( itask_ptr_t ) = 0 ;
                
    public: // run-time

        virtual void_t execute( locked_tasks_ref_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;

    protected:

        virtual void_t run( void_t ) = 0 ;

    private:

        virtual size_t connect_incoming( itask_ptr_t ) = 0 ;
        virtual size_t disconnect_incoming( itask_ptr_t ) = 0 ;
    };

    so_typedef( itask ) ;
}

#endif
