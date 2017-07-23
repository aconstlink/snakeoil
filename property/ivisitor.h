//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_IPROPERTY_VISITOR_H_
#define _SNAKEOIL_PROPERTY_IPROPERTY_VISITOR_H_

#include "api.h"
#include "typedefs.h"
#include "protos.h"

namespace so_property
{
    class SNAKEOIL_PROPERTY_API ivisitor
    {
    public:

        virtual void_t visit( group_property_ptr_t ) = 0 ;
        virtual void_t post_visit( group_property_ptr_t ) = 0 ;
        
        virtual void_t visit( decorator_property_ptr_t ) = 0 ;
        virtual void_t post_visit( decorator_property_ptr_t ) = 0 ;

        virtual void_t visit( single_choice_property_ptr_t ) = 0 ;
        virtual void_t post_visit( single_choice_property_ptr_t ) = 0 ;

        virtual void_t visit( multi_choice_property_ptr_t ) = 0 ;
        virtual void_t post_visit( multi_choice_property_ptr_t ) = 0 ;

        virtual void_t visit( so_property::iproperty_ptr_t ) = 0 ;
    };

    /// default implementation of ivisitor for default 
    /// unimplemented behavior
    class SNAKEOIL_PROPERTY_API visitor : public ivisitor
    {
        so_this_typedefs( visitor ) ;

    public:

        visitor( void_t ) {}
        visitor( this_rref_t ) {}
        visitor( this_cref_t ) {}
        virtual ~visitor( void_t ) {}

    public:

        virtual void_t visit( group_property_ptr_t ) {}
        virtual void_t post_visit( group_property_ptr_t ) {}

        virtual void_t visit( decorator_property_ptr_t ) {}
        virtual void_t post_visit( decorator_property_ptr_t ) {}

        virtual void_t visit( single_choice_property_ptr_t ) {}
        virtual void_t post_visit( single_choice_property_ptr_t ) {}

        virtual void_t visit( multi_choice_property_ptr_t ) {}
        virtual void_t post_visit( multi_choice_property_ptr_t ) {}

        virtual void_t visit( so_property::iproperty_ptr_t ) {}
    };
}

#endif