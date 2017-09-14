//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_BOOL_PROPERTY_HPP_
#define _SNAKEOIL_PROPERTY_BOOL_PROPERTY_HPP_

#include "generic_property.hpp"
#include "../ivisitor.h"

#include <limits>

namespace so_property
{
    class bool_property : public generic_base_property<bool_t>
    {
        typedef generic_base_property<bool_t> base_t ;
        so_this_typedefs( bool_property ) ;
        so_typedefs( bool_t, value ) ;

    public:

        bool_property( so_std::utf8_t name ) : base_t( name )
        {}

        bool_property( this_cref_t rhv ) : base_t( rhv )
        {}

        bool_property( this_rref_t rhv ) : base_t( std::move( rhv ) )
        {}

        bool_property( so_std::utf8_t name, value_cref_t value ) : base_t( name, value )
        {}

        bool_property( so_std::utf8_t name, value_rref_t value ) : base_t( name, std::move( value ) )
        {}

        virtual ~bool_property( void_t )
        {}

    public:

        static this_ptr_t create( this_rref_t prop, so_memory::purpose_cref_t p )
        {
            return so_memory::global::alloc( std::move( prop ), p ) ;
        }

        static this_ptr_t create( so_std::utf8_cref_t name, value_cref_t v, so_memory::purpose_cref_t p )
        {
            return so_memory::global::alloc( this_t( name, v ), p ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_memory::global::dealloc( ptr ) ;
        }

    public:

        void_t set( value_cref_t v )
        {
            base_t::set( v ) ;
        }

        value_cref_t get( void_t ) const
        {
            return base_t::get() ;
        }

        static bool_t get_value( so_property::iproperty_cptr_t pin, value_out_t vo )
        {
            this_cptr_t ptr = dynamic_cast< this_cptr_t >( pin ) ;
            if( so_core::is_nullptr( ptr ) )
                return false ;

            vo = ptr->get() ;

            return true ;
        }

    public:

        virtual void_t apply( ivisitor_ptr_t v )
        {
            if( so_core::is_nullptr( v ) )
                return ;

            v->visit( this ) ;
        }

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }
    };
    so_typedef( bool_property ) ;
}

#endif
