//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_OUTPUT_SLOT_OUTPUT_SLOT_H_
#define _SNAKEOIL_FLOW_OUTPUT_SLOT_OUTPUT_SLOT_H_

#include "../ioutput_slot.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/thread/mutex.h>

namespace so_flow
{
    class SNAKEOIL_FLOW_API output_slot : public ioutput_slot
    {
        so_this_typedefs( output_slot ) ;

        so_typedefs( so_std::vector<iinput_slot_ptr_t>, input_slots ) ;

    private:

        so_thread::mutex_t _mtx ;        
        input_slots_t _inputs ;

        so_flow::inode_ptr_t _owner_ptr = nullptr ;

    public:
        
        output_slot( void_t ) ;
        output_slot( this_rref_t ) ;
        virtual ~output_slot( void_t ) ;

    public: // interface
       
        virtual so_flow::result connect( so_flow::iinput_slot_ptr_t ) ;
        virtual so_flow::result disconnect( so_flow::iinput_slot_ptr_t ) ;
        virtual so_flow::result disconnect( void_t ) ;
        virtual so_flow::result update( void_t ) ;
        
        virtual void_t get_all_connected_owner( so_flow::inode::nodes_ref_t ) ;

    public: // owner 

        virtual void_t set_owner( so_flow::inode_ptr_t ) ;
        virtual so_flow::inode_ptr_t get_owner( void_t ) ;

    public: // still virtual 

        virtual idata_cptr_t get_data_ptr( void_t ) const = 0 ;
        virtual bool_t is_compatible( so_flow::iinput_slot_ptr_t ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;

    protected:

        virtual so_flow::generic_funk::exchange_funk_t exchange_funk( void_t ) const = 0 ;

    private:

        void_t disconnect_all( void_t ) ;
    };
    so_typedef( output_slot ) ;
}

#endif
