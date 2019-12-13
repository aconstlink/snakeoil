#pragma once

#include "../../typedefs.h"

#include <snakeoil/flow/slot/input/input_slot.hpp>
#include <snakeoil/flow/node/variable/variable_node.h>

#include <snakeoil/gpu/viewport/viewport_2d.h>
#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/core/macros/typedef.h>

#include <vector>

namespace sox_presentation
{
    namespace transitions
    {
        struct sdf_a_shared_data
        {
            so_math::mat4f_t proj ;
            so_math::mat4f_t view ;

            so_flow::variable<so_gpu::viewport_2d_t>* win_vp ;

            so_flow::variable_set_t vs ;
            so_flow::variable_node_t vn ;

            std::chrono::milliseconds ms_trans = std::chrono::milliseconds(0) ;

            sdf_a_shared_data( void_t ) noexcept
            {
                win_vp = so_flow::variable<so_gpu::viewport_2d_t>::create(
                    so_gpu::viewport_2d_t(), "[rects_shared_data] : variable" ) ;

                vs.add( "vp_window", win_vp ) ;
                vn.create_input_connections( vs ) ;
            }

        };
        so_typedef( sdf_a_shared_data ) ;
    }
}