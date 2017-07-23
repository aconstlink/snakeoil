//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_UTILITY_2D_COORD_TRANSFORMATOR_HPP_
#define _SNAKEOIL_UTILITY_2D_COORD_TRANSFORMATOR_HPP_

#include "../../typedefs.h"
#include "../../vector/vector2.hpp"

namespace so_math
{
	namespace so_2d
	{
		/// assumes the origin of source/destination coords to 
		/// have the origin at the borders.
		/// ndc: normalize device coords 
		/// nnc: negative normalized coords in [-1,1]
		/// pnc: positive normalized coords in [0,1]
		template< typename real_t >
		class coord_transformator
		{
			typedef vector2< uint_t > vec2ui_t ;
			typedef vec2ui_t const & vec2ui_cref_t ;

			typedef vector2< real_t > vec2r_t ;
			typedef vec2r_t const & vec2r_cref_t ;
			typedef vec2r_t & vec2r_ref_t ;

		private:

			vec2ui_t _src_res ;
			vec2ui_t _dst_res ;

		public:

			coord_transformator( vec2ui_cref_t source_res, vec2ui_cref_t dest_res )
			{
				_src_res = source_res ;
				_dst_res = dest_res ;
			}

			coord_transformator( vec2ui_cref_t dest_res )
			{
				_dst_res = dest_res ;
			}

		public: // transformation

			/// transforms from [0,1] to [-1,1]
			vec2r_t from_pnc_to_nnc( vec2r_cref_t ndc )
			{
				return ndc * vec2r_t(2.0) - vec2r_t(1.0) ;
			}

			/// transforms from [-1,1] to [0,1]
			vec2r_t from_nnc_to_pnc( vec2r_cref_t ndc )
			{
				return ndc * vec2r_t(0.5) + vec2r_t(0.5) ;
			}

		public: // from source

			/// @point in source space coords
			/// @return pndc
			vec2r_t from_src_to_pnc( vec2ui_cref_t point )
			{
				return vec2r_t(point) / vec2r_t(_src_res)  ;
			}

			/// @point in source space coords
			vec2r_t from_src_to_nnc( vec2ui_cref_t point )
			{
				return from_pnc_to_nnc( from_src_to_pnc(point) )  ;
			}

			vec2ui_t from_src_to_dst( vec2ui_cref_t point )
			{
				return vec2ui_t(from_src_to_pnc( point ) * vec2r_t(_dst_res)) ;
			}

		public: // from dest

			/// @point in destination space coords
			/// @return pndc
			vec2r_t from_dst_to_pnc( vec2ui_cref_t point )
			{
				return vec2r_t(point) / vec2r_t(_dst_res)  ;
			}

			/// @point in destination space coords
			vec2r_t from_dst_to_nnc( vec2ui_cref_t point )
			{
				return from_pnc_to_nnc( from_dst_to_pnc(point) )  ;
			}
		};

		typedef coord_transformator< float_t > coord_transformatorf_t ;
	}
}

#endif

