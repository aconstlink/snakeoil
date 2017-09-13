//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include <snakeoil/std/string/string.hpp>

namespace so_device
{
    namespace so_output
    {
        enum class output_component_type
        {
            invalid,

            binary_led,
            multi_led,
            light_bar,
            value_motor,

            num_components
        };

        namespace so_internal
        {
            static const so_std::string_t output_component_type_names[] = { 
                "invalid", "binary_led", "multi_led", "light_bar", "value_motor"
            } ;
        }

        static so_std::string_cref_t to_string( output_component_type const t )
        {
            if( t >= output_component_type::num_components ) 
                return so_internal::output_component_type_names[ 0 ] ;

            return so_internal::output_component_type_names[ size_t(t) ] ;
        }

        static output_component_type from_string( so_std::string_cref_t name )
        {
            size_t i = 0 ;
            for( auto const & n : so_internal::output_component_type_names )
            {
                if( n == name ) break ;
                ++i ;
            }

            output_component_type const t = output_component_type( i ) ;
            if( t == output_component_type::num_components )
                return output_component_type::invalid ;

            return output_component_type( i ) ;
        }
    }
}