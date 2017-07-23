//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_IPROPERTY_H_
#define _SNAKEOIL_PROPERTY_IPROPERTY_H_

#include "typedefs.h"
#include "result.h"
#include "api.h"
#include "protos.h"

namespace so_property
{
    class SNAKEOIL_PROPERTY_API iproperty
    {
    public:

        virtual so_std::utf8_cref_t get_name( void_t ) const = 0 ;

        virtual void_t apply( so_property::ivisitor_ptr_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
