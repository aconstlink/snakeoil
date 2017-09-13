//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/std/string/string.hpp>

namespace so_device
{
    namespace so_input
    {
        enum class input_component_type
        {
            invalid,
            binary_button,
            value_button,
            rotary_knob,
            slider,
            value_stick,
            single_touch,

            num_components
        };

        namespace so_internal
        {
            static const so_std::string_t input_component_type_names[] = {
                "invalid", "binary_button", "value_button", "rotary_knob", "slider",
                "value_stick","single_touch"
            } ;
        }

        static so_std::string_cref_t to_string( input_component_type const t )
        {
            if( t >= input_component_type::num_components )
                return so_internal::input_component_type_names[ 0 ] ;

            return so_internal::input_component_type_names[ size_t( t ) ] ;
        }

        static input_component_type from_string( so_std::string_cref_t name )
        {
            size_t i = 0 ;
            for( auto const & n : so_internal::input_component_type_names )
            {
                if( n == name ) break ;
                ++i ;
            }

            input_component_type const t = input_component_type( i ) ;
            if( t == input_component_type::num_components )
                return input_component_type::invalid ;

            return input_component_type( i ) ;
        }
    }
}