//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_IMAGE_DEDUCE_IMAGE_FORMAT_H_
#define _SNAKEOIL_IMEX_IMAGE_DEDUCE_IMAGE_FORMAT_H_

#include "../typedefs.h"
#include "../enums/image_file_formats.h"

namespace so_imex
{
    struct deduce_image_format
    {
        static image_file_format from_path( so_io::path_cref_t  ) ;
        static std::string from_format( so_imex::image_file_format ) ;
    };
}

#endif
