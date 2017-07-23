//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_INPUT_INPUT_SLOT_H_
#define _SNAKEOIL_FLOW_INPUT_INPUT_SLOT_H_

#include "../iinput_slot.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/thread/semaphore.hpp>

#include <functional>

namespace so_flow
{
    class SNAKEOIL_FLOW_API input_slot : public iinput_slot
    {
        so_this_typedefs( input_slot ) ;
        
    private:

        so_thread::mutex_t _mtx ;       

        so_flow::ioutput_slot_ptr_t _os_ptr = nullptr ;

        /// multi calls to connect input slot has too value checks
        /// so I just do the semaphore thing by just allowing one 
        /// entree at any time.
        so_thread::semaphore_t _in_connect ;
        
     
        // - exchange funk
        // - connection check funk

        so_flow::inode_ptr_t _owner_ptr = nullptr ;

    private:

        input_slot( this_cref_t ) {}

    public:

        input_slot( void_t ) ;
        input_slot( this_rref_t ) ;
        virtual ~input_slot( void_t ) ;

    public:

        virtual so_flow::result connect( so_flow::ioutput_slot_ptr_t os_ptr ) ;

        virtual so_flow::result disconnect( void_t ) ;
        virtual so_flow::result disconnect( so_flow::ioutput_slot_ptr_t ) ;

    public: // owner 

        virtual void_t set_owner( so_flow::inode_ptr_t ) ;
        virtual so_flow::inode_ptr_t get_owner( void_t ) ;


    public: // still virtual 
        
        virtual idata_ptr_t get_data_ptr( void_t ) = 0 ;
        virtual idata_cptr_t get_data_ptr( void_t ) const = 0 ;
        virtual bool_t is_compatible( so_flow::ioutput_slot_ptr_t ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;

  
    };
    so_typedef( input_slot ) ;
}

#endif
