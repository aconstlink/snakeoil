//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_OUTPUT_IOUTPUT_SLOT_H_
#define _SNAKEOIL_FLOW_OUTPUT_IOUTPUT_SLOT_H_

#include "../node/inode.h"
#include "islot.h"

#include "../data/generic_funk.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API ioutput_slot : public islot
    {
    public: // could be in islot 

        virtual idata_cptr_t get_data_ptr( void_t ) const = 0 ;
        virtual so_flow::result disconnect( void ) = 0 ;
        
    public: // output slot specific 

        virtual bool_t is_compatible( so_flow::iinput_slot_ptr_t ) const = 0 ;

        virtual so_flow::result connect( so_flow::iinput_slot_ptr_t ) = 0 ;
        virtual so_flow::result disconnect( so_flow::iinput_slot_ptr_t ) = 0 ;

        virtual so_flow::result update( void_t ) = 0 ;

        virtual void_t get_all_connected_owner( so_flow::inode::nodes_ref_t ) = 0 ;

    public: // islot interface

        virtual void_t set_owner( so_flow::inode_ptr_t ) = 0 ;
        virtual so_flow::inode_ptr_t get_owner( void_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;

    protected:

        virtual so_flow::generic_funk::exchange_funk_t exchange_funk( void_t ) const = 0 ;
    };
}

#endif
