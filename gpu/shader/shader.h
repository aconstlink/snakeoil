//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_SHADER_SHADER_H_
#define _SNAKEOIL_GPU_SHADER_SHADER_H_

#include "shader_types.h"

#include "../object.h"
#include "../typedefs.h"

#include <snakeoil/std/string/string.hpp>

namespace so_gpu
{
    class SNAKEOIL_GPU_API shader : public so_gpu::object
    {
    private:

        so_gpu::shader_type _type = shader_type::undefined ;
        so_std::string _code ;

    protected:

        shader( so_gpu::shader_type stype ) : _type(stype) {}

    public:

        shader( shader && rhv ) : object( std::move( rhv ) )
        { 
            _type = rhv._type ;
            _code = std::move( rhv._code ) ; 
        }

        virtual ~shader( void_t ) {}

        void_t add_code( so_std::string const & source ) { _code += source ; }
        void_t set_code( so_std::string const & source ) { _code = source ; }
        so_std::string const & get_code( void_t ) const { return _code; }

        bool_t has_no_code( void_t ) const ;
        bool_t has_code( void_t ) const ;

        shader_type get_shader_type( void_t ) const { return _type ; }
    };
    so_typedef( shader ) ;
}

#endif

