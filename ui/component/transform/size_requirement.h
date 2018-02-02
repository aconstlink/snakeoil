//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../icomponent.h"

#include <snakeoil/math/vector/vector4.hpp>

namespace so_ui
{
    namespace so_component
    {
        class SNAKEOIL_UI_API size_requirement : public component_base<size_requirement>
        {
            so_this_typedefs( size_requirement ) ;

        public:
            
            enum class size_type
            {
                percent,
                pixel
            };

            struct size_data
            {
                so_this_typedefs( size_data ) ;

            public:

                size_type st_width = size_type::percent ;
                float_t width = 1.0f ;

                size_type st_height = size_type::percent ;
                float_t height = 1.0f ;

            public:

                static this_t by_pixel_height( float_t const v )
                {
                    this_t sr ;
                    sr.st_height = size_type::pixel ;
                    sr.height = v ;
                    return sr ;
                }

                static this_t by_pixel_width( float_t const v )
                {
                    this_t sr ;
                    sr.st_width = size_type::pixel ;
                    sr.width = v ;
                    return sr ;
                }

                static this_t by_pixel_width_and_height( float_t const w, float_t const h )
                {
                    this_t sr ;

                    sr.st_width = size_type::pixel ;
                    sr.width = w ;

                    sr.st_height = size_type::pixel ;
                    sr.height = h ;

                    return sr ;
                }
            };
            so_typedef( size_data ) ;

        private:

            size_type _st_width = size_type::percent ;
            float_t _width = 1.0f ;

            size_type _st_height = size_type::percent ;
            float_t _height = 1.0f ;

        public:

            size_requirement( void_t ) ;
            size_requirement( size_data_cref_t ) ;
            size_requirement( this_cref_t ) = delete ;
            size_requirement( this_rref_t ) ;
            virtual ~size_requirement( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            size_data_t get_size_data( void_t ) const ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( size_requirement ) ;
    }
}