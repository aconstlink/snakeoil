//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_INPUT_IINPUT_SLOT_H_
#define _SNAKEOIL_FLOW_INPUT_IINPUT_SLOT_H_

#include "islot.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API iinput_slot : public islot
    {
    public: // could be in islot 
        
        virtual idata_ptr_t get_data_ptr( void_t ) = 0 ;
        virtual idata_cptr_t get_data_ptr( void_t ) const = 0 ;
        virtual so_flow::result disconnect( void_t ) = 0 ;

    public: // input slot specific 

        virtual bool_t is_compatible( so_flow::ioutput_slot_ptr_t ) const = 0 ;
        virtual so_flow::result connect( so_flow::ioutput_slot_ptr_t ) = 0 ;        
        virtual so_flow::result disconnect( so_flow::ioutput_slot_ptr_t ) = 0 ;

    public: // islot interface

        virtual so_flow::inode_ptr_t get_owner( void_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
