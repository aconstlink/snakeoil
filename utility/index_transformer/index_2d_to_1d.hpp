//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_UTIL_INDEX_2D_TO_1D_HPP_
#define _SNAKEOIL_UTIL_INDEX_2D_TO_1D_HPP_

namespace so_utility
{
    namespace index_transformer
    {
        template< typename type_t >
        class index_2d_to_1d
        {
        private:

            type_t _width ;
            type_t _index ;

        public:

            index_2d_to_1d( type_t width )
            {
                _width = width ;
                _index = 0 ;
            }

        public:

            inline void_t add_1d( void_t ){ ++_index ; }
            inline void_t add_2d( void_t ){ _index+=_width ; }

            type_t get_index( void_t ) const 
            {
                return _index ;
            }
        };
    }
}

#endif

