//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_PROPERTY_CONSTRUCTION_VISITOR_H_
#define _SNAKEOIL_PROPERTY_PROPERTY_CONSTRUCTION_VISITOR_H_

#include "../ivisitor.h"

#include <snakeoil/std/container/stack.hpp>

namespace so_property
{
    class SNAKEOIL_PROPERTY_API property_construction_visitor : public ivisitor
    {
        so_this_typedefs( property_construction_visitor ) ;

        typedef so_std::stack< so_std::utf8_t, 20 > __stack_t ;
        so_typedefs( __stack_t, stack ) ;

    private:

        stack_t _name_stack ;
        so_std::utf8_t _cur_eval ;
        so_property::property_map_ptr_t _map ;

    public:

        property_construction_visitor( so_property::property_map_ptr_t ) ;

        property_construction_visitor( this_rref_t ) ;
        property_construction_visitor( this_cref_t ) = delete ;
        virtual ~property_construction_visitor( void_t ) ;

        so_property::property_map_ref_t map( void_t ) ;

    public:

        virtual void_t visit( group_property_ptr_t ) ;
        virtual void_t post_visit( group_property_ptr_t ) ;

        virtual void_t visit( decorator_property_ptr_t ) ;
        virtual void_t post_visit( decorator_property_ptr_t ) ;

        virtual void_t visit( single_choice_property_ptr_t ) ;
        virtual void_t post_visit( single_choice_property_ptr_t ) ;

        virtual void_t visit( multi_choice_property_ptr_t ) ;
        virtual void_t post_visit( multi_choice_property_ptr_t ) ;

        virtual void_t visit( so_property::iproperty_ptr_t ) ;

    private:

        so_std::utf8_t construct_name( void_t ) ;
        so_std::utf8_t construct_name( so_std::utf8_cref_t end_name ) ;
        bool_t insert( so_std::utf8_cref_t, so_property::iproperty_ptr_t ) ;
    };
    so_typedef( property_construction_visitor ) ;
}

#endif