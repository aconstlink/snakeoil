//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_VARIABLE_ISLOT_H_
#define _SNAKEOIL_VARIABLE_ISLOT_H_

#include "typedefs.h"
#include "protos.h"
#include "api.h"
#include "result.h"

namespace so_var
{
    class SNAKEOIL_VARIABLE_API ivariable
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
