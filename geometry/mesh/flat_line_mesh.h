//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GEOMETRY_FLAT_LINE_WIRE_H_
#define _SNAKEOIL_GEOMETRY_FLAT_LINE_WIRE_H_

#include "imesh.h"
#include "../enums/component_formats.h"

#include <vector>

namespace so_geo
{
    class SNAKEOIL_GEOMETRY_API flat_line_mesh : public imesh
    {
        so_this_typedefs( flat_line_mesh ) ;

    public:

        so_geo::vector_component_format position_format ;

        uints_t indices ;
        floats_t positions ;

    public:

        flat_line_mesh( void_t ) ;
        flat_line_mesh( this_rref_t ) ;
        virtual ~flat_line_mesh( void_t ) ;

        this_ref_t operator = ( this_rref_t ) ;
        
    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        this_t repeat_full( size_t const times ) const ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( flat_line_mesh ) ;
}

#endif
