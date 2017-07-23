//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_PROPERTY_GROUP_H_
#define _SNAKEOIL_PROPERTY_PROPERTY_GROUP_H_

#include "../iproperty.h"
#include "../ivisitor.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_property
{
    class SNAKEOIL_PROPERTY_API group_property : public iproperty
    {
        so_this_typedefs( group_property ) ;

        so_typedefs( so_std::vector<iproperty_ptr_t>, properties ) ;

    private:
        
        so_std::utf8_t _name ;
        properties_t _properties ;

    public:

        group_property( so_std::utf8_cref_t ) ;
        group_property( this_cref_t ) = delete ;
        group_property( this_rref_t ) ;
        virtual ~group_property( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:
        
        bool_t add_child( iproperty_ptr_t pptr ) ;

    public:

        virtual so_std::utf8_cref_t get_name( void_t ) const final ;
        virtual void_t apply( so_property::ivisitor_ptr_t vptr ) ;
        virtual void_t destroy( void_t ) ;

    protected:

        void_t traverse_children( so_property::ivisitor_ptr_t vptr ) ;
    };
    so_typedef( group_property ) ;
}

#endif