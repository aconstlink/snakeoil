//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../icomponent.h"

#include <snakeoil/std/string/utf8.hpp>

namespace so_ui
{
    namespace so_component
    {
        class SNAKEOIL_UI_API text : public component_base<text>
        {
            so_this_typedefs( text ) ;

        private:

            so_std::string_t _text ;

        public:

            text( void_t ) ;
            text( so_std::string_cref_t ) ;
            text( this_cref_t ) = delete ;
            text( this_rref_t ) ;
            virtual ~text( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_text( so_std::string_cref_t ) ;
            so_std::string_cref_t get_text( void_t ) const ;
            
        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( text ) ;
    }
}