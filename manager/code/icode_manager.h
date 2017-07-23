//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_CODE_ICODE_MANAGER_H_
#define _SNAKEOIL_MANAGER_CODE_ICODE_MANAGER_H_

#include "../result.h"
#include "../typedefs.h"

namespace so_manager
{
    class icode_manager
    {
    public:

        virtual bool_t has_code( so_manager::key_cref_t ) = 0 ;

        /// just adds the code segment for the passed key.
        virtual so_manager::result add_code( so_manager::key_cref_t, 
            so_manager::string_cref_t code_in ) = 0 ;
            
        /// if the key exists, the code segment is replaced and the listeners are called.
        /// if the key does not exist, the code segment is added.
        virtual so_manager::result replace_code( so_manager::key_cref_t, 
            so_manager::string_cref_t code_in ) = 0 ;

        /// combine code segments referenced by the keys to one single code segment which is
        /// returned in code_out.
        virtual so_manager::result compose( so_manager::key_list_cref_t keys, 
            so_manager::string_ref_t code_out ) = 0 ;

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
