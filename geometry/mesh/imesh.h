//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_MESH_IMESH_H_
#define _SNAKEOIL_GEOMETRY_MESH_IMESH_H_

#include "../typedefs.h"
#include "../api.h"
#include "../result.h"
#include "../protos.h"

#include "../enums/component_formats.h"

namespace so_geo
{
    class SNAKEOIL_GEOMETRY_API imesh
    {
    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( imesh ) ;
}

#endif
