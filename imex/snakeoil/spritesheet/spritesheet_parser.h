//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "spritesheet.h"

namespace so_imex
{
    namespace so_snakeoil
    {
        struct SNAKEOIL_IMEX_API spritesheet_parser
        {
            static spritesheet_t from_string( so_std::string_in_t ) ;
            static so_std::string_t from_data( spritesheet_in_t ) ;
        };
        so_typedef( spritesheet_parser ) ;
    }
}