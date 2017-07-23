//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_ISLOT_H_
#define _SNAKEOIL_SHADE_SLOT_ISLOT_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"
#include "../type.h"

#include <snakeoil/std/string/string.hpp>

namespace so_shade
{
    class SNAKEOIL_SHADE_API islot
    {

    public:

        virtual so_std::string_cref_t get_name( void_t ) const = 0 ;
        virtual so_shade::type_cref_t get_type( void_t ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;

    };
}

#endif
