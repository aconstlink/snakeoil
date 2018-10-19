
#include "api.h"
#include "typedefs.h"
#include "string/string.hpp"

namespace so_std
{
    struct SNAKEOIL_IMEX_API dummy
    {
        static so_std::string_t dummy_strings( void_t ) { 
            static so_std::string_t __dummies[] = { "a", "b", "c" } ;
            return __dummies[ 0 ] ;
        }


    };
    
}