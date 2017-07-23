//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_BUFFER_TYPE_H_
#define _SNAKEOIL_SHADE_BUFFER_TYPE_H_

#include "typedefs.h"
#include "type.h"

namespace so_shade
{
    enum class buffer_dimension
    {
        invalid,
        dim_1d
    };
    
    struct buffer_type
    {
        so_this_typedefs( buffer_type ) ;

        type_base base ;
        buffer_dimension dim ;

    public:

        buffer_type( void_t ){}
        buffer_type( this_cref_t rhv ) : 
            base(rhv.base), dim(rhv.dim){}
        buffer_type( this_rref_t rhv ) :
            base( std::move(rhv.base) ), dim( std::move(rhv.dim) ){}
        ~buffer_type( void_t ) {}

        this_ref_t operator = ( this_cref_t rhv )
        {
            base = rhv.base ;
            dim = rhv.dim ;
            return *this ;
        }

        this_ref_t operator = ( this_rref_t rhv ) 
        {
            base = rhv.base ;
            dim = rhv.dim ;
            return *this ;
        }

    public:

        type_base get_type_base( void_t ) const { return base ; }
        buffer_dimension get_dimension( void_t ) const { return dim ; }
    };
}

#endif
