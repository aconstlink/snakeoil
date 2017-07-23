//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_NUMERIC_PROPERTY_HPP_
#define _SNAKEOIL_PROPERTY_NUMERIC_PROPERTY_HPP_

#include "generic_property.hpp"

#include <limits>

namespace so_property
{
    template< typename T >
    class numeric_property : public generic_base_property<T>
    {
        typedef generic_base_property<T> base_t ;
        so_this_typedefs( numeric_property<T> ) ;
        so_typedefs( T, value ) ;

    private:
        
        value_t _min = std::numeric_limits<T>::min() ;
        value_t _max = std::numeric_limits<T>::max() ;

    public:

        numeric_property( so_std::utf8_t name ) : base_t( name )
        {}

        numeric_property( this_cref_t rhv ) : base_t( rhv ), _min(rhv._min), _max(rhv._max)
        {}

        numeric_property( this_rref_t rhv ) : base_t( std::move( rhv ) ), _min( rhv._min ), _max( rhv._max )
        {}

        numeric_property( so_std::utf8_t name, value_cref_t value ) : base_t( name, value )
        {}

        numeric_property( so_std::utf8_t name, value_cref_t value, value_cref_t min_val, value_cref_t max_val ) :
            base_t( name, std::move( value ) ), _min(min_val), _max(max_val)
        {}

        virtual ~numeric_property( void_t )
        {}

    public:

        static this_ptr_t create( so_memory::purpose_cref_t p )
        {
            return so_memory::memory::alloc( this_t(), p ) ;
        }

        static this_ptr_t create( this_rref_t prop, so_memory::purpose_cref_t p )
        {
            return so_memory::memory::alloc( std::move( prop ), p ) ;
        }

        static this_ptr_t create( so_std::utf8_cref_t name, value_cref_t v, so_memory::purpose_cref_t p )
        {
            return so_memory::memory::alloc( this_t( name, v ), p ) ;
        }

        static this_ptr_t create( so_std::utf8_cref_t name, value_cref_t v, value_cref_t min_val, value_cref_t max_val,
            so_memory::purpose_cref_t p )
        {
            return so_memory::memory::alloc( this_t( name, v, min_val, max_val ), p ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_memory::memory::dealloc( ptr ) ;
        }

    public:

        void_t set( value_cref_t v )
        {
            base_t::set( std::min( _max, std::max( _min, v ) ) ) ;
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

    so_typedefs( numeric_property<int_t>, int_property ) ;
    so_typedefs( numeric_property<uint_t>, uint_property ) ;
    so_typedefs( numeric_property<float_t>, float_property ) ;
}

#endif
