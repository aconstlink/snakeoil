//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_CONVERTER_GEOMETRY_BINDING_CONVERTER_H_
#define _SNAKEOIL_SCENE_CONVERTER_GEOMETRY_BINDING_CONVERTER_H_

#include "../typedefs.h"

#include <snakeoil/imex/node/attribute/geometry_binding.h>
#include <snakeoil/gpu/enums/vertex_attributes.h>

namespace so_scene
{
    struct geometry_binding_converter
    {
        static so_gpu::vertex_attribute from_imex_to_gpu( 
            so_imex::so_node::geometry_binding_point bp )
        {
            switch( bp )
            {
            case so_imex::so_node::geometry_binding_point::position:
                return so_gpu::vertex_attribute::position ;

            case so_imex::so_node::geometry_binding_point::normal:
                return so_gpu::vertex_attribute::normal ;

            case so_imex::so_node::geometry_binding_point::color0:
                return so_gpu::vertex_attribute::color0 ;

            case so_imex::so_node::geometry_binding_point::color1:
                return so_gpu::vertex_attribute::color1 ;

            case so_imex::so_node::geometry_binding_point::color2:
                return so_gpu::vertex_attribute::color2 ;

            case so_imex::so_node::geometry_binding_point::texcoord0:
                return so_gpu::vertex_attribute::texcoord0 ;

            case so_imex::so_node::geometry_binding_point::texcoord1:
                return so_gpu::vertex_attribute::texcoord1 ;

            case so_imex::so_node::geometry_binding_point::texcoord2:
                return so_gpu::vertex_attribute::texcoord2 ;
            default:
            break ;
            }
            return so_gpu::vertex_attribute::undefined ;
        }
    };
}

#endif
