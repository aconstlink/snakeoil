//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TYPE_TRAITS_H_
#define _SNAKEOIL_GPU_TYPE_TRAITS_H_

#include "../typedefs.h"

#include "types.h"
#include "shader_variable_types.h"
#include "color_types.h"
#include "data_buffer_formats.h"

#include <limits>
#include <cstdlib>

namespace so_gpu
{
    template< typename type_t >
    class type_traits
    {
    } ;

    template<>
    class type_traits< uint_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tuint ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec1 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec1ui ;
        static const so_gpu::color_type gfx_color_type = color_type::intensity ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::intensity32ui ;
    } ;

    template<>
    class type_traits< so_math::vec2ui_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tuint ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec2 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec2ui ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec3ui_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tuint ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec3 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec3ui ;
        static const so_gpu::color_type gfx_color_type = color_type::rgb ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec4ui_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tuint ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec4 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec4ui ;
        static const so_gpu::color_type gfx_color_type = color_type::rgba ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::rgba32ui ;
    } ;

    template<>
    class type_traits< int_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tint ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec1 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec1i ;
        static const so_gpu::color_type gfx_color_type = color_type::intensity ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::intensity32i ;
    } ;

    template<>
    class type_traits< so_math::vec2i_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tint ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec2 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec2i ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec3i_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tint ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec3 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec3i ;
        static const so_gpu::color_type gfx_color_type = color_type::rgb ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec4i_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tint ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec4 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec4i ;
        static const so_gpu::color_type gfx_color_type = color_type::rgba ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::rgba32i ;
    } ;

    template<>
    class type_traits< float_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tfloat ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec1 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec1f ;
        static const so_gpu::color_type gfx_color_type = color_type::luminance ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::intensity32f ;
    } ;

    template<>
    class type_traits< so_math::vec2f_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tfloat ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec2 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec2f ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec3f_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tfloat ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec3 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec3f ;
        static const so_gpu::color_type gfx_color_type = color_type::rgb ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec4f_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tfloat ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec4 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec4f ;
        static const so_gpu::color_type gfx_color_type = color_type::rgba ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::rgba32f ;
    } ;

    template<>
    class type_traits< bool_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tbool ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec1 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec1b ;
        static const so_gpu::color_type gfx_color_type = color_type::intensity ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec2b_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tbool ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec2 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec2b ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec3b_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tbool ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec3 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec3b ;
        static const so_gpu::color_type gfx_color_type = color_type::rgb ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec4b_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tbool ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec4 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::vec4b ;
        static const so_gpu::color_type gfx_color_type = color_type::rgba ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< char_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tchar ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec1 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::undefined ;
        static const so_gpu::color_type gfx_color_type = color_type::intensity ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec2c_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tchar ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec2 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::undefined ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec3c_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tchar ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec3 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::undefined ;
        static const so_gpu::color_type gfx_color_type = color_type::rgb ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec4c_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tchar ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec4 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::undefined ;
        static const so_gpu::color_type gfx_color_type = color_type::rgba ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::rgba8i ;
    } ;

    template<>
    class type_traits< uchar_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tuchar ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec1 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::undefined ;
        static const so_gpu::color_type gfx_color_type = color_type::intensity ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec2uc_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tuchar ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec2 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::undefined ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec3uc_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tuchar ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec3 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::undefined ;
        static const so_gpu::color_type gfx_color_type = color_type::rgb ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::vec4uc_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tuchar ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::vec4 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::undefined ;
        static const so_gpu::color_type gfx_color_type = color_type::rgba ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::rgba8 ;
    } ;

    template<>
    class type_traits< so_math::mat2f_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tfloat ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::mat2 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::mat2f ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::mat3f_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tfloat ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::mat3 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::mat3f ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;

    template<>
    class type_traits< so_math::mat4f_t >
    {
    public:

        static const so_gpu::type gfx_type = type::tfloat ;
        static const so_gpu::type_struct gfx_type_struct = type_struct::mat4 ;
        static const so_gpu::shader_variable_type gfx_shader_variable_type = shader_variable_type::mat4f ;
        static const so_gpu::color_type gfx_color_type = color_type::undefined ;
        static const so_gpu::data_buffer_format gfx_data_buffer_format = data_buffer_format::undefined ;
    } ;
}

#endif

