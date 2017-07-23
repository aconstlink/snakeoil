//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_CTM_CTM_HELPER_H_
#define _SNAKEOIL_IMEX_MODULE_CTM_CTM_HELPER_H_

#include "../../typedefs.h"

#include <openctm.h>

namespace so_imex
{
    struct ctm_helper
    {
        static so_std::string_t error_to_string( CTMenum e ) 
        {
            switch( e )
            {
            case CTM_INVALID_CONTEXT:
            return "CTM_INVALID_CONTEXT" ;
            case CTM_INVALID_ARGUMENT:
            return "CTM_INVALID_ARGUMENT" ;
            case CTM_INVALID_OPERATION:
            return "CTM_INVALID_OPERATION" ;
            case CTM_INVALID_MESH:
            return "CTM_INVALID_MESH" ;
            case CTM_OUT_OF_MEMORY:
            return "CTM_OUT_OF_MEMORY" ;
            case CTM_FILE_ERROR:
            return "CTM_FILE_ERROR" ;
            case CTM_BAD_FORMAT:
            return "CTM_BAD_FORMAT" ;
            case CTM_LZMA_ERROR:
            return "CTM_LZMA_ERROR" ;
            case CTM_INTERNAL_ERROR:
            return "CTM_INTERNAL_ERROR" ;
            case CTM_UNSUPPORTED_FORMAT_VERSION:
            return "CTM_UNSUPPORTED_FORMAT_VERSION" ;
            default:
            break ;
            }

            return "" ;
        }
    };
    so_typedef( ctm_helper ) ;
}

#endif