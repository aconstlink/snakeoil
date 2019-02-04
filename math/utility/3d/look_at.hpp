//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MATH_UTILITY_U3D_LOOK_AT_HPP_
#define _SNAKEOIL_MATH_UTILITY_U3D_LOOK_AT_HPP_

#include "../../vector/vector3.hpp"
#include "../../vector/vector4.hpp"
#include "../../matrix/matrix3.hpp"
#include "../../matrix/matrix4.hpp"

namespace so_math
{
    namespace so_3d
    {
        template< typename type_t >
        void translate_camera_matrix_to( 
            so_math::vector3< type_t > const &pos,
            so_math::matrix4< type_t > & inout ) 
        {
            typedef so_math::vector4< type_t > vec4_t ;

            inout.set_column(3, vec4_t(pos) ) ;
            inout[15] = 1.0f ;
        }

        template< typename type_t >
        void create_lookat_from_vec( 
            so_math::vector3< type_t > const &pos, 
            so_math::vector3< type_t > const &dir, 
            so_math::vector3< type_t > const &up, 
            so_math::vector3< type_t > const &right,
            so_math::matrix4< type_t > & inout ) 
        {
            typedef so_math::vector4< type_t > vec4_t ;

            // recreate the object matrix
            inout.set_column(0, vec4_t(right) ) ; 
            inout.set_column(1, vec4_t(up) ) ;
            inout.set_column(2, vec4_t(dir) ) ;
            inout.set_column(3, vec4_t(pos) ) ;

            inout[15] = 1.0f ;
        }

        /// the 3x3 camera matrix is supposed to be orthonormal.
        template< typename type_t >
        void get_frame_from_camera_matrix( 
            so_math::vector3< type_t > & pos, 
            so_math::vector3< type_t > & dir, 
            so_math::vector3< type_t > & up, 
            so_math::vector3< type_t > & right,
            so_math::matrix4< type_t > const & mat_in ) 
        {
           
            mat_in.get_column(0, right ) ;
            mat_in.get_column(1, up ) ;
            mat_in.get_column(2, dir ) ;
            mat_in.get_column(3, pos) ;
            
        }

        /// the upper 3x3 camera matrix is supposed to be orthonormal.
        template< typename type_t >
        void get_dir_from_camera_matrix( 
            so_math::vector3< type_t > & dir,
            so_math::matrix4< type_t > const & mat_in ) 
        {
            mat_in.get_column(2, dir ) ;
        }

        /// the upper 3x3 camera matrix is supposed to be orthonormal.
        template< typename type_t >
        void get_up_from_camera_matrix( 
            so_math::vector3< type_t > & up,
            so_math::matrix4< type_t > const & mat_in ) 
        {
            mat_in.get_column(1, up ) ;
        }

        /// the upper 3x3 camera matrix is supposed to be orthonormal.
        template< typename type_t >
        void get_right_from_camera_matrix( 
            so_math::vector3< type_t > & right,
            so_math::matrix4< type_t > const & mat_in ) 
        {
            mat_in.get_column(0, right ) ;
        }

        template< typename type_t >
        void get_pos_from_camera_matrix( 
            so_math::vector3< type_t > & pos,
            so_math::matrix4< type_t > const & mat_in ) 
        {
            mat_in.get_column(3, pos ) ;
        }

        template< typename type_t >
        void set_pos_for_camera_matrix( 
            so_math::vector3< type_t > const & pos,
            so_math::matrix4< type_t > & mat_inout ) 
        {
            mat_inout.set_column(3, pos ) ;
        }

        /// the 3x3 view matrix is supposed to be orthonormal.
        template< typename type_t >
        void get_frame_from_view_matrix( 
            so_math::vector3< type_t > & dir, 
            so_math::vector3< type_t > & up, 
            so_math::vector3< type_t > & right,
            so_math::matrix4< type_t > const & mat_in ) 
        {
            mat_in.get_row(0, right ) ;
            mat_in.get_row(1, up ) ;
            mat_in.get_row(2, dir ) ;
        }

        /// the 3x3 view matrix is supposed to be orthonormal.
        template< typename type_t >
        void get_frame_from_view_matrix( 
            so_math::matrix3< type_t > & mat_out,
            so_math::matrix4< type_t > const & mat_in ) 
        {
            typename mat3_t::vec3_t vec ;
            mat_in.get_row(0, vec ) ; // right
            mat_out.set_column(0,vec) ;
            mat_in.get_row(1, vec ) ; // up
            mat_out.set_column(1,vec) ;
            mat_in.get_row(2, vec ) ; // dir
            mat_out.set_column(2,vec) ;
        }

        /// the 3x3 view matrix is supposed to be orthonormal.
        template< typename type_t >
        void get_dir_from_view_matrix( 
            so_math::vector3< type_t > & dir,
            so_math::matrix4< type_t > const & mat_in ) 
        {
            mat_in.get_row(2, dir ) ;
        }

        /*
        */
        template< typename type_t >
        void create_lookat_rh( 
            so_math::vector3< type_t > const &vcPos,
            so_math::vector3< type_t > const &vcUp,
            so_math::vector3< type_t > const &vcAt, 
            so_math::matrix4< type_t > & inout ) 
        {
            typedef so_math::vector3< type_t > vec3_t ;

            // get the position.
            vec3_t vcCurPos ;
            vcCurPos = (vcPos) ;
        
            // calculate the current lookat direction vector
            vec3_t vcLookAt( vcAt - vcCurPos ) ;

            // calculate new the up vector
            // for this, we need to use the world up vector
            vec3_t vcWorldUp = vcUp ;

            // Normalize the new lookat vector
            vec3_t vcLookAte = vec3_t(vcLookAt).normalize() ;

            // now, we have the projected vector.
            typename vec3_t::type_t c = vcLookAte.dot(vcWorldUp) ;
            vec3_t vcTemp = vcLookAte * c ;

            // its time to calculate the new up vector
            vec3_t vcUpe = vec3_t(vcWorldUp - vcTemp).normalize() ;

            // now we have the new up vector. 
            // With it, we can calculate the 
            // new right vector
            vec3_t vcRighte = vec3_t( vcUpe.crossed( vcLookAte )).normalize() ;

            create_lookat_from_vec<type_t>( vcPos, vcLookAte, vcUpe, vcRighte, inout ) ;
        }

        /*
        */
        template< typename type_t >
        void create_lookat_rh( 
            so_math::vector3< type_t > const &vcPos, 
            so_math::vector3< type_t > const &vcAt, 
            so_math::matrix4< type_t > & inout ) 
        {
            typedef so_math::vector3< type_t > vec3_t ;
            create_lookat_rh( 
                vcPos, vec3_t(so_math::so_vector::y_axis()), 
                vcAt, inout ) ;
        }

        /*
        */
        template< typename type_t >
        void create_lookat_rh_from_dir( 
            so_math::vector3< type_t > const &vcPos, 
            so_math::vector3< type_t > const &vcDir, 
            so_math::matrix4< type_t > & inout ) 
        {
            typedef so_math::vector3< type_t > vec3_t ;

            // get the position.
            vec3_t vcCurPos ;
            vcCurPos = vcPos ;

            // calculate the current lookat direction vector
            vec3_t vcLookAt( vcDir ) ;

            // calculate new the up vector
            // for this, we need to use the world up vector
            vec3_t vcWorldUp( 0.0f,1.0f,0.0f ) ;

            // Normalize the new lookat vector
            vec3_t vcLookAte = vec3_t( vcLookAt ).normalize() ;

            // now, we have the projected vector.
            typename vec3_t::type_t c = vcLookAte.dot( vcWorldUp ) ;
            vec3_t vcTemp = vcLookAte * c ;

            // its time to calculate the new up vector
            vec3_t vcUpe = vec3_t( vcWorldUp - vcTemp ).normalize() ;

            // now we have the new up vector. 
            // With it, we can calculate the 
            // new right vector
            vec3_t vcRighte = vec3_t( vcUpe.crossed( vcLookAte ) ).normalize() ;

            //vcRighte = vec3_t(vcWorldUp).normalize().cross( vcLookAte ) ;
            //vcUpe = vcLookAte.cross( vcRighte ) ;

            create_lookat_from_vec<type_t>( vcCurPos, vcLookAte, vcUpe, vcRighte, inout ) ;
        }

        template< typename type_t >
        void create_lookat_dir( 
            so_math::vector3< type_t > const & pos, 
            so_math::vector3< type_t > const & dir,
            so_math::matrix4< type_t > & inout ) 
        {
            create_lookat_rh_from_dir<type_t>( pos, dir, inout ) ;
        }

        /**
            This function inverts the camera matrix.
            The camera matrix is given by T*R. 
            Invert this gives: R^t * T^-1
            This transforms all points in world space, or relative space
            to the camera, into the camera space or view space.
        

            @param cm [in] The camera matrix.
            @param out [out] The viewing matrix.

            @note
            In OpenGL, this matrix must be loaded transposed.
            This is because OpenGL stores the matrices in row order.
            @endnote
        */
        template< typename type_t >
        void create_view_matrix( 
            so_math::matrix4< type_t > const & cm, 
            so_math::matrix4< type_t > & out )
        {
            typedef so_math::matrix3< type_t > mat3_t ;
            typedef so_math::matrix4< type_t > mat4_t ;
            typedef so_math::vector3< type_t > vec3_t ;

            vec3_t t = vec3_t(cm.column( 3 )) ;
            mat3_t cm3 =  mat3_t( cm ).transpose() ;
            out = mat4_t(cm3) ;
        
            out.set_column(3, vec3_t(-cm3.row_u(0).dot(t),-cm3.row_u(1).dot(t),-cm3.row_u(2).dot(t))) ;
        
            out[15] = 1.0f ;
        }

        template< typename type_t >
        void translate_view_matrix_to( 
            so_math::vector3< type_t > const & pos, 
            so_math::matrix4< type_t > & inout )
        {
            typedef so_math::vector3< type_t > vec3_t ;


            inout.set_column(3, vec3_t(-inout.row(0).dot(pos),-inout.row(1).dot(pos),-inout.row(2).dot(pos))) ;

            inout[15] = 1.0f ;
        }

    }  // namespace
}  // namespace

#endif

