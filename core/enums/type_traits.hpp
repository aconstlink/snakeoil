//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_ENUMS_TRAITS_HPP_
#define _SNAKEOIL_CORE_ENUMS_TRAITS_HPP_

#include "types.h"
#include "../types.hpp"


#include <limits>
#include <cstdlib>

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/math/matrix/matrix2.hpp>
#include <snakeoil/math/matrix/matrix3.hpp>
#include <snakeoil/math/matrix/matrix4.hpp>


namespace so_core
{
    using namespace so_core::so_types ;

    template< typename type_t >
    class type_traits
    {
        static const so_core::type core_type ;
        static const so_core::type_structure core_type_structure ;
    };

    template<>
    class type_traits< uint_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tuint ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec1 ;
    };

    template<>
    class type_traits< so_math::vec2ui_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tuint ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec2 ;
    };

    template<>
    class type_traits< so_math::vec3ui_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tuint ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec3 ;
    };

    template<>
    class type_traits< so_math::vec4ui_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tuint ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec4 ;
    };

    template<>
    class type_traits< int_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tint ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec1 ;
    };

    template<>
    class type_traits< so_math::vec2i_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tint ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec2 ;
    };

    template<>
    class type_traits< so_math::vec3i_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tint ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec3 ;
    };

    template<>
    class type_traits< so_math::vec4i_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tint ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec4 ;
    };

    template<>
    class type_traits< float_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tfloat ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec1 ;
    };

    template<>
    class type_traits< so_math::vec2f_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tfloat ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec2 ;
    };

    template<>
    class type_traits< so_math::vec3f_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tfloat ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec3 ;
    };

    template<>
    class type_traits< so_math::vec4f_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tfloat ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec4 ;
    };

    template<>
    class type_traits< bool_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tbool ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec1 ;
    };

    template<>
    class type_traits< so_math::vec2b_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tbool ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec2 ;
    };

    template<>
    class type_traits< so_math::vec3b_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tbool ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec3 ;
    };

    template<>
    class type_traits< so_math::vec4b_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tbool ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec4 ;
    };

    template<>
    class type_traits< char_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tchar ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec1 ;
    };

    template<>
    class type_traits< so_math::vec2c_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tchar ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec2 ;
    };

    template<>
    class type_traits< so_math::vec3c_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tchar ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec3 ;
    };

    template<>
    class type_traits< so_math::vec4c_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tchar ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec4 ;
    };

    template<>
    class type_traits< uchar_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tuchar ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec1 ;
    };

    template<>
    class type_traits< so_math::vec2uc_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tuchar ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec2 ;
    };

    template<>
    class type_traits< so_math::vec3uc_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tuchar ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec3 ;
    };

    template<>
    class type_traits< so_math::vec4uc_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tuchar ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::vec4 ;
    };

    template<>
    class type_traits< so_math::mat3f_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tfloat ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::mat3 ;
    };

    template<>
    class type_traits< so_math::mat4f_t >
    {
    public:

        static const so_core::type core_type = so_core::type::tfloat ;
        static const so_core::type_structure core_type_structure = so_core::type_structure::mat4 ;
    };    
}

#endif



