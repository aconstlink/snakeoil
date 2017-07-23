//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GENERATOR_GLSL_GENERATOR_PROPERTIES_H_
#define _SNAKEOIL_SHADE_GENERATOR_GLSL_GENERATOR_PROPERTIES_H_

#include "../../typedefs.h"

namespace so_shade
{
    namespace so_generator
    {
        namespace so_glsl
        {
            struct generator_properties
            {
                so_std::string_t position_name = "in_pos" ;
                so_std::string_t normal_name = "in_nrm" ;
                so_std::string_t texcoord_pre_name = "in_txc_" ;

                bool_t enable_object_matrix = true ;
                so_std::string_t object_matrix_name = "u_mat_obj" ;
                so_std::string_t world_matrix_name = "u_mat_wrl" ;
                so_std::string_t view_matrix_name = "u_mat_view" ;
                so_std::string_t camera_matrix_name = "u_mat_cam" ;
                so_std::string_t projection_matrix_name = "u_mat_proj" ;
                
                bool_t enable_mvp_matrix = false ;
                so_std::string_t mvp_matrix_name = "u_mat_mvp" ;

                bool_t enable_linear_depth = true ;
                so_std::string_t projection_param_0_name = "u_proj_info_0" ;
            };
            so_typedef( generator_properties ) ;
        }
    }
}

#endif
