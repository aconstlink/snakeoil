//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_COLLIDE_3D_BOUNDS_AABB_HPP_
#define _SNAKEOIL_COLLIDE_3D_BOUNDS_AABB_HPP_

#include <snakeoil/math/vector/vector3.hpp>

namespace so_collide 
{
    namespace so_3d
    {
        template< typename type_t >
        class aabb
        {
            typedef aabb<type_t> this_t ;
            typedef this_t & this_ref_t ;
            typedef this_t const & this_cref_t ;

            typedef so_math::vector3< type_t > vec3_t ;
            typedef vec3_t const & vec3_cref_t ;
            typedef vec3_t & vec3_ref_t ;

			typedef so_math::vec3b_t vec3b_t ;

        private:

            vec3_t _max ;
            vec3_t _min ;

        public:

            aabb( void_t ){}

            aabb( vec3_cref_t max, vec3_cref_t min )
            {
                _max = max ;
                _min = min ;
            }

			aabb( vec3_cref_t center, type_t radius )
			{
				const vec3_t vmin = center - vec3_t(radius) ;
				const vec3_t vmax = center + vec3_t(radius) ;
			
				_max = vmax ;
				_min = vmin ;
			}

            aabb( this_cref_t rhv )
            {
                (*this) = rhv ;
            }

        public: // test

			/// is point inside the aabb.
            inline bool_t is_inside( vec3_cref_t p ) const
            {
				const vec3b_t test_max = p.less_than(_max) ;
				const vec3b_t test_min = p.greater_than(_min) ;

				return test_max.all() && test_min.all() ;
            }

		public: // transformation

            inline this_ref_t translate( vec3_cref_t translate )
			{
                this->set_min(_min+translate) ;
                this->set_max(_max+translate) ;
                return *this ;
            }

            inline this_t translated( vec3_cref_t translate ) const 
            {
                return this_t( *this ).translate() ;
            }

            void translate( vec3_cref_t translate, this_ref_t box_out ) const
			{
                box_out.set_max(this_t::get_max()+translate) ;
                box_out.set_min(this_t::get_min()+translate) ;
            }

            this_ref_t translate_to_position( vec3_t const & position ) 
            {
                vec3_t center = center() ;
                this_t::set_min(this_t::get_min()-center+position) ;
                this_t::set_max(this_t::get_max()-center+position) ;
                return *this ;
            }

        public:

            inline this_ref_t operator()( vec3_cref_t max, vec3_cref_t min )
            {
                _max = max ;
                _min = min ;
                return *this ;
            }

            inline this_ref_t operator = ( this_cref_t rhv )
            {
                return (*this)( rhv.get_max(), rhv.get_min() ) ;
            }

        public:

            inline vec3_cref_t get_max( void_t ) const {return _max;}
            inline vec3_cref_t get_min( void_t ) const {return _min;}
            inline vec3_ref_t get_max( void_t ){ return _max; }
            inline vec3_ref_t get_min( void_t ){ return _min; }

            inline void_t set_max( vec3_cref_t max ){_max=max;}
            inline void_t set_min( vec3_cref_t min ){_min=min;}

            inline vec3_t get_center( void_t ) const 
            {
                return (this_t::get_max() - this_t::get_min()) * type_t(0.5) + this_t::get_min() ;
            }

			vec3_t get_extend( void_t ) const 
			{
				return ((this_t::get_max() - this_t::get_min()) * type_t(0.5)) ;
			}

        };
    }
}

#endif

