//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IO_MATH_STRING_TO_VALUE_CONVERTER_H_
#define _SNAKEOIL_IO_MATH_STRING_TO_VALUE_CONVERTER_H_

#include "../../result.h"

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/std/string/split.hpp>

namespace so_io
{
    namespace io_math
    {
#if 0
        namespace so_internal
        {
            template< typename value_t >
            value_t convert_to_scalar( std::string const & s )
            {
                return boost::lexical_cast< value_t, std::string >(s) ;
            }

            template< typename vec_t, size_t comp >
            so_io::result convert_to_vector( std::string const & s, std::string const & sep, vec_t & vec_out )
            {

                typedef typename vec_t::type_t value_t ;
                typedef boost::tokenizer< boost::char_separator< char > > tokenizer_t ;
                tokenizer_t tokens( s, boost::char_separator<char>(sep.c_str()) ) ;

                size_t i = 0 ;
                std::for_each( tokens.begin(), tokens.end(), [&]( std::string const & s ) 
                {
                    if( i < comp ) vec_out[i] = convert_to_scalar<value_t>( s ) ; 
                    ++i ;
                }) ;

                
                return ( i != comp ) ? so_io::failed : so_io::ok ;
            }
        }

        template< typename value_t >
        so_io::result convert_to_value( std::string const & s, std::string const & sep, value_t & v_out )
        {
            v_out = so_internal::convert_to_scalar<value_t>( s ) ;
            return so_io::ok ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec2f_t>( std::string const & s, std::string const & sep, so_math::vec2f_t & v_out )
        {
            typedef so_math::vec2f_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 2>( s, sep, v_out ) ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec3f_t>( std::string const & s, std::string const & sep, so_math::vec3f_t & v_out )
        {
            typedef so_math::vec3f_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 3>( s, sep, v_out ) ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec4f_t>( std::string const & s, std::string const & sep, so_math::vec4f_t & v_out )
        {
            typedef so_math::vec4f_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 4>( s, sep, v_out ) ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec2i_t>( std::string const & s, std::string const & sep, so_math::vec2i_t & v_out )
        {
            typedef so_math::vec2i_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 2>( s, sep, v_out ) ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec3i_t>( std::string const & s, std::string const & sep, so_math::vec3i_t & v_out )
        {
            typedef so_math::vec3i_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 3>( s, sep, v_out ) ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec4i_t>( std::string const & s, std::string const & sep, so_math::vec4i_t & v_out )
        {
            typedef so_math::vec4i_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 4>( s, sep, v_out ) ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec2ui_t>( std::string const & s, std::string const & sep, so_math::vec2ui_t & v_out )
        {
            typedef so_math::vec2ui_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 2>( s, sep, v_out ) ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec3ui_t>( std::string const & s, std::string const & sep, so_math::vec3ui_t & v_out )
        {
            typedef so_math::vec3ui_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 3>( s, sep, v_out ) ;
        }

        template<>
        so_io::result convert_to_value<so_math::vec4ui_t>( std::string const & s, std::string const & sep, so_math::vec4ui_t & v_out )
        {
            typedef so_math::vec4ui_t vec_t ;
            return so_internal::convert_to_vector<vec_t, 4>( s, sep, v_out ) ;
        }
#endif
    }
}

#endif

