//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_DECORATOR_PROPERTY_H_
#define _SNAKEOIL_PROPERTY_DECORATOR_PROPERTY_H_

#include "../iproperty.h"

namespace so_property
{
    class SNAKEOIL_PROPERTY_API decorator_property : public iproperty
    {
        so_this_typedefs( decorator_property ) ;

    private:

        so_std::utf8_t _name ;
        so_property::iproperty_ptr_t _deco_ptr ;

    public:
        
        decorator_property( so_std::utf8_cref_t, so_property::iproperty_ptr_t ) ;
        decorator_property( this_cref_t ) = delete ;
        decorator_property( this_rref_t ) ;
        virtual ~decorator_property( void_t ) ;

    public:
        
        virtual so_std::utf8_cref_t get_name( void_t ) const final ;

        virtual void_t apply( so_property::ivisitor_ptr_t ) ;
        virtual void_t destroy( void_t ) ;

    protected:

        void_t traverse_decorated( so_property::ivisitor_ptr_t ) ;
    };
}

#endif