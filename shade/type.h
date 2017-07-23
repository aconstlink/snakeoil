//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_TYPE_H_
#define _SNAKEOIL_SHADE_TYPE_H_

#include "api.h"
#include "typedefs.h"

#include <snakeoil/std/container/vector.hpp>

#include <snakeoil/core/macros/typedef.h>

namespace so_shade
{
    enum class type_base
    {
        invalid,
        tbyte,
        tint,
        tuint,
        tfloat,
        tdouble
    };

    enum class type_struct
    {
        invalid,
        vector1,
        vector2,
        vector3,
        vector4,
        matrix2,
        matrix3,
        matrix4
    };

    class SNAKEOIL_SHADE_API type
    {
        so_this_typedefs( type ) ;

    private:

        type_base _tb ;
        type_struct _ts ;

    public:

        type( void_t ) 
        {}

        type( type_base tb, type_struct ts ) 
        {
            _tb = tb ;
            _ts = ts ;
        }

        type(this_cref_t rhv) : _tb(rhv._tb), _ts(rhv._ts)
        {}
        
        bool_t operator == ( this_cref_t rhv ) const
        {
            return _tb == rhv._tb && _ts == rhv._ts ;
        }

        bool_t operator != (this_cref_t rhv) const
        {
            return so_core::is_not( *this == rhv ) ;
        }

        this_ref_t operator = ( this_cref_t rhv )
        {
            _tb = rhv._tb ;
            _ts = rhv._ts ;

            return *this ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            _tb = rhv._tb;
            _ts = rhv._ts;

            return *this;
        }

        this_ref_t operator = ( type_base tb )
        {
            _tb = tb ;
            return *this ;
        }

        this_ref_t operator = ( type_struct ts ) 
        {
            _ts = ts ;
            return *this ;
        }

    public:

        type_base get_type_base( void_t ) const 
        {
            return _tb ;
        }

        type_struct get_type_struct( void_t ) const
        {
            return _ts ;
        }

    public:

        static this_t vec1f( void_t ) { return this_t( type_base::tfloat, type_struct::vector1) ; }
        static this_t vec1ui( void_t ) { return this_t( type_base::tuint, type_struct::vector1) ; }
        static this_t vec1i( void_t ) { return this_t( type_base::tint, type_struct::vector1) ; }

        static this_t vec2f( void_t ) { return this_t( type_base::tfloat, type_struct::vector2) ; }
        static this_t vec2ui( void_t ) { return this_t( type_base::tuint, type_struct::vector2) ; }
        static this_t vec2i( void_t ) { return this_t( type_base::tint, type_struct::vector2) ; }

        static this_t vec3f( void_t ) { return this_t( type_base::tfloat, type_struct::vector3) ; }
        static this_t vec3ui( void_t ) { return this_t( type_base::tuint, type_struct::vector3) ; }
        static this_t vec3i( void_t ) { return this_t( type_base::tint, type_struct::vector3) ; }

        static this_t vec4f( void_t ) { return this_t( type_base::tfloat, type_struct::vector4) ; }
        static this_t vec4ui( void_t ) { return this_t( type_base::tuint, type_struct::vector4) ; }
        static this_t vec4i( void_t ) { return this_t( type_base::tint, type_struct::vector4) ; }

        static this_t mat2f( void_t ) { return this_t( type_base::tfloat, type_struct::matrix2) ; }
        static this_t mat2ui( void_t ) { return this_t( type_base::tuint, type_struct::matrix2) ; }
        static this_t mat2i( void_t ) { return this_t( type_base::tint, type_struct::matrix2) ; }

        static this_t mat3f( void_t ) { return this_t( type_base::tfloat, type_struct::matrix3) ; }
        static this_t mat3ui( void_t ) { return this_t( type_base::tuint, type_struct::matrix3) ; }
        static this_t mat3i( void_t ) { return this_t( type_base::tint, type_struct::matrix3) ; }

        static this_t mat4f( void_t ) { return this_t( type_base::tfloat, type_struct::matrix3) ; }
        static this_t mat4ui( void_t ) { return this_t( type_base::tuint, type_struct::matrix3) ; }
        static this_t mat4i( void_t ) { return this_t( type_base::tint, type_struct::matrix3) ; }

        /*static this_t tex1df( void_t ) { return this_t( type_base::tfloat, type_struct::texture_1d) ; }
        static this_t tex1db( void_t ) { return this_t( type_base::tbyte, type_struct::texture_1d) ; }

        static this_t tex2df( void_t ) { return this_t( type_base::tfloat, type_struct::texture_2d) ; }
        static this_t tex2db( void_t ) { return this_t( type_base::tbyte, type_struct::texture_2d) ; }

        static this_t tex3df( void_t ) { return this_t( type_base::tfloat, type_struct::texture_3d) ; }
        static this_t tex3db( void_t ) { return this_t( type_base::tbyte, type_struct::texture_3d) ; }
        */

    };
    so_typedef( type ) ;

    so_typedefs( so_std::vector<type>, types ) ;
}

#endif
