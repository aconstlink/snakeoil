//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_PIXEL_FORMAT_H_
#define _SNAKEOIL_IMEX_PIXEL_FORMAT_H_

namespace so_imex
{
    enum class pixel_format
    {
        undefined,
        r,
        rg,
        rgb,
        rgba
    };

    static size_t to_component_count( pixel_format pf )
    {
        switch( pf )
        {
        case so_imex::pixel_format::r: return 1 ;
        case so_imex::pixel_format::rg: return 2 ;
        case so_imex::pixel_format::rgb: return 3 ;
        case so_imex::pixel_format::rgba: return 4 ;
        default: return 0 ;
        }
        return 0 ;
    }

    static pixel_format from_component_count( size_t comp_count )
    {
        switch( comp_count )
        {
        case 0: return so_imex::pixel_format::undefined ;
        case 1: return so_imex::pixel_format::r ;
        case 2: return so_imex::pixel_format::rg ;
        case 3: return so_imex::pixel_format::rgb ;
        case 4: return so_imex::pixel_format::rgba ;
        }
        return so_imex::pixel_format::undefined;
    }
}

#endif
