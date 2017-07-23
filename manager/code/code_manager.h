//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_SHADER_CODE_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_SHADER_CODE_MANAGER_H_

#include "../api.h"
#include "icode_manager.h"

#include <snakeoil/thread/typedefs.h>

namespace so_manager
{
    class SNAKEOIL_MANAGER_API code_manager : public icode_manager
    {
        so_this_typedefs( code_manager ) ;

    private:

        struct code_data
        {
            code_data( void_t ) {}
            code_data( code_data && rhv ) :
            code(std::move(rhv.code)){}

            so_manager::string_t code ;

        private:

            code_data( code_data const & ){}
                        
            code_data & operator=( code_data const & rhv )
            {
                code = rhv.code ;
                return *this ;
            }
            
        public:
            
            code_data & operator=( code_data && rhv )
            {
                code = rhv.code ;
                return *this ;
            }
        };

    private: // typedefs
            
        typedef so_std::map< so_manager::key_t, code_data > key_to_code_t ;

    private:

        so_thread::mutex_t _mtx ;
        key_to_code_t _key_to_code ;

    private:

        /// no copy allowed.
        code_manager( this_cref_t rhv ) ;

    public:

        code_manager( void_t ) ;
        code_manager( this_rref_t rhv ) ;
        ~code_manager( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual bool_t has_code( so_manager::key_cref_t ) ;

        /// just adds the code segment for the passed key.
        virtual so_manager::result add_code( so_manager::key_cref_t, so_manager::string_cref_t code_in ) ;
            
        /// if the key exists, the code segment is replaced and the listeners are called.
        /// if the key does not exist, the code segment is added.
        virtual so_manager::result replace_code( so_manager::key_cref_t, so_manager::string_cref_t code_in ) ;

    public:

        /// combine code segments referenced by the keys to one single code segment which is
        /// returned in code_out.
        virtual so_manager::result compose( so_manager::key_list_cref_t keys, so_manager::string_ref_t code_out ) ;            

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( code_manager ) ;
}

#endif
