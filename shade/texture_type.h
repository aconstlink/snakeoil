//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_TEXTURE_TYPE_H_
#define _SNAKEOIL_SHADE_TEXTURE_TYPE_H_

#include "typedefs.h"
#include "type.h"

namespace so_shade
{
    enum class texture_dimension
    {
        invalid,
        dim_1d,
        dim_2d,
        dim_3d,
        dim_cube
    };
    enum class texture_encoding
    {
        invalid,
        color,
        depth
    };

    struct texture_type
    {
        so_this_typedefs( texture_type ) ;

        type_base base ;
        texture_dimension dim ;
        texture_encoding encoding ;

    public:

        this_ref_t operator = ( this_cref_t rhv )
        {
            base = rhv.base ;
            dim = rhv.dim ;
            encoding = rhv.encoding ;
            return *this ;
        }

        this_ref_t operator = ( this_rref_t rhv ) 
        {
            base = rhv.base ;
            dim = rhv.dim ;
            encoding = rhv.encoding ;
            return *this ;
        }

    public:

        type_base get_type_base( void_t ) const { return base ; }
        texture_dimension get_dimension( void_t ) const { return dim ; }
        texture_encoding get_encoding( void_t ) const { return encoding ; }
    };
}

#endif
