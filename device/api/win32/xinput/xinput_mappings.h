//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_DEVICE_API_XINPUT_XINPUT_MAPPINGS_H_
#define _SNAKEOIL_DEVICE_API_XINPUT_XINPUT_MAPPINGS_H_

#include "xinput_enums.h"

#include "../../../typedefs.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/string/string.hpp>

#include <functional>

#include <windows.h>

namespace so_device
{
    namespace so_win32
    {
        struct xinput_button_to_enum
        {
            DWORD xinput_button ;
            xinput_inputs button_enum ;
        };

        struct xinput_input_to_id
        {
            xinput_inputs input_enum ;
            size_t id ;
            so_std::string_t name ;
        };

        struct xinput_output_to_id
        {
            xinput_outputs output_enum ;
            size_t id ;
            so_std::string_t name ;
        };

        struct xinput_mappings
        {
            so_this_typedefs( xinput_mappings ) ;

            typedef std::function< void_t ( xinput_button_to_enum const & ) > xbutton_to_enum_funk_t ;
            typedef std::function< void_t ( xinput_input_to_id const & ) > input_to_id_funk_t ;
            typedef std::function< void_t ( xinput_output_to_id const & ) > output_to_id_funk_t ;

        public:

            so_std::vector< xinput_button_to_enum > button_to_enum ;
            so_std::vector< xinput_input_to_id > input_to_id ;
            so_std::vector< xinput_output_to_id > output_to_id ;

        public:

            xinput_mappings( void_t ) ;
            xinput_mappings( this_rref_t ) ;
            xinput_mappings( this_cref_t ) ;
            ~xinput_mappings( void_t ) ;

            this_ref_t operator = ( this_rref_t ) ;
            this_ref_t operator = ( this_cref_t ) ;

        public:

            bool_t enum_for_button_id( DWORD, xinput_inputs &) ;
            bool_t id_for_enum( xinput_inputs, size_t & ) ;
            bool_t id_name_for_enum( xinput_inputs, size_t &, so_std::string_ref_t ) ;
            bool_t id_for_enum( xinput_outputs, size_t & ) ;
            bool_t id_name_for_enum( xinput_outputs, size_t &, so_std::string_ref_t ) ;
            
            bool_t enum_for_id( size_t, xinput_outputs & ) ;

            bool_t deadzone_for_stick( xinput_inputs, uint16_t & ) ;

            void_t for_each_xinput_button( xbutton_to_enum_funk_t ) ;
            void_t for_each_button( input_to_id_funk_t ) ;
            void_t for_each_stick( input_to_id_funk_t ) ;
            void_t for_each_trigger( input_to_id_funk_t ) ;
            void_t for_each_motor( output_to_id_funk_t ) ;
        };        
    }
}

#endif
