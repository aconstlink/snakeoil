//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_MULTI_CHOICE_PROPERTY_H_
#define _SNAKEOIL_PROPERTY_MULTI_CHOICE_PROPERTY_H_

#include "group_property.h"

namespace so_property
{
    class SNAKEOIL_PROPERTY_API multi_choice_property : public group_property
    {
        typedef group_property base_t ;
        so_this_typedefs( multi_choice_property ) ;

    public:

        multi_choice_property( so_std::utf8_cref_t ) ;
        multi_choice_property( this_cref_t ) = delete ;
        multi_choice_property( this_rref_t ) ;
        virtual ~multi_choice_property( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual void_t apply( so_property::ivisitor_ptr_t ) ;
        virtual void_t destroy( void_t ) ;
    };
    so_typedef( multi_choice_property ) ;
}

#endif