//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_MODULES_MODULE_STRUCTS_H_
#define _SNAKEOIL_DEVICE_MODULES_MODULE_STRUCTS_H_

#include "script_type.h"
#include <snakeoil/io/typedefs.h>

namespace so_device
{
    struct script_files_desc
    {
        so_device::script_type sc ;
        so_io::path_t path_to_scripts ;
    };
    so_typedef( script_files_desc ) ;

    struct script_desc
    {
        so_device::script_type sc ;
        so_std::string_t script ;
    };
    so_typedef( script_desc ) ;
}

#endif
