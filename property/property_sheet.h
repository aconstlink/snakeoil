//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_PROPERTY_SHEET_H_
#define _SNAKEOIL_PROPERTY_PROPERTY_SHEET_H_

#include "api.h"
#include "typedefs.h"
#include "protos.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/string/string.hpp>

namespace so_property
{
    class SNAKEOIL_PROPERTY_API property_sheet
    {
        so_this_typedefs( property_sheet ) ;

    private:

        so_property::property_map_t _props ;
        so_property::iproperty_ptr_t _graph ;

    public:

        property_sheet( void_t ) ;
        property_sheet( so_property::iproperty_ptr_t ) ;
        property_sheet( this_cref_t ) = delete ;
        property_sheet( this_rref_t ) ;
        ~property_sheet( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t process_and_take_graph( so_property::iproperty_ptr_t ) ;

        /// allows to apply external visitor to the internal graph
        void_t apply( so_property::ivisitor_ptr_t ) ;        

    };
    so_typedef( property_sheet ) ;
}

#endif