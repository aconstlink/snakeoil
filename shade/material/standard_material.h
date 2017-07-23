//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_MATERIAL_STANDARD_MATERIAL_STANDARD_MATERIAL_H_
#define _SNAKEOIL_SHADE_MATERIAL_STANDARD_MATERIAL_STANDARD_MATERIAL_H_

#include "material.h"

#include "../variable.h"

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>

namespace so_shade
{
    namespace so_material
    {
        class SNAKEOIL_SHADE_API standard_material : public material
        {
            so_this_typedefs( standard_material ) ;

            struct texture_info
            {
                so_this_typedefs( texture_info ) ;

                so_shade::texture_variable_t tex_var ;

                texture_info( void_t ) {}
                texture_info( this_cref_t rhv ) : tex_var(rhv.tex_var)
                {}

                texture_info( this_rref_t rhv ) : tex_var( std::move(rhv.tex_var) )
                {}

                this_ref_t operator = ( this_cref_t rhv )
                {
                    tex_var = rhv.tex_var ;
                    return *this ;
                }

                this_ref_t operator = ( this_rref_t rhv )
                {
                    tex_var = std::move(rhv.tex_var) ;
                    return *this ;
                }
                
            };
            so_typedef( texture_info ) ;
            so_typedefs( so_std::vector<texture_info_t>, texture_infos ) ;

            struct texture_layer
            {
                texture_infos_t texture_infos ;
                so_math::vec2f_t uv_scale = so_math::vec2f_t(1.0f) ;
                so_math::vec2f_t uv_offset = so_math::vec2f_t(0.0f) ;
            };
            so_typedef( texture_layer ) ;
            so_typedefs( so_std::vector<texture_layer_t>, texture_layers ) ;            

            struct color_layer
            {
                so_math::vec3f_t color ;
            };
            so_typedef( color_layer ) ;
            so_typedefs( so_std::vector<color_layer_t>, color_layers ) ;

        private:

            // input variable
            // output variable
            // variables

            // texture layers
            // texture layer ops
            // textures
            // per layer texture mix ops

        private: 
            
            bool_t _normal_layer = false ;

            size_t _lights = 0 ;

            texture_layers_t _texture_layers ;
            color_layers_t _color_layers ;

        public:

            standard_material( void_t ) ;
            standard_material( this_rref_t ) ;
            virtual ~standard_material( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public: // dummy

            void_t add_light( void_t ) ;

        public: 

            
            size_t get_num_lights( void_t ) const ;

            void_t add_normal_layer( void_t ) ;
            bool_t has_normal_layer( void_t ) const ;

            void_t add_texture_layer( void_t ) ;
            void_t add_texture_layer( so_math::vec2f_cref_t scale, so_math::vec2f_cref_t offset ) ;
            size_t get_num_texture_layers( void_t ) const ;
            bool_t get_texture_layer_coord_info( size_t, 
                so_math::vec2f_out_t, so_math::vec2f_out_t ) const ;

            so_shade::result add_texture( size_t layer, so_shade::texture_variable_in_t ) ;
            size_t get_num_textures( size_t ) const ;
            bool_t get_texture( size_t layer, size_t tex, so_shade::texture_variable_out_t ) ;

            void_t add_color_layer( so_math::vec3f_cref_t ) ;
            size_t get_num_color_layers( void_t ) const ;
            bool_t get_color_layer( size_t i, so_math::vec3f_out_t ) const ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( standard_material ) ;
    }
}

#endif
