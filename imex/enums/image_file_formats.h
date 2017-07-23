//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_ENUMS_IMAGE_TYPES_H_
#define _SNAKEOIL_IMEX_ENUMS_IMAGE_TYPES_H_

namespace so_imex
{
    enum class image_file_format
    {
        undefined,
        bmp, // Windows/OS2 Format
        jpg, // Independent JGEP Group
        png, // Portable Network Graphics
        gif, // Graphics Interchange Format
        tif, // Tagged Image File Format
        tga, // Truevision Targa File
        hdr, // High Dynamic Range
        exr, // OpenEXR
        ico, // Windows Icon
        dds // direct draw surface
    };
}

#endif

