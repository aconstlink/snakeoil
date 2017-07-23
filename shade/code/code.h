//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_CODE_CODE_H_
#define _SNAKEOIL_SHADE_CODE_CODE_H_

#include "icode.h"

namespace so_shade
{
    class SNAKEOIL_SHADE_API code : public icode
    {
        so_this_typedefs( code ) ;

    private:

        so_shade::shader_type _stype = so_shade::shader_type::undefined ;
        so_shade::code_type _type = so_shade::code_type::unknown ;
        
    public:

        code( void_t ) ;
        code( so_shade::code_type, so_shade::shader_type ) ;
        code( this_rref_t ) ;
        virtual ~code( void_t ) ;

    public:

        virtual so_shade::shader_type get_shader_type( void_t ) const ;
        virtual so_shade::code_type get_type( void_t ) const ;
        
    };
}

#endif
