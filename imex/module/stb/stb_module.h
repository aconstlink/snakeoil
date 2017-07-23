//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_STB_STB_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_STB_STB_MODULE_H_

#include "../iimage_module.h"
#include <snakeoil/io/protos.h>

namespace so_imex
{
    class SNAKEOIL_IMEX_API stb_module : public iimage_module
    {
        so_this_typedefs( stb_module ) ;
        
    public:

        stb_module( void_t ) ;
        stb_module( this_rref_t ) ;
        virtual ~stb_module( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public: // interface

        virtual so_imex::result initialize( init_params_cref_t ) ;
        virtual so_imex::result destroy( void_t ) ;

        virtual bool_t create_properties( property_map_inout_t ) const ;

        virtual bool_t is_format_supported( so_std::string_cref_t ) const ;
        virtual bool_t is_format_supported( so_imex::image_file_format ) const ;
        virtual so_imex::file_extension_names_t get_file_extension_names( void_t ) const ;

    public: // image interface
        
        virtual so_thread::task_graph_t import_image( import_params_cref_t ) ;
        virtual so_thread::task_graph_t export_image( export_params_cref_t ) ;
        
    private:

        static bool_t flip_vertically( byte_ptr_t out_ptr, byte_ptr_t const in_ptr, 
            size_t const width, size_t const height, size_t const comp ) ;
    };
    so_typedef( stb_module ) ;
}

#endif
