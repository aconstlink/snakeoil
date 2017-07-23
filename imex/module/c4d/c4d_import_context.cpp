//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "c4d_import_context.h"

using namespace so_imex ;

//**********************************************************************************************
c4d_import_context::c4d_import_context( so_io::path_cref_t p, 
    so_imex::iscene_module::import_params_cref_t ip ) : _doc_path(p), _import_params(ip) 
{

}

//**********************************************************************************************
c4d_import_context::c4d_import_context( this_cref_t rhv )
{
    _unique_polygon = rhv._unique_polygon ;
    _unique_material = rhv._unique_material ;
    _unique_bitmap = rhv._unique_bitmap ;
    _unique_camera = rhv._unique_camera ;

    _mat_to_name = rhv._mat_to_name ;
    _bitmap_to_name = rhv._bitmap_to_name ;
    _polygon_to_name = rhv._polygon_to_name ;
    _camera_to_name = rhv._camera_to_name ;

    _camera_names = rhv._camera_names ;

    _mat_stack = rhv._mat_stack ;
    _doc_path = rhv._doc_path ;
    _import_params = rhv._import_params ;
}

//**********************************************************************************************
c4d_import_context::~c4d_import_context( void_t )
{
}

//**********************************************************************************************
bool_t c4d_import_context::add_polygon( melange::PolygonObject const* pobj, so_std::string_inout_t key ) 
{
    so_thread::lock_guard_t lk( _mtx_polygon ) ;

    auto iter = _polygon_to_name.find( pobj ) ;
    if(iter != _polygon_to_name.end())
        return false ;

    so_std::string const unique_number =
        std::to_string( _unique_polygon++ ) ;

    key = key + "." + unique_number ;

    _polygon_to_name[pobj] = key ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::get_polygon_key( melange::PolygonObject const* pobj, so_std::string_out_t key ) const 
{
    so_thread::lock_guard_t lk( _mtx_polygon ) ;

    auto iter = _polygon_to_name.find( pobj ) ;
    if(iter == _polygon_to_name.end()) return false ;

    key = iter->second ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::add_material( melange::BaseMaterial const * mat_ptr, so_std::string_inout_t key )
{
    so_thread::lock_guard_t lk(_mtx_material) ;

    auto iter = _mat_to_name.find( mat_ptr ) ;
    if( iter != _mat_to_name.end() )
        return false ;

    so_std::string const unique_number = 
        std::to_string( _unique_material++ ) ;

    key = key + "." + unique_number ;

    _mat_to_name[mat_ptr] = key ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::get_material_key( melange::BaseMaterial const * mat_ptr, so_std::string_out_t key ) const
{
    so_thread::lock_guard_t lk(_mtx_material) ;

    auto iter = _mat_to_name.find( mat_ptr ) ;
    if( iter == _mat_to_name.end() ) return false ;

    key = iter->second ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::add_bitmap( melange::BaseShader const * shd_ptr, so_std::string_inout_t key ) 
{
    so_thread::lock_guard_t lk( _mtx_bitmap ) ;

    auto iter = _bitmap_to_name.find( shd_ptr ) ;
    if(iter != _bitmap_to_name.end())
        return false ;

    so_std::string const unique_number = 
        std::to_string( _unique_bitmap++ ) ;

    key = key + "." + unique_number ;

    _bitmap_to_name[shd_ptr] = key ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::get_bitmap_key( melange::BaseShader const * shd_ptr, 
    so_std::string_out_t key ) const
{
    so_thread::lock_guard_t lk( _mtx_bitmap ) ;

    auto iter = _bitmap_to_name.find( shd_ptr ) ;
    if(iter == _bitmap_to_name.end()) return false ;

    key = iter->second ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::change_bitmap_key( melange::BaseShader const * shd_ptr, 
    so_std::string_in_t key ) 
{
    so_thread::lock_guard_t lk( _mtx_bitmap ) ;

    auto iter = _bitmap_to_name.find( shd_ptr ) ;
    if(iter == _bitmap_to_name.end()) return false ;

    iter->second = key ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::remove_bitmap( melange::BaseShader const * shd_ptr ) 
{
    so_thread::lock_guard_t lk( _mtx_bitmap ) ;

    auto iter = _bitmap_to_name.find( shd_ptr ) ;
    if( iter == _bitmap_to_name.end() ) 
        return false ;

    _bitmap_to_name.erase( iter ) ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::add_camera( melange::BaseObject const * obj_ptr, 
    so_std::string_inout_t key ) 
{
    so_thread::lock_guard_t lk( _mtx_camera ) ;

    {
        auto iter = _camera_to_name.find( obj_ptr ) ;
        if( iter != _camera_to_name.end() )
            return false ;
    }

    {
        auto iter = std::find( _camera_names.begin(), _camera_names.end(), key ) ;
        if( iter != _camera_names.end() )
        {
            so_std::string const unique_number =
                std::to_string( _unique_camera++ ) ;

            key = key + "." + unique_number ;
        }
    }

    _camera_to_name[obj_ptr] = key ;

    return true ;
}

//**********************************************************************************************
bool_t c4d_import_context::get_camera_key( melange::BaseObject const * obj_ptr, 
    so_std::string_out_t key ) const 
{
    so_thread::lock_guard_t lk( _mtx_camera ) ;

    auto iter = _camera_to_name.find( obj_ptr ) ;
    if( iter == _camera_to_name.end() ) return false ;

    key = iter->second ;

    return true ;
}

//**********************************************************************************************
void_t c4d_import_context::push_material( melange::BaseMaterial const * mat_ptr ) 
{
    so_thread::lock_guard_t lk( _mtx_mat_stack ) ;
    _mat_stack.push( mat_ptr ) ;
}

//**********************************************************************************************
melange::BaseMaterial const * c4d_import_context::top_material( void_t ) const
{
    so_thread::lock_guard_t lk( _mtx_mat_stack ) ;
    if( so_core::is_not(_mat_stack.has_item()) )
        return nullptr ;

    return _mat_stack.top() ;
}

//**********************************************************************************************
bool_t c4d_import_context::has_top_material( void_t ) const 
{
    so_thread::lock_guard_t lk( _mtx_mat_stack ) ;
    return _mat_stack.has_item() ;
}

//**********************************************************************************************
void_t c4d_import_context::pop_material( void_t )
{
    so_thread::lock_guard_t lk( _mtx_mat_stack ) ;
    _mat_stack.pop() ;
}

//**********************************************************************************************
void_t c4d_import_context::push_visibility( visibility_item_in_t item ) 
{
    _vis_stack.push( item ) ;
}

//**********************************************************************************************
bool_t c4d_import_context::top_visibility( visibility_item_out_t item_out ) 
{
    if( so_core::is_not(_vis_stack.has_item() ) ) 
        return false ;

    item_out = _vis_stack.top() ;

    return true ;
}

//**********************************************************************************************
void_t c4d_import_context::pop_visibility( void_t ) 
{
    _vis_stack.pop() ;
}

//**********************************************************************************************
so_io::path_cref_t c4d_import_context::doc_path( void_t ) const 
{
    return _doc_path ;
}

//**********************************************************************************************
so_imex::iscene_module::import_params_cref_t c4d_import_context::import_params( void_t ) const 
{
    return _import_params ;
}
