//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_AUDIO_MANAGER_H_
#define _SNAKEOIL_IMEX_MANAGER_AUDIO_MANAGER_H_

#include "../api.h"
#include "../protos.h"

#include "base_resource_manager.hpp"

namespace so_imex
{
    /// manage scene graph structure from imported graph.
    class SNAKEOIL_IMEX_API audio_manager :
        public base_resource_manager<so_imex::iaudio>
    {
        so_this_typedefs( audio_manager ) ;
        so_typedefs( base_resource_manager<so_imex::iaudio>, base ) ;

    public:

        audio_manager( void_t ) ;
        audio_manager( this_cref_t ) = delete ;
        audio_manager( this_rref_t ) ;
        virtual ~audio_manager( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual void_t destroy( void_t ) ;

    };
    so_typedef( audio_manager ) ;
}

#endif