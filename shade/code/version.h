//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_CODE_VERSION_H_
#define _SNAKEOIL_SHADE_CODE_VERSION_H_

namespace so_shade
{
    struct code_version
    {
        so_this_typedefs( code_version ) ;

        size_t major = size_t(-1) ;
        size_t minor = size_t(-1) ;

        code_version( void_t ) 
        {}

        code_version( size_t major_, size_t minor_ ) : major(major_), minor(minor_)
        {}
        
        code_version( this_cref_t rhv ) : major(rhv.major), minor(rhv.minor) 
        {}

        ~code_version( void_t ) 
        {}
    };
    so_typedef( code_version ) ;
}

#endif
