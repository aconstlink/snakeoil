//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_FEEDBACK_PRIMITIVE_TYPE_H_
#define _SNAKEOIL_GPU_API_GL_GL_ANY_GL_CONVERT_FEEDBACK_PRIMITIVE_TYPE_H_

#include "../../../../enums/primitive_types.h"
#include "../../../../enums/geometry_shader_output_types.h"
#include <snakeoil/gli/gl/gl.h>

namespace so_gpu
{
    namespace so_gl
    {
        namespace so_internal
        {
            static const primitive_type feedback_primitive_type_deduction_array[] = 
            {
                primitive_type::undefined,
                primitive_type::points,
                primitive_type::points,
                primitive_type::lines,
                primitive_type::lines,
                primitive_type::lines,
                primitive_type::triangles,
                primitive_type::triangles,
                primitive_type::triangles,
                primitive_type::undefined
            } ;

            static size_t num_feedback_prmitive_types_gl = 
                sizeof(feedback_primitive_type_deduction_array) / sizeof(feedback_primitive_type_deduction_array[0]) ;

            static const primitive_type feedback_geometry_output_type_deduction_array[] = 
            {
                primitive_type::undefined,
                primitive_type::points,
                primitive_type::lines,
                primitive_type::triangles
            } ;

            static size_t num_feedback_geometry_output_type_gl = 
                sizeof(feedback_geometry_output_type_deduction_array) / sizeof(feedback_geometry_output_type_deduction_array[0]) ;

        }

        static primitive_type deduce_transform_feedback( primitive_type const pt )
        {
            if( size_t(pt) >= so_internal::num_feedback_prmitive_types_gl ) return so_internal::feedback_primitive_type_deduction_array[0] ;
            return so_internal::feedback_primitive_type_deduction_array[(size_t)pt] ;
        }

        static primitive_type deduce_transform_feedback( geometry_shader_output_type const pt )
        {
            if( (size_t)pt >= so_internal::num_feedback_geometry_output_type_gl ) return so_internal::feedback_geometry_output_type_deduction_array[0] ;
            return so_internal::feedback_geometry_output_type_deduction_array[(size_t)pt] ;
        }        
    }
}

#endif

