//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "so_import_context.h"

#include <snakeoil/log/log.h>

using namespace so_imex ;

//*************************************************************************************
snakeoil_import_context::snakeoil_import_context( so_imex::iscene_module::import_params_cref_t ip ) : 
    _ip(ip)
{}

//*************************************************************************************
snakeoil_import_context::snakeoil_import_context( this_rref_t rhv )
{
    *this = std::move( std::move(rhv) ) ;
}

//*************************************************************************************
snakeoil_import_context::this_ref_t snakeoil_import_context::operator = ( this_rref_t rhv )
{
    _ip = std::move( rhv._ip ) ;
    _path_to_content = std::move( rhv._path_to_content ) ;
    return *this ;
}

//*************************************************************************************
bool_t snakeoil_import_context::add_content_for_path( so_io::path_cref_t p, so_std::string_cref_t cnt )
{
    so_thread::lock_guard_t lk( _mtx_path_to_content ) ;
    auto iter = _path_to_content.find( p ) ;
    if( iter != _path_to_content.end() )
        return false ;

    _path_to_content[p] = cnt ;

    return true ;
}

//*************************************************************************************
bool_t snakeoil_import_context::get_content_for_path( so_io::path_cref_t p, so_std::string_ref_t cnt_out )
{
    so_thread::lock_guard_t lk( _mtx_path_to_content ) ;
    auto iter = _path_to_content.find( p ) ;
    if( iter == _path_to_content.end() )
        return false ;

    cnt_out = iter->second ;

    return true ;
}

//*************************************************************************************
bool_t snakeoil_import_context::add_document( so_io::path_cref_t p, 
    so_imex::so_jso::document_ptr_t doc_ptr ) 
{
    so_thread::lock_guard_t lk(_mtx_docs) ;
    auto const iter = _docs.find( p ) ;
    if( iter != _docs.end() ) 
        return false ;
   
    _docs[p] = doc_ptr ;

    return true ;
}

//*************************************************************************************
bool_t snakeoil_import_context::get_document( so_io::path_cref_t p, 
    so_imex::so_jso::document_ptr_ref_t doc_out ) 
{
    so_thread::lock_guard_t lk(_mtx_docs) ;
    auto const iter = _docs.find( p ) ;
    if( iter == _docs.end() )
        return false ;

    doc_out = iter->second ;

    return true ;
}

//*************************************************************************************
bool_t snakeoil_import_context::add_code( so_std::string_cref_t id, so_shade::ascii_code_in_t code ) 
{
    so_thread::lock_guard_t lk( _mtx_code ) ;
    auto const iter = _id_to_code.find( id ) ;
    if( iter != _id_to_code.end() )
        return false ;

    _id_to_code[id] = code ;

    return true ;
}

//*************************************************************************************
bool_t snakeoil_import_context::get_code( so_std::string_cref_t id, so_shade::ascii_code_out_t code )
{
    so_thread::lock_guard_t lk( _mtx_code ) ;
    auto const iter = _id_to_code.find( id ) ;
    if( iter != _id_to_code.end() )
        return false ;

    code = _id_to_code[id] ;

    return true ;
}

//*************************************************************************************
snakeoil_import_context::documents_cref_t snakeoil_import_context::get_documents( void_t ) 
{
    so_thread::lock_guard_t lk(_mtx_docs) ;
    return _docs ;
}

//*************************************************************************************
so_imex::manager_registry_ptr_t snakeoil_import_context::manager_reg( void_t ) 
{
    return _ip.manager_reg_ptr ;
}

//*************************************************************************************
so_imex::module_registry_ptr_t snakeoil_import_context::module_reg( void_t ) 
{
    return _ip.module_reg_ptr ;
}
