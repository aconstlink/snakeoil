//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PHYSICS_2D_INERTIA_HPP_
#define _SNAKEOIL_PHYSICS_2D_INERTIA_HPP_

namespace so_physics
{
	namespace so_2d
	{
		template< typename type_t >
		class inertia
		{
		public:

			static type_t for_circle( type_t radius )
			{
				return type_t(1) ;
			}

			static type_t for_rectangle( type_t width, type_t height )
			{
				return type_t(1) ;
			}
		};
	}
}

#endif

