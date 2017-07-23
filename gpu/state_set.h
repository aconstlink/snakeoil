//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_STATE_SET_H_
#define _SNAKEOIL_GPU_STATE_SET_H_

#include "enums/attribute_state.h"

#include "enums/render_states.h"
#include "enums/blend_factors.h"
#include "enums/blend_functions.h"
#include "enums/cull_modes.h"
#include "enums/front_faces.h"
#include "enums/fill_modes.h"

#include "viewport/viewport_2d.h"

#include <snakeoil/math/vector/vector4.hpp>

namespace so_gpu
{
    using namespace so_math ;
    
    struct color_state_set
    {
        vec4f_t clear_color = vec4f_t(0.0f) ;
    };

    struct blend_state_set
    {
        // blend active
        bool_t blend = false ;
        vec4f_t constant_blend_color = vec4f_t(0.0f) ;

        bool_t blend_factor_separate = false ;
        blend_factor src_blend_factor = so_gpu::blend_factor::one ;
        blend_factor dst_blend_factor = so_gpu::blend_factor::zero ;
        blend_factor src_color_blend_factor = so_gpu::blend_factor::one ;
        blend_factor dst_color_blend_factor = so_gpu::blend_factor::zero ;
        blend_factor src_alpha_blend_factor = so_gpu::blend_factor::one ;
        blend_factor dst_alpha_blend_factor = so_gpu::blend_factor::zero ;


        bool_t blend_func_separate = false ;
        blend_function blend_func = so_gpu::blend_function::add ;
        blend_function color_blend_func = so_gpu::blend_function::add ;
        blend_function alpha_blend_func = so_gpu::blend_function::add ;

    };

    struct depth_state_set
    {
        bool_t depth_test = false ;
        bool_t depth_write = false ;

        // depth buffer test func
    };

    struct stencil_state_set
    {
        // missing
    };

    struct polygon_state_set
    {
        bool_t cull = false ;

        cull_mode cm = so_gpu::cull_mode::back ;
        front_face ff = so_gpu::front_face::clock_wise ;

        // fill mode
        cull_mode cm_fill = so_gpu::cull_mode::front ;
        fill_mode fm = so_gpu::fill_mode::fill ;
    };

    struct viewport_state_set
    {
        so_gpu::viewport_2d vp ;
        // depth range
    };

    struct state_sets
    {
        blend_state_set blend_s ;
        color_state_set color_s ;
        depth_state_set depth_s ;
        stencil_state_set stencil_s ;
        polygon_state_set polygon_s ;
        viewport_state_set viewport_s ;
    };
    so_typedef( state_sets ) ;
}

#endif

