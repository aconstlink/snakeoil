//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_CONNECTOR_REGISTRY_H_
#define _SNAKEOIL_SHADE_SLOT_CONNECTOR_REGISTRY_H_

#include "iconnector.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    class SNAKEOIL_SHADE_API connector_registry
    {
        so_this_typedefs( connector_registry ) ;

        so_typedefs( so_std::vector<iconnector_ptr_t>, connectors ) ;

    private:

        connectors_t _connectors ;

    public:

        connector_registry( void_t ) ;
        connector_registry( this_rref_t ) ;
        ~connector_registry( void_t ) ;

    public:

        so_shade::result register_connector( so_shade::iconnector_ptr_t ) ;
        so_shade::result unregister_connector( so_shade::iconnector_ptr_t ) ;

    public:

        bool_t call_on_connect( so_shade::iinput_slot_ptr_t, 
            so_shade::ioutput_slot_ptr_t, connect_direction ) ;
        
        void_t call_on_disconnect( so_shade::iinput_slot_ptr_t ) ;
        void_t call_on_disconnect( so_shade::ioutput_slot_ptr_t ) ;
        
        
    };
}

#endif
