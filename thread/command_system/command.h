//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_THREAD_COMMAND_H_
#define _SNAKEOIL_THREAD_COMMAND_H_

#include "../typedefs.h"

//#include <snakeoil/memory/memory_pool/simple_paged_memory_pool.hpp>
#include <snakeoil/core/types.hpp>

namespace so_thread
{
    using namespace so_core::so_types ;

    enum command_state
    {
        permanent,		// the command system is not allowed to delete the command
        nonpermanent	// the command system is allowed to delete the command
    } ;

    struct command
    {
        command( void_t ) {}
        virtual ~command( void_t ) {}

        virtual command_state execute( void_ptr_t user_ptr ) = 0 ;
    };

    template< typename user_command_t >
    class carrier_command : public command
    {
        typedef carrier_command< user_command_t > this_t ;

        user_command_t _com ;        
        command_state _state ;

    public:

        carrier_command( void_t ){ _state = nonpermanent ; }
        carrier_command( user_command_t const & user_com ){ _com = user_com ; _state = nonpermanent ; }
        virtual ~carrier_command( void_t ) {}

    public:

        virtual command_state execute( void_ptr_t user_ptr ) { _com.execute(user_ptr) ; return _state ; }
    };

    typedef command * command_ptr_t ;

}

#endif

