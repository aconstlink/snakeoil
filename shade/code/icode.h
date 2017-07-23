//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_CODE_ICODE_H_
#define _SNAKEOIL_SHADE_CODE_ICODE_H_

#include "../protos.h"
#include "../typedefs.h"
#include "../result.h"
#include "../api.h"

#include "code_type.h"
#include "version.h"
#include "shader_type.h"

namespace so_shade
{
    class SNAKEOIL_SHADE_API icode
    {

    public:

        virtual so_shade::shader_type get_shader_type( void_t ) const = 0 ;
        virtual so_shade::code_type get_type( void_t ) const = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
