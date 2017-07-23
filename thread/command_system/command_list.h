//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_COMMAND_LIST_H_
#define _SNAKEOIL_THREAD_COMMAND_LIST_H_

#include "command.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_thread
{
    class command_list : public command
    {
        typedef command_list this_t ;
        typedef so_std::vector< command_ptr_t > commands_t ;

    private:
        
        command_state _state = permanent ;
        size_t _cur_pos = 0 ;
        commands_t _commands ;
        
    public:

        command_list( void_t ) ;
        command_list( command_state s ) ;
        command_list( this_t const & rhv ) ;
        command_list( this_t && rhv ) ;

        virtual ~command_list( void_t ) ;

        void_t put_command( command_ptr_t com_ptr ) ;
        virtual command_state execute( void_ptr_t user_ptr ) ;
    };

    typedef command_list * command_list_ptr_t ;
}

#endif

