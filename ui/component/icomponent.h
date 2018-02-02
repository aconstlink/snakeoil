//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

namespace so_ui
{
    class SNAKEOIL_UI_API icomponent
    {
    public:

        virtual bool_t is_same_type( icomponent * ) = 0 ;
        virtual bool_t is_of_type( type_info const & ) = 0 ;

        virtual void_t destroy( void_t ) = 0 ;
    };

    template< typename comp >
    class component_base : public icomponent
    {
        so_this_typedefs( component_base<comp> ) ;
        so_typedefs( comp, component ) ;

    public:

        component_base( void_t ){}
        component_base( this_cref_t ) = delete ;
        component_base( this_rref_t ) {}
        virtual ~component_base( void_t ){}

    public:

        virtual bool_t is_of_type( type_info const & ti )
        {
            return typeid( component_t ) == ti ;
        }

        virtual bool_t is_same_type( icomponent * cptr )
        {
            return typeid(component_t) == typeid(*cptr) ;
        }

        virtual void_t destroy( void_t ) = 0 ;
    };
}