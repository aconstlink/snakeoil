//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_GENERIC_PROPERTY_HPP_
#define _SNAKEOIL_PROPERTY_GENERIC_PROPERTY_HPP_

#include "../iproperty.h"

namespace so_property
{
    template< typename T > 
    class generic_base_property : public iproperty
    {
        so_this_typedefs( generic_base_property<T> ) ;
        so_typedefs( T, value ) ;

    private:

        so_std::utf8_t _name ;
        value_t _value ;

    public:

        generic_base_property( so_std::utf8_t name ) : _name(name)
        {}

        generic_base_property( this_cref_t rhv ) : _name(rhv._name), _value(rhv._value)
        {}

        generic_base_property( this_rref_t rhv ) : _name(std::move(rhv._name)), _value(std::move(rhv._value))
        {}

        generic_base_property( so_std::utf8_t name, value_cref_t value ) : _name(name), _value(value)
        {}

        generic_base_property( so_std::utf8_t name, value_rref_t value ) : _name( name ), _value(std::move(value))
        {}

        virtual ~generic_base_property( void_t )
        {}

    protected:

        void_t set( value_cref_t v ) 
        {
            _value = v ;
        }

        void_t set( value_rref_t v )
        {
            _value = std::move(v) ;
        }

        value_cref_t get( void_t ) const 
        {
            return _value ;
        }

    public:

        virtual so_std::utf8_cref_t get_name( void_t ) const final
        {
            return _name ;
        }

        virtual void_t apply( ivisitor_ptr_t v ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
