//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_PROPERTY_PROPERTY_MAP_H_
#define _SNAKEOIL_IMEX_PROPERTY_PROPERTY_MAP_H_

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"

#include <snakeoil/property/protos.h>
#include <snakeoil/std/container/map.hpp>

namespace so_imex
{
    class SNAKEOIL_IMEX_API property_map
    {
        so_this_typedefs( property_map ) ;

    public:

        struct data
        {
            so_std::utf8_t name ;
            so_std::utf8_t short_name ;
            so_std::utf8_t desc ;
            so_std::utf8_t extensions ;
            so_property::property_sheet_ptr_t props_import ;
            so_property::property_sheet_ptr_t props_export ;
        };
        so_typedef( data ) ;
        typedef std::function< void_t ( so_std::string_cref_t, data_cref_t ) > for_each_funk_t ;        

    private:

        typedef so_std::map< so_std::string_t, data_t > __props_t ;
        so_typedefs( __props_t, props ) ;

        props_t _props ;

    public:

        property_map( void_t ) ;
        property_map( this_rref_t ) ;
        property_map( this_cref_t ) = delete ;
        ~property_map( void_t ) ;

        this_ref_t operator = ( this_cref_t ) = delete ;
        this_ref_t operator = ( this_rref_t ) ;

    public:

        bool_t add_property_sheet( so_std::string_cref_t key, so_std::utf8_cref_t name,
            so_std::utf8_cref_t desc, so_property::property_sheet_ptr_t imp, 
            so_property::property_sheet_ptr_t exp ) ;

        bool_t add_property_sheet( so_std::string_cref_t key, this_t::data_rref_t ) ;

        void_t for_each( for_each_funk_t ) ;

    };
    so_typedef( property_map ) ;
}

#endif