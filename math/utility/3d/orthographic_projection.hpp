//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_UTILITY_U3D_ORTHOGRAPHIC_PROJECTION_HPP_
#define _SNAKEOIL_MATH_UTILITY_U3D_ORTHOGRAPHIC_PROJECTION_HPP_

#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

namespace so_math
{
    namespace so_3d
    {
        /// left hand
        template< typename type_t >
        void create_orthographic_projection( type_t width, type_t height, type_t n, type_t f, 
            so_math::matrix4< type_t > & inout ) 
        {
            typedef so_math::vector4< type_t > vec4_t ;

            // make column vectors
            // but they are the row vectors of
            // the dx projection matrix.
            vec4_t vcX(type_t(2.0)/width,	type_t(0),				type_t(0),				type_t(0)) ;
            vec4_t vcY(type_t(0),			type_t(2.0)/height,		type_t(0),				type_t(0)) ;
            vec4_t vcZ(type_t(0),			type_t(0),				type_t(1.0)/(f-n),		-(n)/(f-n) ) ;
            vec4_t vcW(type_t(0),			type_t(0),				type_t(0),				type_t(1) ) ;

            inout.set_row( 0,vcX ) ;
            inout.set_row( 1,vcY ) ;
            inout.set_row( 2,vcZ ) ;
            inout.set_row( 3,vcW ) ;
        }
    }
}

#endif

