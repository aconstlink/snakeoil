//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_NOISE_METHOD_FBM_HPP_
#define _SNAKEOIL_NOISE_METHOD_FBM_HPP_

#include "../typedefs.h"

#include <snakeoil/math/utility/fn.hpp>

#include <vector>

namespace so_noise
{
    template< typename noise_method_t >
    class fbm
    {
        typedef fbm<noise_t> fbm_t ;
        so_this_typedefs( fbm_t ) ;

        typedef typename noise_t::type_t type_t ;
        typedef so_math::vector2< type_t > _vec2_t ;
        typedef so_math::vector3< type_t > _vec3_t ;
        
        so_typedefs( noise_method_t, noise ) ;
        so_typedefs( _vec2_t, vec2 ) ;
        so_typedefs( _vec3_t, vec3 ) ;

    public:

        /// original from book tam
        static type_t noise( vec2_cref_t point, type_t h, type_t lacunarity, 
                type_t octaves, noise_cref_t noi )
        {
            type_t value = type_t(0) ;
            int_t i ;

            int_t const octaves_i = so_math::fn<type_t>::floor(octaves) ;
            type_t const remainder = octaves - type_t(octaves_i) ;

            vec2_t p = point ;

            for( i=0; i<(int)octaves_i; ++i )
            {
                value += noi.noise(p.x(), p.y() ) * 
                    so_math::fn<type_t>::pow(lacunarity, -h*type_t(i)) ;

                p *= lacunarity ;
            }

            value += remainder * noi.noise( p.x(), p.y() ) * 
                so_math::fn<type_t>::pow(lacunarity, -h*type_t(i)) ;

            return value ;
        }

        /// original from book tam
        static type_t noise( vec3_cref_t point, type_t h, type_t lacunarity, 
                type_t octaves, noise_cref_t noi )
        {
            type_t value = type_t(0) ;
            int i ;

            const int octaves_i = so_math::fn<type_t>::floor(octaves) ;
            const type_t remainder = octaves - type_t(octaves_i) ;

            vec3_t p = point ;

            for( i=0; i<(int)octaves_i; ++i )
            {
                value += noi.noise(p.x(), p.y(), p.z() ) * 
                    so_math::fn<type_t>::pow(lacunarity, -h*type_t(i)) ;

                p *= lacunarity ;
            }

            value += remainder * noi.noise( p.x(), p.y(), p.z() ) * 
                so_math::fn<type_t>::pow(lacunarity, -h*type_t(i)) ;

            return value ;
        }

        /// my tweeked version.
        static type_t noise2( vec2_cref_t point, type_t h, type_t lacunarity, 
            type_t lac_shift, type_t value_damp, type_t octaves, noise_cref_t noi )
        {
            type_t value = type_t(0) ;
            int_t i ;

            int_t const octaves_i = so_math::fn<type_t>::floor(octaves) ;
            type_t const remainder = octaves - type_t(octaves_i) ;

            vec2_t p = point ;

            for( i=0; i<(int)octaves_i; ++i )
            {
                value += noi.noise(p.x(), p.y() ) * 
                    so_math::fn<type_t>::pow(lacunarity+lac_shift, -h*type_t(i)) ;

                value *= value_damp ;
            
                p = (p + vec2f_t(0.72356f, -0.12324f)) * lacunarity ;
            }

            value += remainder * noi.noise(p.x(), p.y() ) * 
                so_math::fn<type_t>::pow(lacunarity, -h*type_t(i)) ;

            return value ;
        }

        /// my tweeked version.
        static type_t noise2( vec3_cref_t point, type_t h, type_t lacunarity, 
            type_t lac_shift, type_t value_damp, type_t octaves, noise_cref_t noi )
        {
            type_t value = type_t(0) ;
            int_t i ;

            int_t const octaves_i = so_math::fn<type_t>::floor(octaves) ;
            type_t const remainder = octaves - type_t(octaves_i) ;

            vec3_t p = point ;

            for( i=0; i<(int)octaves_i; ++i )
            {
            
                value += noi.noise(p.x(), p.y(), p.z() ) * 
                    so_math::fn<type_t>::pow(lacunarity+lac_shift, -h*type_t(i)) ;

                value *= value_damp ;
            
                p = (p + vec3f_t(0.72356f, -0.12324f, 1.29764f)) * lacunarity ;
            }

            value += remainder * noi.noise(p.x(), p.y(), p.z() ) * 
                so_math::fn<type_t>::pow(lacunarity, -h*type_t(i)) ;

            return value ;
        }
    };
}

#endif

