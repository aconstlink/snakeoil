//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_3D_BOUNDS_FRUSTUM_HPP_
#define _SNAKEOIL_COLLIDE_3D_BOUNDS_FRUSTUM_HPP_

#include "../../typedefs.h"
#include "../../planar/plane/hesse_plane3.hpp"

namespace so_math
{
    namespace so_3d
    {
        template< typename type_t >
        class frustum
        {
        public:

            typedef frustum< type_t > this_t ;
            typedef this_t & this_ref_t ;

            typedef so_math::hesse_plane< type_t, 3 > plane_t ;
            typedef plane_t const & plane_cref_t ;
            typedef plane_t & plane_ref_t ;

        private:

            plane_t _planes[6] ;

        public:

            enum frustum_plane
            {
                p_left,
                p_right,
                p_bottom,
                p_top,
                p_near,
                p_far
            } ;

        public:

            frustum( void_t )
            {

            }


            this_ref_t set_plane( frustum_plane fp, plane_cref_t plane )
            {
                _planes[fp] = plane ;
                return *this ;
            }

            plane_cref_t get_plane( size_t i ) const
            {
                so_assert(i < 6) ;
                return _planes[i] ;
            }
        };
    }
}

#endif

