//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_SNAKEOIL_CODE_CODE_FILE_CONTENT_H_
#define _SNAKEOIL_IMEX_SNAKEOIL_CODE_CODE_FILE_CONTENT_H_

#include "../../typedefs.h"

#include <string>
#include <list>

namespace so_imex
{
    namespace so_snakeoil
    {
        enum class code_fragment_type
        {
            invalid,
            file_path
        };

        static code_fragment_type to_shader_fragment_type( std::string const & s )
        {
            if( s == "file_path" )
            {
                return code_fragment_type::file_path ;
            }
            return code_fragment_type::invalid ;
        }

        struct code_fragment_file_content
        {
            struct item
            {
                key_t key ;
                code_fragment_type type ;
                std::string data ;
            };

            path_t file_path ;
            std::list< item > fragments ;
        };
    }    
}

#endif
