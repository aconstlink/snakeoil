//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_DECORATOR_TRANSFORM_3D_H_
#define _SNAKEOIL_IMEX_NODE_DECORATOR_TRANSFORM_3D_H_

#include "../decorator.h"

#include <snakeoil/math/utility/3d/transformation.hpp>

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API transform_3d : public decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( transform_3d ) ;

            so_typedefs( so_math::so_3d::transformation<float_t>, trafo ) ;

        private:

            so_math::vec3f_t _rotation = so_math::vec3f_t(0.0f) ;
            so_math::vec3f_t _scale = so_math::vec3f_t(1.0f) ;
            so_math::vec3f_t _translation = so_math::vec3f_t(0.0f) ;

        private:

            trafo_t _trafo ;

        public:

            transform_3d( void_t ) ;
            transform_3d( trafo_cref_t ) ;
            transform_3d( node_ptr_t ) ;
            transform_3d( this_rref_t ) ;
            virtual ~transform_3d( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            trafo_cref_t get_transformation( void_t ) const ;
            void_t set_decorated( node_ptr_t ) ;

        public:

            so_math::vec3f_t get_rotation( void_t ) const { return _rotation ; }
            so_math::vec3f_t get_scale( void_t ) const { return _scale ; }
            so_math::vec3f_t get_translation( void_t ) const { return _translation ; }

            void_t set_rotation( so_math::vec3f_cref_t v ) ;
            void_t set_scale( so_math::vec3f_cref_t v ) ;
            void_t set_translation( so_math::vec3f_cref_t v ) ;

            void_t transform( so_math::vec3f_cref_t r, 
                so_math::vec3f_cref_t s, so_math::vec3f_cref_t t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( transform_3d ) ;
    }
}

#endif
