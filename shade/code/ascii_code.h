//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_CODE_ASCII_CODE_H_
#define _SNAKEOIL_SHADE_CODE_ASCII_CODE_H_

#include "code.h"

#include <snakeoil/std/string/string.hpp>

namespace so_shade
{
    class SNAKEOIL_SHADE_API ascii_code : public code
    {
        typedef code base_t ;
        so_this_typedefs( ascii_code ) ;

    private:

        so_std::string_t _code ;

    public:

        ascii_code( void_t ) ;
        ascii_code( so_std::string_cref_t ) ;
        ascii_code( so_shade::code_type,  
            so_shade::shader_type, so_std::string_cref_t ) ;
        ascii_code( this_cref_t ) ;
        ascii_code( this_rref_t ) ;
        virtual ~ascii_code( void_t ) ;

        this_ref_t operator = ( this_cref_t ) ;
        this_ref_t operator = ( this_rref_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

        void_t set_code( so_std::string_cref_t ) ;
        so_std::string_cref_t get_code( void_t ) const ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( ascii_code ) ;
}

#endif
