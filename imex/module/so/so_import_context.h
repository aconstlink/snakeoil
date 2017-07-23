//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_SNAKEOIL_SNAKEOIL_IMPORT_CONTEXT_H_
#define _SNAKEOIL_IMEX_MODULE_SNAKEOIL_SNAKEOIL_IMPORT_CONTEXT_H_

#include "../../typedefs.h"
#include "../../api.h"
#include "../../protos.h"

#include "../iscene_module.h"
#include "so_document.h"

#include <snakeoil/shade/code/ascii_code.h>

#include <snakeoil/io/typedefs.h>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/container/stack.hpp>
#include <snakeoil/std/container/vector.hpp>

namespace so_imex
{
    class snakeoil_import_context
    {
        so_this_typedefs( snakeoil_import_context ) ;

        typedef so_std::map< so_io::path_t, so_std::string_t > __path_to_content_t ;
        so_typedefs( __path_to_content_t, path_to_content ) ;

        typedef so_std::map< so_io::path_t, so_imex::so_jso::document_ptr_t > __documents_t ;
        so_typedefs( __documents_t, documents ) ;

        typedef so_std::map< so_std::string_t, so_shade::ascii_code_t > __id_to_code_t ;
        so_typedefs( __id_to_code_t, id_to_code ) ;

    private: 

        so_thread::mutex_t _mtx_path_to_content ;
        path_to_content_t _path_to_content ;

        so_imex::iscene_module::import_params_t _ip ;

        so_thread::mutex_t _mtx_code ;
        id_to_code_t _id_to_code ;

    private:

        so_thread::mutex_t _mtx_docs ;
        documents_t _docs ;

    private:

        snakeoil_import_context( this_cref_t ) = delete ;
        this_rref_t operator = ( this_cref_t ) = delete ;

    public:

        snakeoil_import_context( so_imex::iscene_module::import_params_cref_t ) ;
        snakeoil_import_context( this_rref_t ) ;
        this_ref_t operator = ( this_rref_t ) ;

    public:

        bool_t add_content_for_path( so_io::path_cref_t, so_std::string_cref_t ) ;
        bool_t get_content_for_path( so_io::path_cref_t, so_std::string_ref_t ) ;

    public:

        bool_t add_document( so_io::path_cref_t, so_imex::so_jso::document_ptr_t ) ;
        bool_t get_document( so_io::path_cref_t, so_imex::so_jso::document_ptr_ref_t ) ;

        documents_cref_t get_documents( void_t ) ;

    public:

        bool_t add_code( so_std::string_cref_t, so_shade::ascii_code_in_t ) ;
        bool_t get_code( so_std::string_cref_t, so_shade::ascii_code_out_t ) ;

    public:

        so_imex::manager_registry_ptr_t manager_reg( void_t ) ;
        so_imex::module_registry_ptr_t module_reg( void_t ) ;

    } ;
    so_typedef( snakeoil_import_context ) ;
}

#endif
