//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "c4d_module.h"

#include "../../manager/graph_manager.h"
#include "../../manager/mesh_manager.h"
#include "../../manager/shade_manager/material_manager.h"
#include "../../manager/manager_registry.h"

#include "../../node/mesh/mesh_asset.h"
#include "../../node/image/image_asset.h"
#include "../../node/decorator/imported_from.h"
#include "../../node/decorator/transform_3d.h"
#include "../../node/group/logic_group.h"
#include "../../node/material/material_asset.h"
#include "../../node/mesh/mesh.h"
#include "../../node/camera/camera_asset.h"

#include "../../camera/generic_camera.h"
#include "../../camera/pinhole_lens.h"

#include <snakeoil/imex/module/iimage_module.h>
#include <snakeoil/imex/module/module_registry.h>

#include <snakeoil/shade/material/standard_material.h>

#include <snakeoil/std/filesystem/filesystem.hpp>

#include <snakeoil/geometry/mesh/polygon_mesh.h>
#include <snakeoil/thread/task/task_graph.h>
#include <snakeoil/thread/task/tasks.h>

#include <snakeoil/math/utility/angle.hpp>
#include <snakeoil/math/euler/euler_angles.hpp>

#include <snakeoil/log/log.h>
#include <snakeoil/core/break_if.hpp>

#include <c4d_all.h>
#include <c4d_memory.h>

#include <algorithm>

using namespace so_imex ;

//**********************************************************************************************
so_thread::task_graph_t c4d_module::import_scene( import_params_cref_t params_in )
{
    so_imex::sync_object_t::set_not_signal( params_in.sync_ptr, so_imex::not_ready ) ;

    so_thread::itask_ptr_t tt_end = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        so_imex::sync_object_t::set_and_signal( params_in.sync_ptr, so_imex::ok ) ;
    }) ;

    so_thread::itask_ptr_t tt_begin = so_thread::dyn_task_t::create( 
        [=]( so_thread::itask_ptr_t self_ptr )
    {
        so_thread::task_graph_t tg_outer( self_ptr, tt_end ) ;

        melange::Filename load_from( params_in.path_to_file.string().c_str() ) ;
        melange::BaseDocument * c4d_ptr = melange::LoadDocument( 
            load_from, melange::SCENEFILTER_OBJECTS | melange::SCENEFILTER_MATERIALS ) ;

        so_io::path_t doc_path = params_in.path_to_file ;
        doc_path.remove_filename( ) ;
        //so_io::path_helper::path_without_filename( params_in.path_to_file, doc_path ) ;

        melange::BaseObject * root_ptr = c4d_ptr->GetFirstObject() ;

        c4d_import_context_t ctx( doc_path, params_in ) ;
        
        /// all_images must be done before all_materials due to unique image name
        this->import_all_images( params_in.key, root_ptr, params_in, doc_path, tg_outer, ctx ) ;
        this->import_all_materials( params_in.key, root_ptr, params_in, doc_path, tg_outer, ctx ) ;

        this->import_all_meshes( params_in.key, root_ptr, params_in, tg_outer, ctx ) ;
        
        this->import_all_splines( params_in.key, root_ptr, ctx, tg_outer ) ;
        this->import_all_cameras( params_in.key, root_ptr, ctx ) ;

        this->import_scene( params_in.key, root_ptr, params_in,doc_path, tg_outer, ctx ) ;

        so_thread::task_graph_t::dont_destroy( std::move(tg_outer) ) ;

        melange::BaseDocument::Free( c4d_ptr ) ;
    } ) ;

    return so_thread::task_graph_t(tt_begin, tt_end) ;
}

//**********************************************************************************************
void_t c4d_module::import_scene( so_std::string_cref_t prev_name, 
    melange::BaseObject * cur_object , so_imex::iscene_module::import_params_cref_t params_in, 
    so_io::path_cref_t doc_path,
    so_thread::task_graph_ref_t tg_outer, c4d_import_context_ref_t ctx )
{
    if( so_core::is_nullptr(cur_object) )
        return ;

    so_imex::so_node::logic_group_ptr_t root_ptr =
        so_imex::so_node::logic_group::create(
        "[so_imex::fbx_module::import_the_graph] : logic_group for c4d root" ) ;

    this_t::import_the_graph( prev_name, cur_object, root_ptr, params_in, doc_path, ctx ) ;

    // add root node to manager
    {
        so_imex::graph_manager_t::manage_params mp ;
        mp.data_ptr = root_ptr ;
        auto res = params_in.manager_reg_ptr->get_graph_manager()->insert( prev_name, mp ) ;
        if( so_imex::no_success(res) )
        {
            so_log::log::error("[so_imex::c4d_module::import_scene] : unable to manage graph " +
                prev_name ) ;
            root_ptr->destroy() ;
        }
    }
}

//**********************************************************************************************
void_t c4d_module::import_the_graph( so_std::string_cref_t prev_name, melange::BaseObject * cur_object,
    so_imex::so_node::logic_group_ptr_t parent_ptr, 
    so_imex::iscene_module::import_params_cref_t params_in, 
    so_io::path_cref_t doc_path, c4d_import_context_ref_t ctx ) 
{
    if( so_core::is_nullptr( cur_object ) )
        return ;

    do
    {
        {
            melange::GeData data ;
            if( cur_object->GetParameter( melange::ID_BASEOBJECT_VISIBILITY_EDITOR, data ) )
            {
                int32_t const v = data.GetInt32() ;
                switch( v )
                {
                case melange::OBJECT_ON: break ;
                case melange::OBJECT_OFF: break ;
                case melange::OBJECT_UNDEF: break ;
                }
            }
        }

        {
            melange::GeData data ;
            if( cur_object->GetParameter( melange::ID_BASEOBJECT_VISIBILITY_RENDER, data ) )
            {
                int32_t const v = data.GetInt32() ;
                switch( v )
                {
                case melange::OBJECT_ON: break ;
                case melange::OBJECT_OFF: break ;
                case melange::OBJECT_UNDEF: break ;
                }
            }
        }

        so_std::string_t const cur_name = prev_name + "." +
            this_t::to_string( cur_object->GetName() ) ;

        so_imex::so_node::logic_group_ptr_t new_parent_ptr =
            so_imex::so_node::logic_group::create(
            "[so_imex::fbx_module::import_the_graph] : logic_group for " + cur_name ) ;

        bool_t mat_pushed = false ;
        {
            melange::BaseTag * btag = cur_object->GetFirstTag() ;
            while( so_core::is_not_nullptr(btag) )
            {
                if( btag->GetType() == Ttexture )
                {
                    melange::TextureTag * ttag = (melange::TextureTag*) btag ;
                    melange::BaseMaterial * mat_ptr = ttag->GetMaterial() ;
                    if(so_core::is_not_nullptr( mat_ptr ))
                    {
                        ctx.push_material( mat_ptr ) ;
                        mat_pushed = true ;
                    }
                }
                btag = btag->GetNext() ;
            }
        }

        //
        {
            melange::Int32 const type = cur_object->GetType() ;

            switch( type )
            {
            case Ocamera:
            this_t::import_the_camera_graph( cur_object, new_parent_ptr, ctx ) ;
            break ;
            case Ospline:
            // spline node
            break ;
            case Opolygon:
            this_t::import_the_polygon_graph(cur_name, (melange::PolygonObject*)cur_object, 
                new_parent_ptr, params_in, doc_path, ctx ) ;
            break ;
            }

            this_t::import_the_graph( cur_name, cur_object->GetDown(), 
                new_parent_ptr, params_in, doc_path, ctx ) ;
        }

        if( mat_pushed )
        {
            ctx.pop_material() ;
            mat_pushed = false ;
        }

        so_imex::so_node::node_ptr_t to_be_added = new_parent_ptr ;

        // create transformation
        {
            // decorate the last decorated by this transformation
            auto * trafo_nptr = so_imex::so_node::transform_3d_t::create(
                so_imex::so_node::transform_3d_t( to_be_added ),
                "[so_imex::fbx_module::import_the_grap] : transform_3d" ) ;

            so_math::vec3f_t translation = this_t::to_vector( cur_object->GetRelPos() ) ;
            so_math::vec3f_t rotation = this_t::to_vector( cur_object->GetRelRot() ) ;
            so_math::vec3f_t scale = this_t::to_vector( cur_object->GetRelScale() ) ;

            translation.z() = -translation.z() ;
            rotation.z() = -rotation.z() ;

            // transform by separate local transformation
#if 0
            {
                so_math::so_3d::trafof_t tscale ;
                tscale.scale_fl( scale ) ;

                so_math::vec3f_t const x = so_math::vec3f_t( so_math::so_vector::x_axis() ) ;
                so_math::vec3f_t const y = so_math::vec3f_t( so_math::so_vector::y_axis() ) ;
                so_math::vec3f_t const z = so_math::vec3f_t( so_math::so_vector::z_axis() ) ;

                so_math::so_3d::trafof_t trotate ;  
                
                trotate.rotate_by_axis_fl( z, rotation.z() ) ;
                trotate.rotate_by_axis_fl( x, rotation.y() ) ;
                trotate.rotate_by_axis_fl( y, rotation.x() ) ;
                

                so_math::so_3d::trafof_t ttranslate ;
                ttranslate.translate_fl( translation ) ;

                so_math::so_3d::trafof_t const trafo =   ttranslate * trotate * tscale;

                trafo_nptr->set_transformation( trafo ) ;
            }
#endif

            {
                // cinema 4d rotation is: heading, pitch, bank (HPB) <=> y,x,z
                // we need to convert the angles to XYZ, which is the euler rotation
                // sequenced used by the engine.
                typedef so_math::euler_angles<float_t> euler_angles_t ;
                euler_angles_t ea( so_math::euler_sequence::yxz, rotation ) ;

                auto const angles = ea.convert_to( so_math::euler_sequence::xyz ).get_angles() ;
                so_math::vec3f_t rot_angles( angles.x().as_radians(), 
                    angles.y().as_radians(), angles.z().as_radians() ) ;

                trafo_nptr->transform( rot_angles, scale, translation ) ;
            }

            to_be_added = trafo_nptr ;
        }

        parent_ptr->add_child( to_be_added ) ;

        cur_object = cur_object->GetNext() ;

    } while( so_core::is_not_nullptr( cur_object ) );

}

//**********************************************************************************************
void_t c4d_module::import_the_polygon_graph( so_std::string_cref_t this_name, 
    melange::PolygonObject * cur_object,
    so_imex::so_node::logic_group_ptr_t parent_ptr,
    so_imex::iscene_module::import_params_cref_t params_in, 
    so_io::path_cref_t doc_path, c4d_import_context_cref_t ctx ) 
{
    so_imex::so_node::mesh_ptr_t mesh_ptr = so_imex::so_node::mesh_t::create(
        "[so_imex::c4d_module::import_the_polygon_graph] : mesh_node : " + this_name ) ;

    // lets look for a material
    /*{
        melange::BaseTag * btag_ptr = cur_object->GetFirstTag() ;
        while( so_core::is_not_nullptr(btag_ptr) )
        {

            if( btag_ptr->GetType() == Ttexture )
            {
                melange::TextureTag * ttag_ptr = (melange::TextureTag *)btag_ptr ;
                melange::BaseMaterial * mat_ptr = ttag_ptr->GetMaterial() ;

                
                auto iter = mat_to_name.find( mat_ptr ) ;
                if( iter == mat_to_name.end() )
                {
                    so_log::log::status("[************] : material not in map") ;
                }
            }

            btag_ptr = btag_ptr->GetNext() ;
        }
    }*/

    // assign mesh asset
    {
        so_imex::mesh_manager_t::handle_t mhnd ;

        so_imex::key_t geo_key ;
        if( ctx.get_polygon_key( cur_object, geo_key ) )
        {
            auto const res = params_in.manager_reg_ptr->get_mesh_manager()->acquire( geo_key,
                "[so_imex::c4d_module::import_the_polygon_graph]", mhnd ) ;

            so_log::log::error( so_core::is_not( res ),
                "[so_imex::c4d_module::import_the_polygon_graph] : mesh does not exist : " + geo_key ) ;

            if(res)
            {
                so_imex::so_node::mesh_asset_ptr_t mesh_asset_ptr =
                    so_imex::so_node::mesh_asset_t::create(
                    so_imex::so_node::mesh_asset_t( std::move( mhnd ) ),
                    "[so_imex::c4d_module::import_the_polygon_graph] : mesh_asset" ) ;

                mesh_ptr->set_asset( mesh_asset_ptr ) ;
            }
        }
    }

    // create material and material asset node
    {
        so_imex::material_manager_t::handle_t hnd ;

        so_imex::key_t mat_key ;
        if(  ctx.get_material_key( ctx.top_material(), mat_key ) ) 
        {
            if( so_core::is_not( params_in.manager_reg_ptr->get_material_manager()->acquire( mat_key, 
                "[c4d_module::import_the_polygon_graph] : mesh node", hnd ) ) )
            {
                so_log::log::error(so_std::string_t(
                    "[so_imex::so_imex::c4d_module::import_the_polygon_graph] : " 
                    "unable to acquire material : " ) + mat_key ) ;
            }
        }
        else
        {
            // there is no material, so use standard material
            if( so_core::is_not(params_in.manager_reg_ptr->get_material_manager()->acquire( "c4d.standard",
                "[c4d_module::import_the_polygon_graph] : mesh node", hnd )) )
            {
                so_shade::so_material::standard_material_t std_mat ;
                
                // fill standard material
                {
                    std_mat.add_color_layer( so_math::vec3f_t(1.0f,0.0f,0.0f) ) ;
                }

                // insert standard material
                {
                    so_imex::material_manager_t::manage_params mp ;
                    mp.data_ptr = so_shade::so_material::standard_material_t::create( std::move(std_mat),
                        "[c4d_module::import_the_polygon_graph] : standard material") ;
                    params_in.manager_reg_ptr->get_material_manager()->insert( "c4d.standard", mp ) ;
                }

                auto const ac_res = params_in.manager_reg_ptr->get_material_manager()->acquire( 
                    "c4d.standard", "[c4d_module::import_the_polygon_graph] : mesh node", hnd ) ;
                so_log::log::error( so_core::is_not(ac_res), "[c4d_module::import_the_polygon_graph] : "
                    "can not acquire inserted c4d fallback standard material") ;
            }
        }

        so_imex::so_node::material_asset_ptr_t material_ptr =
            so_imex::so_node::material_asset_t::create( 
            so_imex::so_node::material_asset_t(std::move(hnd)),
            "[so_imex::so_imex::c4d_module::import_the_polygon_graph] : material_asset" ) ;


        mesh_ptr->set_material( material_ptr ) ;
    }

    // images
    if( ctx.has_top_material() )
    {
        melange::BaseMaterial * mel_mat_ptr = 
            const_cast<melange::BaseMaterial*>(ctx.top_material()) ;        
        
        melange::BaseShader const * mel_shd_ptr = mel_mat_ptr->GetFirstShader() ;

        so_std::string_t key ;
        if( ctx.get_bitmap_key( mel_shd_ptr, key ) )
        {
            so_imex::image_manager_ptr_t img_mgr_ptr =
                params_in.manager_reg_ptr->get_image_manager() ;

            so_imex::image_manager_t::handle_t hnd ;
            if( img_mgr_ptr->acquire( key,
                "[so_imex::c4d_module::import_the_polygon_graph]", hnd ) )
            {
                auto ikey = hnd.get_key() ;

                so_imex::so_node::image_asset_ptr_t img_ass_ptr =
                    so_imex::so_node::image_asset_t::create(
                    so_imex::so_node::image_asset_t( std::move( hnd ) ),
                    "[so_imex::c4d_module::import_the_polygon_graph] : image_asset" ) ;

                if( so_core::is_not(mesh_ptr->add_image( ikey, img_ass_ptr ) ) )
                {
                    so_log::log::error( "[c4d_module::import_the_polygon_graph] : "
                        "can not add image : " + ikey ) ;
                }
            }
            else
            {
                so_log::log::error_and_exit( "[c4d_module::import_the_polygon_graph] : "
                    "programming error" ) ;
            }
        }
    }

    parent_ptr->add_child( mesh_ptr ) ;
}

//**********************************************************************************************
void_t c4d_module::import_the_camera_graph( melange::BaseObject const * mobj_ptr,
    so_imex::so_node::logic_group_ptr_t parent_ptr, c4d_import_context_cref_t ctx ) 
{
    so_std::string_t key ;
    if( so_core::is_not( ctx.get_camera_key( mobj_ptr, key ) ) )
    {
        so_log::log::error( "[so_imex::c4d_module::import_the_camera_graph] : "
            "can not find key in context " ) ;
        return ;
    }

    so_imex::camera_manager_ptr_t cam_mgr_ptr = ctx.import_params().manager_reg_ptr->
        get_camera_manager() ;

    so_imex::camera_manager_t::handle_t hnd ;
    auto const res = cam_mgr_ptr->acquire( key, 
        "[so_imex::c4d_module::import_the_camera_graph]", hnd ) ;
    if( so_core::is_not(res) )
    {
        so_log::log::error( "[so_imex::c4d_module::import_the_camera_graph] : "
            "failed to acquire camera" ) ;
        return ;
    }

    // 1. create camera node
    so_imex::so_node::camera_asset_ptr_t cam_n_ptr = 
        so_imex::so_node::camera_asset_t::create(
        so_imex::so_node::camera_asset_t(std::move(hnd)), 
        "[so_imex::c4d_module::import_the_camera_graph] : camera_asset" ) ;

    // 2. create transformation for camera
    // @note c4d uses left-handed coord system. In order to have the
    // camera facing the same direction, it needs to be rotated by 180deg
    // about the y axis.
    so_math::so_3d::trafof_t trafo = 
        so_math::so_3d::trafof_t::scaling( so_math::vec3f_t(-1.0f,1.0f,-1.0f) ) ;

    so_imex::so_node::transform_3d_ptr_t trafo_node_ptr = 
        so_imex::so_node::transform_3d_t::create(
        so_imex::so_node::transform_3d_t(trafo), 
        "[so_imex::c4d_module::import_the_camera_graph] : camera transformation" ) ;


    // 3. attach camera node
    trafo_node_ptr->set_decorated(cam_n_ptr) ;
    parent_ptr->add_child( trafo_node_ptr ) ;
}

//**********************************************************************************************
void_t c4d_module::import_all_meshes( so_std::string_cref_t prev_name, melange::BaseObject * node_ptr, 
    so_imex::iscene_module::import_params_cref_t params_in, so_thread::task_graph_ref_t tg_outer,
    c4d_import_context_ref_t ctx)
{
    if( so_core::is_nullptr(node_ptr) )
        return ;

    do 
    {
        so_std::string_t const cur_name = prev_name + "." + 
            this_t::to_string( node_ptr->GetName() ) ;

        auto const type = node_ptr->GetType();

        if( type == melange::OBJECT_POLYGON )
        {
            melange::PolygonObject * geo_ptr = ( melange::PolygonObject* ) node_ptr ;
            
            so_std::string_t key = this_t::to_string( node_ptr->GetName() ) ;
            if( ctx.add_polygon( geo_ptr, key ) )
            {
                this_t::import_mesh( key, geo_ptr, params_in, tg_outer, ctx ) ;
            }
        }
        
        this_t::import_all_meshes( cur_name, node_ptr->GetDown(), params_in, tg_outer, ctx );

        node_ptr = node_ptr->GetNext() ;

    } while( so_core::is_not_nullptr(node_ptr) ) ;
}

//**********************************************************************************************
void_t c4d_module::import_mesh( so_std::string_cref_t key, melange::PolygonObject * geo_ptr,
            so_imex::iscene_module::import_params_cref_t params_in,
            so_thread::task_graph_ref_t tg_outer, c4d_import_context_ref_t ctx) 
{
    so_geo::polygon_mesh_t poly_mesh ;

    melange::Int32 const num_polys = geo_ptr->GetPolygonCount();
    melange::CPolygon const * polys = geo_ptr->GetPolygonR();

    // make polygons
    {
        poly_mesh.polygons.resize( num_polys ) ;
        for(melange::Int32 i = 0; i < num_polys; ++i)
        {
            melange::Int32 const c = polys[i].c ;
            melange::Int32 const d = polys[i].d ;

            poly_mesh.polygons[i] = c != d ? 4 : 3 ;
        }
    }

    // count indices
    size_t num_indices = 0;
    for(uint_t np : poly_mesh.polygons)
    {
        num_indices += np;
    }

    // make indices
    {
        poly_mesh.indices.resize( num_indices );

        size_t cur_index = 0;

        for(melange::Int32 i = 0; i < num_polys; ++i)
        {
            melange::Int32 const a = polys[i].a ;
            melange::Int32 const b = polys[i].b ;
            melange::Int32 const c = polys[i].c ;
            melange::Int32 const d = polys[i].d ;

            poly_mesh.indices[cur_index++] = a ;
            poly_mesh.indices[cur_index++] = b ;
            poly_mesh.indices[cur_index++] = c ;

            if(c != d)
                poly_mesh.indices[cur_index++] = d ;

        }

        so_log::log::error_and_exit( cur_index != num_indices,
            "[so_imex::c4d_module::import_all_meshes] : num indices incorrect" );
    }

    // positions
    {
        poly_mesh.position_format = so_geo::vector_component_format::xyz ;

        melange::Int32 const num_points = geo_ptr->GetPointCount();
        melange::Vector const * points = geo_ptr->GetPointR();

        poly_mesh.positions.resize( num_points * 3 );
        for(melange::Int32 i = 0; i < num_points; ++i)
        {
            poly_mesh.positions[i * 3 + 0] = points[i].x;
            poly_mesh.positions[i * 3 + 1] = points[i].y;
            poly_mesh.positions[i * 3 + 2] = -points[i].z;
        }
    }

    // normals
    {
        poly_mesh.normal_format = so_geo::vector_component_format::xyz ;

        melange::NormalTag * nrm_ptr = (melange::NormalTag *)geo_ptr->GetTag( Tnormal ) ;

        if(so_core::is_not_nullptr( nrm_ptr ))
        {
            melange::NormalStruct ns ;
            melange::ConstNormalHandle nrm_handle = nrm_ptr->GetDataAddressR() ;

            // seems to be the polygon count
            //melange::Int32 const num_normals = nrm_ptr->GetDataCount() ;

            poly_mesh.normals.resize( 1 ) ;
            poly_mesh.normals_indices.resize( 1 ) ;

            poly_mesh.normals[0].resize( num_indices * 3 ) ;
            poly_mesh.normals_indices[0].resize( num_indices ) ;

            uint_t index = 0 ;
            uint_t nindex = 0 ;

            for(melange::Int32 i = 0; i < num_polys; ++i)
            {
                size_t const np = poly_mesh.polygons[i] ;
                melange::NormalTag::Get( nrm_handle, i, ns ) ;

                // triangle
                if(np <= 4)
                {
                    poly_mesh.normals_indices[0][index + 0] = index + 0 ;
                    poly_mesh.normals_indices[0][index + 1] = index + 1 ;
                    poly_mesh.normals_indices[0][index + 2] = index + 2 ;

                    poly_mesh.normals[0][nindex + 0] = ns.a.x ;
                    poly_mesh.normals[0][nindex + 1] = ns.a.y ;
                    poly_mesh.normals[0][nindex + 2] = ns.a.z ;

                    poly_mesh.normals[0][nindex + 3] = ns.b.x;
                    poly_mesh.normals[0][nindex + 4] = ns.b.y;
                    poly_mesh.normals[0][nindex + 5] = ns.b.z;

                    poly_mesh.normals[0][nindex + 6] = ns.c.x;
                    poly_mesh.normals[0][nindex + 7] = ns.c.y;
                    poly_mesh.normals[0][nindex + 8] = ns.c.z;

                    nindex += 9 ;
                    index += 3 ;
                }

                // quad
                if(np == 4)
                {
                    poly_mesh.normals_indices[0][index + 0] = index + 0 ;

                    poly_mesh.normals[0][nindex + 0] = ns.d.x;
                    poly_mesh.normals[0][nindex + 1] = ns.d.y;
                    poly_mesh.normals[0][nindex + 2] = ns.d.z;

                    nindex += 3 ;
                    index += 1 ;
                }
            }
        }
        // by phong
        else
        {
            melange::NormalStruct ns ;
            melange::Vector32 * normals_ptr = geo_ptr->CreatePhongNormals() ;

            if(so_core::is_not_nullptr( normals_ptr ))
            {
                poly_mesh.normals.resize( 1 );
                poly_mesh.normals_indices.resize( 1 );

                poly_mesh.normals[0].resize( num_indices * 3 );
                poly_mesh.normals_indices[0].resize( num_indices );

                uint_t index = 0;
                uint_t nindex = 0;

                for(melange::Int32 i = 0; i < num_polys; ++i)
                {
                    size_t const np = poly_mesh.polygons[i];

                    auto const & a = normals_ptr[i * 4 + 0] ;
                    auto const & b = normals_ptr[i * 4 + 1] ;
                    auto const & c = normals_ptr[i * 4 + 2] ;
                    auto const & d = normals_ptr[i * 4 + 3] ;

                    // triangle
                    if(np <= 4)
                    {
                        poly_mesh.normals_indices[0][index + 0] = index + 0;
                        poly_mesh.normals_indices[0][index + 1] = index + 1;
                        poly_mesh.normals_indices[0][index + 2] = index + 2;

                        poly_mesh.normals[0][nindex + 0] = a.x;
                        poly_mesh.normals[0][nindex + 1] = a.y;
                        poly_mesh.normals[0][nindex + 2] = a.z;

                        poly_mesh.normals[0][nindex + 3] = b.x;
                        poly_mesh.normals[0][nindex + 4] = b.y;
                        poly_mesh.normals[0][nindex + 5] = b.z;

                        poly_mesh.normals[0][nindex + 6] = c.x;
                        poly_mesh.normals[0][nindex + 7] = c.y;
                        poly_mesh.normals[0][nindex + 8] = c.z;

                        nindex += 9;
                        index += 3;
                    }

                    // quad
                    if(np == 4)
                    {
                        poly_mesh.normals_indices[0][index + 0] = index + 0;

                        poly_mesh.normals[0][nindex + 0] = d.x;
                        poly_mesh.normals[0][nindex + 1] = d.y;
                        poly_mesh.normals[0][nindex + 2] = d.z;

                        nindex += 3;
                        index += 1;
                    }
                }

                melange::DeleteMem( normals_ptr ) ;
            }
        }
    }

    // texcoords
    // cycle through all uvw tags
    {
        auto * tag_ptr = geo_ptr->GetTag( Tuvw ) ;
        if(so_core::is_not_nullptr( tag_ptr ))
        {
            poly_mesh.texcoord_format = so_geo::texcoord_component_format::uvw ;
        }

        while(so_core::is_not_nullptr( tag_ptr ))
        {
            if(tag_ptr->GetType() != Tuvw)
            {
                tag_ptr = tag_ptr->GetNext() ;
                continue ;
            }

            uint_t index = 0;
            uint_t nindex = 0;

            melange::UVWTag * uvw_tag = (melange::UVWTag*)tag_ptr ;
            {
                so_geo::uints_t indices( num_indices ) ;
                so_geo::floats_t texcoords( num_indices * 3 ) ;

                melange::UVWStruct uvw ;
                melange::ConstUVWHandle * uvw_data =
                    (melange::ConstUVWHandle*)uvw_tag->GetDataAddressR() ;

                for(melange::Int32 i = 0; i < num_polys; ++i)
                {
                    uint_t const np = poly_mesh.polygons[i] ;
                    melange::UVWTag::Get( uvw_data, i, uvw ) ;

                    // triangle
                    if(np <= 4)
                    {
                        indices[index + 0] = index + 0;
                        indices[index + 1] = index + 1;
                        indices[index + 2] = index + 2;

                        texcoords[nindex + 0] = uvw.a.x ;
                        texcoords[nindex + 1] = 1.0f - uvw.a.y;
                        texcoords[nindex + 2] = uvw.a.z;

                        texcoords[nindex + 3] = uvw.b.x ;
                        texcoords[nindex + 4] = 1.0f - uvw.b.y;
                        texcoords[nindex + 5] = uvw.b.z;

                        texcoords[nindex + 6] = uvw.c.x;
                        texcoords[nindex + 7] = 1.0f - uvw.c.y;
                        texcoords[nindex + 8] = uvw.c.z;

                        nindex += 9;
                        index += 3;
                    }

                    // quad
                    if(np == 4)
                    {
                        indices[index + 0] = index + 0;

                        texcoords[nindex + 0] = uvw.d.x;
                        texcoords[nindex + 1] = 1.0f - uvw.d.y;
                        texcoords[nindex + 2] = uvw.d.z;

                        nindex += 3;
                        index += 1;
                    }
                }

                poly_mesh.texcoords_indices.push_back( std::move( indices ) ) ;
                poly_mesh.texcoords.push_back( std::move( texcoords ) ) ;
            }

            tag_ptr = tag_ptr->GetNext() ;
        }
    }

    {
        auto * mesh_mgr_ptr = params_in.manager_reg_ptr->get_mesh_manager();

        so_imex::mesh_manager::manage_params mp;
        mp.data_ptr = so_geo::polygon_mesh::create( std::move( poly_mesh ),
            "[so_imex::c4d_module::import_all_meshes] : polygon_mesh" );

        auto const res = mesh_mgr_ptr->insert( key, mp );
        if(so_imex::no_success( res ))
        {
            so_log::log::error(
                "[so_imex::c4d_module::import_all_meshes] : insert polygon mesh" );

            mp.data_ptr->destroy();
        }
    }
}

//**********************************************************************************************
void_t c4d_module::import_all_materials( so_std::string_cref_t prev_name, melange::BaseObject * mel_obj_ptr,
    so_imex::iscene_module::import_params_cref_t params_in, so_io::path_cref_t doc_path, 
    so_thread::task_graph_ref_t tg_outer, c4d_import_context_ref_t ctx ) 
{
    if( so_core::is_nullptr( mel_obj_ptr ) )
        return ;

    do
    {
        size_t max_tex_layers = 0 ;

        so_std::string_t const cur_name = prev_name + "." +
            this_t::to_string( mel_obj_ptr->GetName() ) ;

        //auto const type = mel_obj_ptr->GetType();

        
        {
            melange::BaseTag * cur_tag = mel_obj_ptr->GetFirstTag() ;
            while(so_core::is_not_nullptr( cur_tag ))
            {
                if(cur_tag->GetType() == Tuvw)
                {
                    max_tex_layers++;
                }

                cur_tag = cur_tag->GetNext() ;
            }
        }        

        melange::BaseTag * cur_tag = mel_obj_ptr->GetFirstTag() ;
        while( so_core::is_not_nullptr(cur_tag) )
        {
            if( cur_tag->GetType() == Ttexture )
            {
                melange::TextureTag * tx_tag = (melange::TextureTag*)cur_tag ;
                melange::BaseMaterial * mel_mat_ptr = tx_tag->GetMaterial() ;

                so_std::string_t mat_key ;

                // test is material already exists
                {
                    using namespace melange ;

                    char * cname = mel_mat_ptr->GetName().GetCStringCopy() ;
                    mat_key = so_std::string_t( cname ) ;
                    DeleteMem( cname ) ;

                    if( ctx.add_material(mel_mat_ptr, mat_key) )
                    {
                        this_t::import_material( cur_name, tx_tag, mel_mat_ptr, params_in, doc_path, ctx ) ;
                    }
                }
            }            

            cur_tag = cur_tag->GetNext() ;
        }
        

        this_t::import_all_materials( cur_name, mel_obj_ptr->GetDown(), params_in, doc_path, tg_outer, ctx );

        mel_obj_ptr = mel_obj_ptr->GetNext() ;

    } while( so_core::is_not_nullptr(mel_obj_ptr) ) ;
}

//**********************************************************************************************
void_t c4d_module::import_material( so_std::string_cref_t cur_name, 
        melange::TextureTag * tx_tag, melange::BaseMaterial * mel_mat_ptr,
            so_imex::iscene_module::import_params_cref_t params_in, so_io::path_cref_t doc_path,
            c4d_import_context_ref_t ctx ) 
{
    so_shade::so_material::standard_material_ptr_t mat_ptr =
        so_shade::so_material::standard_material_t::create(
        "[c4d_module::import_all_materials] : so_shade standard_material" ) ;

    // normal
    {
        mat_ptr->add_normal_layer() ;
    }

    size_t max_textures = 0 ;

    // check if texture layers are required
    {
        using namespace melange ;

        melange::BaseShader * cur_shd_ptr = mel_mat_ptr->GetFirstShader() ;

        while(so_core::is_not_nullptr( cur_shd_ptr ))
        {
            if( cur_shd_ptr->GetType() == Xbitmap ) max_textures ++ ;
            cur_shd_ptr = cur_shd_ptr->GetNext() ;
        }        
    }

    // texture layer
    if(max_textures > 0)
    {
        // Rule:
        // c4d : 100% <=> 1.0
        // scale : vec2(1.0 / length)
        // offset : * vec2(-1.0, 1.0)
        so_math::vec2f_t uv_offset ;
        so_math::vec2f_t uv_scale ;

        // offset u
        {
            melange::GeData data ;
            if(tx_tag->GetParameter( melange::TEXTURETAG_OFFSETX, data ))
            {
                uv_offset.x() = -data.GetFloat() ;
            }
        }

        // offset v
        {
            melange::GeData data ;
            if(tx_tag->GetParameter( melange::TEXTURETAG_OFFSETY, data ))
            {
                uv_offset.y() = data.GetFloat() ;
            }
        }

        // scale u
        {
            melange::GeData data ;
            if(tx_tag->GetParameter( melange::TEXTURETAG_LENGTHX, data ))
            {
                uv_scale.x() = 1.0f / data.GetFloat() ;
            }
        }

        // scale v
        {
            melange::GeData data ;
            if(tx_tag->GetParameter( melange::TEXTURETAG_LENGTHY, data ))
            {
                uv_scale.y() = 1.0f / data.GetFloat() ;

            }
        }

        mat_ptr->add_texture_layer( uv_scale, uv_offset ) ;
    }
    // currently, this is a fallback due to lack of mix mode and mix strength
    else
    {
        melange::GeData data ;
        if( mel_mat_ptr->GetParameter( melange::MATERIAL_COLOR_COLOR, data ) )
        {
            so_math::vec3f_t color = this_t::to_vector( data.GetVector() ) ;
            mat_ptr->add_color_layer( color ) ;
        }
    }

    // textures
    // @note in order to be able to add textures, you need texture 
    // layers, so texture layers need to be added before textures.
    if( max_textures > 0 )
    {
        using namespace melange ;

        melange::BaseShader * cur_shd_ptr = mel_mat_ptr->GetFirstShader() ;

        if(so_core::is_not_nullptr( cur_shd_ptr ))
        {
            do
            {
                if(cur_shd_ptr->GetType() != Xbitmap) continue ;

                so_imex::image_manager_ptr_t img_mgr_ptr =
                    params_in.manager_reg_ptr->get_image_manager() ;

                melange::Filename const fn = cur_shd_ptr->GetFileName() ;

                melange::Char * filename_ptr = fn.GetString().GetCStringCopy() ;

                so_io::path_t p( filename_ptr ) ;

                DeleteMem( filename_ptr ) ;

                if(so_core::is_not( so_std::filesystem::exists( p ) ))
                {
                    auto ipath = doc_path ;
                    ipath.append( so_io::path_t( "tex/" ) ).append( p ) ;

                    if(so_core::is_not( so_std::filesystem::exists( ipath ) ))
                        continue ;

                    p = ipath ;
                }

                {
                    so_imex::image_manager_t::handle_t hnd ;
                    if(so_core::is_not( img_mgr_ptr->acquire_by_path( p.string(),
                        "[c4d_module::import_all_materials] : require key", hnd ) ))
                    {
                        so_log::log::warning(
                            so_std::string_t( "[c4d_module::import_all_materials] : "
                            "no key for path : " ) + p.string() ) ;
                        continue ;
                    }

                    so_shade::texture_type tt ;
                    tt.base = so_shade::type_base::tbyte ;
                    tt.dim = so_shade::texture_dimension::dim_2d ;
                    tt.encoding = so_shade::texture_encoding::color ;

                    so_shade::texture_variable_t tv ;
                    tv.name = hnd.get_key() ;
                    tv.of_type = tt ;


                    mat_ptr->add_texture( 0, tv ) ;
                }


            } while( so_core::is_not_nullptr( cur_shd_ptr = cur_shd_ptr->GetNext() ) ) ;
        }
    }    

    // @todo more c4d material properties here
    {
        // observe melange material and fill in so_shade standard_material
        (void_t)mel_mat_ptr ;
    }

    so_std::string_t cur_mat_key ;
    {
        auto const res = ctx.get_material_key( mel_mat_ptr, cur_mat_key ) ;
        so_log::log::error_and_exit( so_core::is_not(res), so_std::string_t( "[c4d_module::import_material] : "
        "material must exists" ) + cur_name ) ;
    }

    so_imex::material_manager_t::manage_params_t mp ;
    mp.data_ptr = mat_ptr ;
    auto res = params_in.manager_reg_ptr->get_material_manager()->insert( cur_mat_key, mp ) ;
    if( res != so_imex::ok )
    {
        so_log::log::warning( so_std::string_t( "[c4d_module::import_all_materials] : "
                                "unable to manage material : " ) + cur_mat_key ) ;

        so_shade::so_material::standard_material_t::destroy( mat_ptr ) ;
    }
}

//**********************************************************************************************
void_t c4d_module::import_all_images( so_std::string_cref_t prev_name, melange::BaseObject * mel_obj_ptr, 
    so_imex::iscene_module::import_params_cref_t params_in, so_io::path_cref_t doc_path, 
    so_thread::task_graph_ref_t tg_outer, c4d_import_context_ref_t ctx )
{
    if( so_core::is_nullptr( mel_obj_ptr ) )
        return ;

    do
    {
        so_std::string_t const cur_name = prev_name + "." +
            this_t::to_string( mel_obj_ptr->GetName() ) ;

        melange::BaseTag * btag_ptr = mel_obj_ptr->GetFirstTag() ;

        while( so_core::is_not_nullptr(btag_ptr) )
        {
            if( btag_ptr->GetType() != Ttexture )
            {
                btag_ptr = btag_ptr->GetNext() ;
                continue ;
            }

            melange::TextureTag * tx_tag = (melange::TextureTag*)btag_ptr ;
            melange::BaseMaterial * mel_mat_ptr = tx_tag->GetMaterial() ;

            if( so_core::is_nullptr( mel_mat_ptr ) )
            {
                btag_ptr = btag_ptr->GetNext() ;
                continue ;
            }
            
            melange::BaseShader * shd = mel_mat_ptr->GetFirstShader() ;

            while( so_core::is_not_nullptr( shd ) )
            {
                if( shd->GetType() != Xbitmap )
                {
                    shd = shd->GetNext() ;
                    continue ;
                }

                so_std::string_t key = this_t::to_string(
                    shd->GetFileName().GetFileString() ) ;

                if( ctx.add_bitmap( shd, key ) ) 
                {
                    auto const res = this->import_image( key, shd, params_in, 
                        doc_path, tg_outer, ctx ) ;

                    // unfortunately, a BaseShader is not unique for a single
                    // texture, so it might be possible that the image manager
                    // rejects the image by its path because the same image 
                    // is already loaded via another BaseShader.
                    if( res == so_imex::found )
                    {
                        // the entry must exist because is was just inserted.
                        so_log::log::error_and_exit(so_core::is_not(ctx.change_bitmap_key( shd, key )),
                            "[c4d_module::import_all_images] : programming error : " + key ) ;
                    }
                    else if( res == so_imex::path_does_not_exist )
                    {
                        so_log::log::warning( "[c4d_module::import_all_images] : "
                            "path for key does not exist : " + key ) ;
                        ctx.remove_bitmap( shd ) ;
                        
                    }
                }

                shd = shd->GetNext() ;
            }
            
            btag_ptr = btag_ptr->GetNext() ;
        }

        this_t::import_all_images( cur_name, mel_obj_ptr->GetDown(), params_in, doc_path, tg_outer, ctx );

        mel_obj_ptr = mel_obj_ptr->GetNext() ;

    } while( so_core::is_not_nullptr( mel_obj_ptr ) ) ;
}

//**********************************************************************************************
void_t c4d_module::import_all_splines( so_std::string_cref_t prev_name, 
    melange::BaseObject * mobj_ptr, c4d_import_context_ref_t ctx,
    so_thread::task_graph_ref_t tg_outer  ) 
{
    if( so_core::is_nullptr( mobj_ptr ) )
        return ;

    do
    {
        so_std::string_t const cur_name = prev_name + "." +
            this_t::to_string( mobj_ptr->GetName() ) ;
        
        if( mobj_ptr->GetType() == Ospline )
        {
            this->import_spline( cur_name, (melange::SplineObject*)mobj_ptr, ctx, tg_outer ) ;
        }
        
        this_t::import_all_splines( cur_name, mobj_ptr->GetDown(), ctx, tg_outer );

        mobj_ptr = mobj_ptr->GetNext() ;

    } while( so_core::is_not_nullptr( mobj_ptr ) ) ;
}

//**********************************************************************************************
void_t c4d_module::import_spline( so_std::string_cref_t cur_name,
    melange::SplineObject * mspline_ptr, c4d_import_context_ref_t ctx,
    so_thread::task_graph_ref_t tg_outer )
{   
    size_t const num_points = mspline_ptr->GetPointCount() ;
    size_t const num_tangs = mspline_ptr->GetTangentCount() ;

    melange::Vector const * pptr = mspline_ptr->GetPointR() ;
    melange::Tangent const * tptr = mspline_ptr->GetTangentR() ;

    for( size_t i = 0; i < num_points; ++i )
    {
        melange::Vector const mpoint = pptr[i] ;
        so_math::vec3f_t point = this_t::to_vector( mpoint ) ;
    }

    for( size_t i = 0; i < num_tangs; ++i )
    {
        melange::Tangent const mtangent = tptr[i] ;
        so_math::vec3f_t left = this_t::to_vector( mtangent.vl ) ;
        so_math::vec3f_t right = this_t::to_vector( mtangent.vr ) ;
    }

    // spline type
    {
        melange::GeData data ;
        if( mspline_ptr->GetParameter( melange::SPLINEOBJECT_TYPE, data ) )
        {
            int32_t const v = data.GetInt32() ;
            switch( v )
            {
            case melange::SPLINEOBJECT_TYPE_LINEAR: break ;
            case melange::SPLINEOBJECT_TYPE_BEZIER: break ;
            case melange::SPLINEOBJECT_TYPE_CUBIC: break ; // cubic hermite spline catrom

            }
        }
    }

    // interpolation type
    {
        melange::GeData data ;
        if( mspline_ptr->GetParameter( melange::SPLINEOBJECT_INTERPOLATION, data ) )
        {
            int32_t const v = data.GetInt32() ;
            switch( v )
            {
            case melange::SPLINEOBJECT_INTERPOLATION_NONE: break ;
            case melange::SPLINEOBJECT_INTERPOLATION_NATURAL: break ;
            case melange::SPLINEOBJECT_INTERPOLATION_UNIFORM: break ;
            
            case melange::SPLINEOBJECT_INTERPOLATION_ADAPTIVE: 
            case melange::SPLINEOBJECT_INTERPOLATION_SUBDIV: 
            default:
            // not supported
            break ;
            }
        }
    }
}

//**********************************************************************************************
void_t c4d_module::import_all_cameras( so_std::string_cref_t prev_name, 
    melange::BaseObject * mobj_ptr, c4d_import_context_ref_t ctx ) 
{
    if( so_core::is_nullptr( mobj_ptr ) )
        return ;
    do
    {
        so_std::string_t const cur_name = prev_name + "." +
            this_t::to_string( mobj_ptr->GetName() ) ;

        if( mobj_ptr->GetType() == Ocamera )
        {
            so_std::string_t key = this_t::to_string(mobj_ptr->GetName()) ;
            if( ctx.add_camera( mobj_ptr, key ) )
            {
                this->import_camera( key, mobj_ptr, ctx ) ;
            }
            
        }

        this_t::import_all_cameras( cur_name, mobj_ptr->GetDown(), ctx );

        mobj_ptr = mobj_ptr->GetNext() ;

    } while( so_core::is_not_nullptr( mobj_ptr ) ) ;
}

//**********************************************************************************************
void_t c4d_module::import_camera( so_std::string_cref_t cur_name, 
    melange::BaseObject * mobj_ptr, c4d_import_context_ref_t ctx ) 
{
    so_imex::generic_camera_t cam ;
    so_imex::pinhole_lens_t phole_lens ;

    // projection
    {
        melange::GeData data ;
        if( mobj_ptr->GetParameter( melange::CAMERA_PROJECTION, data ) )
        {
            switch( data.GetInt32() )
            {
            case melange::Pperspective:
            break ;
            case melange::Pparallel:
            break ;
            default:
            break ;
            }
        }
        
        // add projection type to camera or lens
    }

    // Focal length
    {
        melange::GeData data ;
        if( mobj_ptr->GetParameter(melange::CAMERA_FOCUS, data) )
        {

        }
    }

    // Zoom
    {
        melange::GeData data ;
        if( mobj_ptr->GetParameter( melange::CAMERA_ZOOM, data ) )
        {

        }
    }

    {
        so_math::vec2f_t fov ;
        // Fov Horizontal
        {
            melange::GeData data ;
            if( mobj_ptr->GetParameter( melange::CAMERAOBJECT_FOV, data ) )
            {
                fov.x() = data.GetFloat() ;
            }
        }

        // Fov Vertical
        {
            melange::GeData data ;
            if( mobj_ptr->GetParameter( melange::CAMERAOBJECT_FOV_VERTICAL, data ) )
            {
                fov.y() = data.GetFloat() ;
            }
        }
        phole_lens.set_fov_hv( fov.x(), fov.y() ) ;
    }

    // Sensor Size / Film gate
    {
        melange::GeData data ;
        if( mobj_ptr->GetParameter( melange::CAMERAOBJECT_APERTURE, data ) )
        {

        }
    }

    // 
    {
        melange::GeData data ;
        if( mobj_ptr->GetParameter( melange::CAMERAOBJECT_NEAR_CLIPPING, data ) )
        {
            float_t const n = std::max( float_t(data.GetFloat()), 1.0f ) ;
            phole_lens.set_near( n ) ;
        }
    }

    // 
    {
        melange::GeData data ;
        if( mobj_ptr->GetParameter( melange::CAMERAOBJECT_FAR_CLIPPING, data ) )
        {
            phole_lens.set_far( float_t(data.GetFloat()) ) ;
        }
    }

    cam.add_lens( so_imex::pinhole_lens_t::create( std::move(phole_lens), 
        "[so_imex::c4d_module::import_camera] : pinhole_lens" ) ) ;

    so_imex::camera_manager_t::manage_params mp ;
    mp.data_ptr = so_imex::generic_camera_t::create( std::move(cam), 
        "[so_imex::c4d_module::import_camera] : generic_camera" ) ;

    {
        auto const res = ctx.import_params().manager_reg_ptr->get_camera_manager()->insert(
            cur_name, mp ) ;

        if( so_imex::no_success( res ) )
        {
            so_log::log::error( "[so_imex::c4d_module::import_camera] : "
                "inserting camera" ) ;
            mp.data_ptr->destroy() ;
        }
    }
}

//**********************************************************************************************
so_imex::result c4d_module::import_image( so_std::string_inout_t key_io, melange::BaseShader * mel_shd_ptr,
    so_imex::iscene_module::import_params_cref_t params_in, so_io::path_cref_t doc_path, 
    so_thread::task_graph_ref_t tg_outer, c4d_import_context_ref_t ctx ) 
{
    using namespace melange ;

    so_imex::image_manager_ptr_t img_mgr_ptr = params_in.manager_reg_ptr->get_image_manager() ;
    
    melange::Filename const fn = mel_shd_ptr->GetFileName() ;
            
    melange::Char * filename_ptr = fn.GetString().GetCStringCopy() ;

    so_io::path_t p( filename_ptr ) ;

    DeleteMem( filename_ptr ) ;

    if( so_core::is_not( so_std::filesystem::exists( p ) ) )
    {
        auto ipath = doc_path ;
        ipath.append(so_io::path_t( "tex/" )).append(p) ;
        p = ipath ;

        if( so_core::is_not( so_std::filesystem::exists( p ) ) )
        {
            so_log::log::warning( "[c4d_module::import_image] : image not found : " +
                std::string(p.string()) ) ;

            return so_imex::path_does_not_exist ;
        }
    }

    
    so_imex::key_t key_for_image = key_io ;

    // at this stage, the images are only collected in order to
    // generate unique keys for the images by path. the acquisition will
    // be done during graph creation.
    auto const res = img_mgr_ptr->reserve_by_path( p.string(), key_for_image ) ;

    // if reserved, create load task
    if( res == so_imex::reserved )
    {
        so_imex::image_manager::manage_params mp ;
        mp.file_path = p.string() ;
        mp.data_ptr = nullptr ;

        img_mgr_ptr->exchange( key_for_image, mp ) ;

        {
            so_imex::iimage_module::import_params ip;
            ip.key = key_for_image;
            ip.img_mgr_ptr = img_mgr_ptr;
            ip.path_to_file = p;
            //ip.ios_ptr = params_in.ios_ptr ;

            tg_outer.in_between( params_in.module_reg_ptr->import_image( ip ) );
        }
    }

    key_io = key_for_image ;

    return res ;
}
