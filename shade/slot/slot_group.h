//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_SLOT_H_
#define _SNAKEOIL_SHADE_SLOT_SLOT_H_

#include "islot.h"
#include "../type.h"
#include "connector_registry.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/string/string.hpp>

namespace so_shade
{
    class SNAKEOIL_SHADE_API slot_group : public so_shade::islot, public connector_registry
    {
        so_this_typedefs( slot_group ) ;

        so_typedefs( so_std::vector<so_shade::islot_ptr_t>, slots ) ;

    protected:

        typedef std::function< bool_t( this_ptr_t ) > exchange_funk_t ;

    private:

        
        slots_t _slots ;

    private:

        slot_group( this_cref_t ) {} 

    protected:

        slot_group( void_t ) ;
        slot_group( this_rref_t ) ;

    public:

        virtual ~slot_group( void_t ) ;        
        virtual void_t destroy( void_t ) = 0 ;

    public:        
        
        bool_t has_any_connection( void_t ) const ;

    protected:

        bool_t exchange( this_ptr_t which_ptr, this_ptr_t with_ptr ) ;
        so_shade::result add_slot( islot_ptr_t ) ;
        
        /// only removes the slot. it does not disconnect it.
        so_shade::result remove_slot( islot_ptr_t ) ;

        /// will not disconnect the slots. 
        /// The slots will only be removed
        so_shade::result remove_all_slots( void_t ) ;

    };
    so_typedef( slot_group ) ;
}

#endif
