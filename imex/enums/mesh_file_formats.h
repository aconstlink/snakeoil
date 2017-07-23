//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_ENUMS_MESH_FILE_FORMATS_H_
#define _SNAKEOIL_IMEX_ENUMS_MESH_FILE_FORMATS_H_

#include "../typedefs.h"

namespace so_imex
{
    /// defines the ex/im-portable mesh types.
    /// the support is compile time dependent. If 
    /// the format is not compiled into the engine, 
    /// there will be no support.
    enum class mesh_file_format
    {
        unknown,
        raw,        // the engines own layout
        obj,        // Wavefront Obj format
        openctm     // OpenCTM format
    };

    namespace so_internal
    {
    }

    static mesh_file_format to_mesh_type( so_imex::string_cref_t ext )
    {
        if( ext == "ctm" )
            return mesh_file_format::openctm ;
        else if(ext == "obj")
            return mesh_file_format::obj ;
        else if(ext == "raw")
            return mesh_file_format::raw ;

        return mesh_file_format::unknown ;
    }
}

#endif
