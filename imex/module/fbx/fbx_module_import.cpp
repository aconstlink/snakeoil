//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "fbx_module.h"

#include "../../manager/graph_manager.h"
#include "../../manager/mesh_manager.h"
#include "../../manager/camera_manager.h"
#include "../../manager/image_manager.h"
#include "../../manager/manager_registry.h"

#include "../../node/mesh/mesh_asset.h"
#include "../../node/image/image_asset.h"
#include "../../node/decorator/imported_from.h"
#include "../../node/decorator/transform_3d.h"
#include "../../node/group/logic_group.h"
#include "../../node/mesh/mesh.h"
#include "../../node/mesh/mesh_asset.h"
#include "../../node/material/material_asset.h"
#include "../../node/camera/camera_asset.h"

#include "../../camera/generic_camera.h"
#include "../../camera/pinhole_lens.h"

#include "../../module/iimage_module.h"
#include "../../module/module_registry.h"

#include <snakeoil/io/path/path_helper.h>

#include <snakeoil/math/utility/angle.hpp>
#include <snakeoil/geometry/mesh/polygon_mesh.h>
#include <snakeoil/thread/task/task_graph.h>
#include <snakeoil/thread/task/tasks.h>
#include <snakeoil/log/log.h>

using namespace so_imex ;


//*************************************************************************************
so_thread::task_graph_t fbx_module::import_scene( import_params_cref_t params_in )
{
    so_thread::itask_ptr_t tt_end = so_thread::void_funk_task_t::create_noop(
        "[so_imex::fbx_module::import_scene] : end node") ;

    so_thread::itask_ptr_t tt_begin = so_thread::dyn_task_t::create( 
        [params_in, tt_end, this]( so_thread::itask_ptr_t self_ptr )
    {
        so_thread::task_graph_t tg_outer( self_ptr, tt_end ) ;

        FbxManager * fbx_manager = this_t::_fbx_manager ;
        FbxScene * scene = nullptr ;
        FbxImporter * importer = nullptr ;

        // import the fbx scene
        {
            so_thread::lock_guard_t lk( this->_fbx_mtx ) ;
            
            importer = FbxImporter::Create( fbx_manager, params_in.key.c_str() ) ;

            bool status = importer->Initialize( params_in.path_to_file.c_str(),
                -1, fbx_manager->GetIOSettings() ) ;

            if(so_log::log::error( so_core::not(status),
                "[so_imex::fbx_module::import_scene] : failed to load scene from path for key " +
                    params_in.key ))
            {
                so_imex::sync_object::set_and_signal( params_in.sync_ptr, so_imex::failed ) ;
                importer->Destroy() ;
                return ;
            }

            scene = FbxScene::Create( fbx_manager, params_in.key.c_str() ) ;
            importer->Import( scene ) ;
        }

        
        FbxGlobalSettings * gs = &scene->GetGlobalSettings() ;
        (void)gs ;

        FbxAxisSystem as = gs->GetAxisSystem() ;
        int up = gs->GetOriginalUpAxis() ;
        
        (void) as ;
        (void) up ;

        FbxAxisSystem my_system( FbxAxisSystem::eOpenGL ) ;
        my_system.ConvertScene( scene ) ;

        // import the assets
        {
            this_t::import_all_mesh_geometries( params_in.key, scene->GetRootNode(), 
                params_in.manager_reg_ptr->get_mesh_manager() ) ;

            //this_t::import_all_cameras( params_in.key, scene->GetRootNode(), params_in.cam_store_ptr ) ;
            
            
            this_t::import_all_images( params_in.key, scene->GetRootNode(), 
                params_in.manager_reg_ptr->get_image_manager(), params_in, tg_outer ) ;
        }

        {
            this_t::import_the_root( params_in.key, scene->GetRootNode(), params_in ) ;
        }

        // clean up fbx stuff
        {
            so_thread::lock_guard_t lk( this->_fbx_mtx ) ;
            importer->Destroy() ;
        }

        so_log::log::status( "[so_imex::fbx_module::import_scene] : [finised] import task : " +
            params_in.key )  ;
    }) ;

    // load all images via image module via task graph
    
    return so_thread::task_graph_t( tt_begin, tt_end ) ;
}

//*************************************************************************************
so_imex::result fbx_module::import_the_root( so_imex::key_cref_t key_in, 
    FbxNode * fbx_root, import_params_cref_t params_in ) 
{
    //
    // STEP 1: Create resources
    //
    so_imex::so_node::imported_from_ptr_t imp_from_ptr = so_imex::so_node::imported_from::create(
        so_imex::so_node::imported_from_t("fbxsdk"), 
        "[so_imex::fbx_module::import_scene] : Root Node for : " + key_in ) ;

    so_imex::so_node::logic_group_ptr_t root_ptr = so_imex::so_node::logic_group::create(
            "[so_imex::fbx_module::import_scene] : Root Node for : " + key_in ) ;
    
    {        
        auto * trafo_nptr = so_imex::so_node::transform_3d_t::create(
            so_imex::so_node::transform_3d_t( root_ptr ),
            "[so_imex::fbx_module::import_the_root] : transform_3d") ;
        
        so_math::so_3d::trafof_t rotate ;
        rotate.rotate_by_angle_fr( so_math::vec3f_t(
            so_math::angle<float_t>::degree_to_radian(180),0.0f,0.0f) ) ;

        trafo_nptr->set_transformation( rotate ) ;
        
        imp_from_ptr->set_decorated( trafo_nptr ) ;
    }

    //
    // STEP 2: Do traversal
    //
    int ccount = fbx_root->GetChildCount() ;
    for(int i = 0; i < ccount; ++i)
    {
        FbxNode * child_ptr = fbx_root->GetChild( i ) ;
        this_t::import_the_graph( key_in, child_ptr, root_ptr, params_in ) ;
    }    

    //
    // STEP 3: manage the graph
    //
    {
        //auto res = _graph_mgr_ptr->manage( key_in, imp_from_ptr ) ;

        so_imex::graph_manager::manage_params mp ;
        mp.data_ptr = imp_from_ptr ;

        auto res = params_in.manager_reg_ptr->get_graph_manager()->insert( key_in, mp ) ;
        if( so_imex::no_success(res) )
        {
            so_imex::so_node::imported_from_t::destroy( imp_from_ptr ) ;
        }
    }

    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result fbx_module::import_the_graph( so_imex::key_cref_t key_in, FbxNode * fbx_node_ptr,
    so_imex::so_node::logic_group_ptr_t parent_ptr, import_params_cref_t params_in ) 
{
    std::string const node_name_dbg( fbx_node_ptr->GetName() ) ;

    so_imex::so_node::logic_group_ptr_t new_parent_ptr = 
            so_imex::so_node::logic_group::create( 
            "[so_imex::fbx_module::import_the_graph] : logic_group for " + 
            key_in + "." + node_name_dbg ) ;    

    // Attributes
    // add the leaf nodes to the parent here
    {
        int acount = fbx_node_ptr->GetNodeAttributeCount() ;

        for(int i = 0; i < acount; ++i)
        {
            FbxNodeAttribute * attr_ptr = fbx_node_ptr->GetNodeAttributeByIndex( i ) ;
            FbxNodeAttribute::EType type = attr_ptr->GetAttributeType() ;
            
            so_imex::key_t const cur_key = this_t::construct_key( key_in, fbx_node_ptr ) ;
            so_imex::so_node::node_ptr_t to_be_inserted = nullptr ;

            switch( type )
            {
                case FbxNodeAttribute::eUnknown: break ;
                case FbxNodeAttribute::eNull: break ;
                case FbxNodeAttribute::eMarker: break ;
                case FbxNodeAttribute::eSkeleton: break ; 
                case FbxNodeAttribute::eMesh:
                    to_be_inserted = this_t::import_mesh_scenes( 
                        cur_key, fbx_node_ptr, (FbxMesh*) attr_ptr, params_in ) ;
                    break ; 
                case FbxNodeAttribute::eNurbs: break ; 
                case FbxNodeAttribute::ePatch: break ;
                case FbxNodeAttribute::eCamera: 
                    to_be_inserted = this_t::import_camera_scene( 
                        cur_key, fbx_node_ptr, (FbxCamera*)attr_ptr, params_in ) ;
                    break ;
                case FbxNodeAttribute::eCameraStereo: break ;
                case FbxNodeAttribute::eCameraSwitcher: break ;
                case FbxNodeAttribute::eLight: break ;
                case FbxNodeAttribute::eOpticalReference: break ;
                case FbxNodeAttribute::eOpticalMarker: break ;
                case FbxNodeAttribute::eNurbsCurve: break ;
                case FbxNodeAttribute::eTrimNurbsSurface: break ;
                case FbxNodeAttribute::eBoundary: break ;
                case FbxNodeAttribute::eNurbsSurface: break ;
                case FbxNodeAttribute::eShape: break ;
                case FbxNodeAttribute::eLODGroup: break ;
                case FbxNodeAttribute::eSubDiv: break ;
                case FbxNodeAttribute::eCachedEffect: break ;
                case FbxNodeAttribute::eLine: break ;
            }
            new_parent_ptr->add_child( to_be_inserted ) ;                
        }
    }

    // Children
    {
        int const ccount = fbx_node_ptr->GetChildCount() ;
        
        for(int i = 0; i < ccount; ++i)
        {
            FbxNode * child_ptr = fbx_node_ptr->GetChild( i ) ;
            this_t::import_the_graph( key_in, child_ptr, new_parent_ptr, params_in ) ;
        }
    }

    so_imex::so_node::node_ptr_t to_be_added = new_parent_ptr ;

    // Transformation
    // Add a transformation decorator node
    {
        // decorate the last decorated by this transformation
        auto * trafo_nptr = so_imex::so_node::transform_3d_t::create(
            so_imex::so_node::transform_3d_t( to_be_added ),
            "[so_imex::fbx_module::import_the_grap] : transform_3d") ;

        {
            FbxDouble3 translation = fbx_node_ptr->LclTranslation.Get() ;
            FbxDouble3 rotation = fbx_node_ptr->LclRotation.Get() ;
            FbxDouble3 scale = fbx_node_ptr->LclScaling.Get() ;
            
            FbxDouble3 translation_geo = fbx_node_ptr->GetGeometricTranslation(FbxNode::eSourcePivot) ;
            FbxDouble3 rotation_geo = fbx_node_ptr->GetGeometricRotation(FbxNode::eSourcePivot) ;
            FbxDouble3 scale_geo = fbx_node_ptr->GetGeometricScaling(FbxNode::eSourcePivot) ;

            FbxEuler::EOrder order = fbx_node_ptr->RotationOrder.Get() ;
            (void_t) order ;

            FbxDouble3 prerotation = fbx_node_ptr->PreRotation.Get() ;
            FbxDouble3 postrotation = fbx_node_ptr->PostRotation.Get() ;
            (void)prerotation ;
            (void)postrotation ;

            FbxDouble3 spivot = fbx_node_ptr->ScalingPivot ;
            (void) spivot ;
            FbxDouble3 soffset= fbx_node_ptr->ScalingOffset ;
            (void)spivot ;

            auto y_v = fbx_node_ptr->UpVectorProperty.Get() ;
            auto z_v = fbx_node_ptr->UpVectorProperty.Get() ;

            (void) y_v ;
            (void) z_v ;

            FbxRotationOrder ro = fbx_node_ptr->RotationOrder ;
            (void)ro ;
            
            FbxDouble3 rot_piv = fbx_node_ptr->RotationPivot.Get() ;
            FbxDouble3 rot_off = fbx_node_ptr->RotationOffset.Get() ;

            FbxDouble axis_len = fbx_node_ptr->AxisLen.Get() ;

            so_math::vec3f_t const rot_vec = 
                this_t::fbx_angles_swizzle( this_t::fbx_angles_vector(rotation) ) ;

            so_math::vec3f_t const prerot_vec = 
                this_t::fbx_angles_swizzle( this_t::fbx_angles_vector(prerotation) ) ;

            so_math::vec3f_t const trans_vec = 
                this_t::fbx_vector_swizzle( this_t::conv_vector(translation) ) ;

            so_math::vec3f_t const scale_vec = 
                this_t::fbx_scale_swizzle( this_t::conv_vector(scale) ) ;

            //FbxTransform::EInheritType it = fbx_node_ptr->InheritType.Get(float_t(scale.Buffer()[0])) ;
            //(void)it ;
            (void)translation_geo ;
            (void)rotation_geo ;
            (void)scale_geo ;
            (void)axis_len ;

            // transform by separate local transformation
            {
                so_math::so_3d::trafof_t tscale ; 
                tscale.scale_fl( scale_vec ) ;

                so_math::vec3f_t const x = so_math::vec3f_t( so_math::so_vector::x_axis() ) ;
                so_math::vec3f_t const y = so_math::vec3f_t( so_math::so_vector::y_axis() ) ;
                so_math::vec3f_t const z = so_math::vec3f_t( so_math::so_vector::z_axis() ) ;

                //
                // NOTE! the order of the axes is very important
                // fbx is switch the angles, so must the axis of rotation change in order
                // fbx sdk says: XYZ
                // we need: YZX => Y and Z are switched

                so_math::so_3d::trafof_t trotate ;
                //trotate.rotate_by_angle_fr( prerot_vec ) ;

                // apply the prerotation first
                {
                    trotate.rotate_by_axis_fr( y, prerot_vec.y() ) ;
                    trotate.rotate_by_axis_fr( z, prerot_vec.z() ) ;
                    trotate.rotate_by_axis_fr( x, prerot_vec.x() ) ;
                }

                // apply the rotation
                {                    
                    trotate.rotate_by_axis_fr( y, rot_vec.y() ) ;
                    trotate.rotate_by_axis_fr( z, rot_vec.z() ) ;
                    trotate.rotate_by_axis_fr( x, rot_vec.x() ) ;                    
                }

                so_math::so_3d::trafof_t ttranslate ;
                ttranslate.translate_fl( trans_vec ) ;

                //so_math::so_3d::trafof_t const trafo = ttranslate * tscale * trotate ;//* texchange ;
                //so_math::so_3d::trafof_t const trafo = tscale * trotate * ttranslate ;

                so_math::so_3d::trafof_t const trafo = ttranslate * trotate * tscale ;

                trafo_nptr->set_transformation( trafo ) ;
            }
        }

        to_be_added = trafo_nptr ;
    }

    parent_ptr->add_child( to_be_added ) ;

    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result fbx_module::import_all_mesh_geometries( so_imex::key_cref_t key, 
    FbxNode * node_ptr, so_imex::mesh_manager_ptr_t mesh_mgr_ptr ) 
{
    // Attributes
    {
        int acount = node_ptr->GetNodeAttributeCount() ;

        for(int i = 0; i < acount; ++i)
        {
            FbxNodeAttribute * attr_ptr = node_ptr->GetNodeAttributeByIndex( i ) ;
            FbxNodeAttribute::EType type = attr_ptr->GetAttributeType() ;
            if( type == FbxNodeAttribute::eMesh )
            {
                this_t::import_mesh_geometry( key, node_ptr, (FbxMesh*)attr_ptr, mesh_mgr_ptr ) ;
            }
        }
    }

    // Children
    {
        int ccount = node_ptr->GetChildCount() ;
        for(int i = 0; i < ccount; ++i)
        {
            FbxNode * child_ptr = node_ptr->GetChild( i ) ;
            this_t::import_all_mesh_geometries( key, child_ptr, mesh_mgr_ptr ) ;
        }
    }

    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result fbx_module::import_mesh_geometry( so_imex::key_cref_t key, FbxNode * node_in_ptr, 
    FbxMesh * mesh_ptr, so_imex::mesh_manager_ptr_t mesh_mgr_ptr ) 
{
    so_geo::polygon_mesh the_mesh ;

    int const poly_count = mesh_ptr->GetPolygonCount() ;
    int const index_count = mesh_ptr->GetPolygonVertexCount() ;
    int const cp_count = mesh_ptr->GetControlPointsCount() ;

    the_mesh.polygons.resize( poly_count ) ;

    // indices 
    {
        int counter = 0 ;

        the_mesh.indices.resize( index_count ) ;
        for(int poly_i = 0; poly_i < poly_count; ++poly_i)
        {
            int const poly_size = mesh_ptr->GetPolygonSize( poly_i ) ;
            for(int poly_pos = 0; poly_pos < poly_size; ++poly_pos)
            {
                int const index = mesh_ptr->GetPolygonVertex( poly_i, poly_pos ) ;
                the_mesh.indices[counter++] = index ;
            }
            the_mesh.polygons[poly_i] = poly_size ;
        }
    }

    // control points (vertex positions)
    {
        the_mesh.position_format = so_geo::vector_component_format::xyz ;
        the_mesh.positions.resize( cp_count*3 ) ;

        for( int i=0; i<cp_count; ++i )
        {
            FbxVector4 pos = mesh_ptr->GetControlPointAt( i ) ;
            the_mesh.positions[i*3+0] = float_t(pos.Buffer()[0]) ;
            the_mesh.positions[i*3+1] = float_t(pos.Buffer()[2]) ;
            the_mesh.positions[i*3+2] = float_t(pos.Buffer()[1]) ;
        }
    }

    // normals
    {
        int const necount = mesh_ptr->GetElementNormalCount() ;
        the_mesh.normals.resize( necount ) ;
        the_mesh.normals_indices.resize( necount ) ;

        for( int ne=0; ne<necount; ++ne )
        {
            FbxLayerElementNormal * element_normal = mesh_ptr->GetElementNormal( ne ) ;
            int const normal_count = element_normal->GetDirectArray().GetCount() ;
            int const normal_index_count = element_normal->GetIndexArray().GetCount() ;
            
            // do normals
            {
                the_mesh.normals[ne].resize( normal_count * 3 ) ;
                for( int i=0; i<normal_count; ++i )
                {
                    FbxVector4 const normal = element_normal->GetDirectArray()[i] ;
                    the_mesh.normals[ne][i*3+0] = float_t(normal.Buffer()[0]) ;
                    the_mesh.normals[ne][i*3+1] = float_t(normal.Buffer()[1]) ;
                    the_mesh.normals[ne][i*3+2] = float_t(normal.Buffer()[2]) ;
                }
            }

            // do indices
            {
                if( element_normal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect )
                {
                    the_mesh.normals_indices[ne].resize( normal_index_count ) ;

                    if( element_normal->GetMappingMode() == FbxGeometryElement::eByControlPoint )
                    {
                        for(size_t i = 0; i < the_mesh.indices.size(); ++i)
                        {
                            the_mesh.normals_indices[ne][i] = 
                                uint_t( element_normal->GetIndexArray()[uint_t(i)] ) ;
                        }
                    }
                    else if( element_normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
                    {
                        for(int i = 0; i < the_mesh.indices.size(); ++i)
                        {
                            uint_t const vi = the_mesh.indices[i] ;
                            the_mesh.normals_indices[ne][i] = 
                                uint_t( element_normal->GetIndexArray()[vi] ) ;
                        }
                    }                    
                }
                else if( element_normal->GetReferenceMode() == FbxGeometryElement::eDirect )
                {
                    if( element_normal->GetMappingMode() == FbxGeometryElement::eByControlPoint )
                    {
                        the_mesh.normals_indices[ne].resize( index_count ) ;

                        for(size_t i = 0; i < index_count; ++i)
                        {
                            the_mesh.normals_indices[ne][i] = uint_t( the_mesh.indices[i] ) ;
                        }
                    }
                    else if( element_normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
                    {
                        the_mesh.normals_indices[ne].resize( index_count ) ;

                        for(size_t i = 0; i < index_count; ++i)
                        {
                            the_mesh.normals_indices[ne][i] = uint_t( i ) ;
                        }
                    }
                }
            }
        }
    }

    // texcoords
    {
        int const necount = mesh_ptr->GetElementUVCount() ;
        the_mesh.texcoords.resize( necount ) ;
        the_mesh.texcoords_indices.resize( necount ) ;

        for(int ne = 0; ne < necount; ++ne)
        {
            FbxLayerElementUV * element_uv = mesh_ptr->GetElementUV( ne ) ;
            int const uv_count = element_uv->GetDirectArray().GetCount() ;
            int const uv_index_count = element_uv->GetIndexArray().GetCount() ;

            // do texcoords
            {
                the_mesh.texcoords[ne].resize( uv_count * 2 ) ;
                for(int i = 0; i < uv_count; ++i)
                {
                    FbxVector2 const uv = element_uv->GetDirectArray()[i] ;
                    the_mesh.texcoords[ne][i * 2 + 0] = float_t( uv.Buffer()[0] ) ;
                    the_mesh.texcoords[ne][i * 2 + 1] = float_t( uv.Buffer()[1] ) ;                    
                }
            }

            // do indices
            {
                if(element_uv->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                {
                    the_mesh.texcoords_indices[ne].resize( uv_index_count ) ;

                    if(element_uv->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                    {
                        for(size_t i = 0; i < the_mesh.indices.size(); ++i)
                        {
                            the_mesh.texcoords_indices[ne][i] =
                                uint_t( element_uv->GetIndexArray()[uint_t( i )] ) ;
                        }
                    }
                    else if(element_uv->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                    {
                        for(int i = 0; i < the_mesh.indices.size(); ++i)
                        {
                            //uint_t const vi = the_mesh.indices[i] ;
                            the_mesh.texcoords_indices[ne][i] =
                                uint_t( element_uv->GetIndexArray()[i] ) ;
                        }
                    }
                }
                else if(element_uv->GetReferenceMode() == FbxGeometryElement::eDirect)
                {
                    if(element_uv->GetMappingMode() == FbxGeometryElement::eByControlPoint)
                    {
                        the_mesh.texcoords_indices[ne].resize( index_count ) ;

                        for(size_t i = 0; i < index_count; ++i)
                        {
                            the_mesh.texcoords_indices[ne][i] = uint_t( the_mesh.indices[i] ) ;
                        }
                    }
                    else if(element_uv->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
                    {
                        the_mesh.texcoords_indices[ne].resize( index_count ) ;

                        for(size_t i = 0; i < index_count; ++i)
                        {
                            the_mesh.texcoords_indices[ne][i] = uint_t( i ) ;
                        }
                    }
                }
            }
        }
    }

    // material layers
    // not used at the moment
    {
        int num_mat_layers = mesh_ptr->GetElementMaterialCount() ;

        so_log::log::warning( num_mat_layers == 0,
            "[so_imex::fbx_module::import_mesh_geometry] : mesh has no materials" );

        so_log::log::warning( num_mat_layers > 1,
            "[so_imex::fbx_module::import_mesh_geometry] : multiple material layers not supported at the moment." ) ;

        num_mat_layers = num_mat_layers >= 1 ? 1 : 0 ;

        for(int i = 0; i < num_mat_layers; ++i)
        {
            FbxLayerElementMaterial * mat_layer_ptr = mesh_ptr->GetElementMaterial( i ) ;

            if(mat_layer_ptr->GetMappingMode() == FbxLayerElement::eAllSame)
            {
                int const mat_id = mat_layer_ptr->GetIndexArray()[0] ;
                (void_t)mat_id ;
            }
            else if(mat_layer_ptr->GetMappingMode() == FbxLayerElement::eByPolygon)
            {
                for(int mi = 0; mi < mat_layer_ptr->GetIndexArray().GetCount(); ++mi)
                {
                    int const mat_id = mat_layer_ptr->GetIndexArray()[mi] ;
                    (void_t)mat_id ;
                }
            }
            else
            {
                so_log::log::error( "[so_imex::fbx_module::import_mesh_geometry] : \
                                        invalid material mapping" ) ;
            }
        }
    }

    std::vector< uint_t > material_ids ;

    {
        int const num_materials = node_in_ptr->GetMaterialCount() ;
        for(int mi = 0; mi < num_materials; ++mi)
        {
            uint_t const mid = uint_t( mi ) ;
            material_ids.push_back( mid ) ;
        }
    }

    for( auto mid : material_ids )
    {
        so_imex::key_t the_key = this_t::construct_key( key, mesh_ptr->GetNode() ) ;

        the_key = the_key + "." + std::to_string( mid ) ;

        // split mesh based on material ids
        {
        }

        // store imported mesh
        {
            so_geo::polygon_mesh_ptr_t store_mesh_ptr = so_geo::polygon_mesh_t::create(
                std::move( the_mesh ), "[so_imex::fbx_module::import_mesh] : tri_mesh" ) ;

            so_imex::mesh_manager::manage_params mp ;
            mp.data_ptr = store_mesh_ptr ;

            auto const res = mesh_mgr_ptr->insert( the_key, mp ) ;

            if( so_imex::no_success( res ) )
            {
                so_log::log::error( "[so_imex::fbx_module::import_mesh] : manage key: " + the_key ) ;
                store_mesh_ptr->destroy() ;
                return so_imex::failed ;
            }
        }
    }    
    
    return so_imex::ok ;
}

//*************************************************************************************
so_imex::so_node::node_ptr_t fbx_module::import_mesh_scenes( so_imex::key_cref_t key_in, 
    FbxNode * node_in_ptr, FbxMesh * fbx_mesh_ptr, import_params_cref_t params_in ) 
{

    std::vector< uint_t > material_ids ;
    
    {
        int const num_materials = node_in_ptr->GetMaterialCount() ;
        for( int mi=0; mi<num_materials; ++mi )
        {
            uint_t const mid = uint_t( mi ) ;
            material_ids.push_back( mid ) ;
        }

        if( so_log::log::warning( num_materials == 0, 
            "[so_imex::fbx_module::import_mesh_scenes] : No materials in " + key_in ) )
        {
            return nullptr ;
        }
    }

    so_imex::so_node::logic_group_ptr_t mesh_group_ptr = so_imex::so_node::logic_group::create(
        "[so_imex::fbx_module::import_mesh_scenes] : mesh group" ) ;

    //for(auto const & material_ids : material_layers)
    {
        for(auto mid : material_ids)
        {
            // make mesh asset node 
            so_std::string_t const mesh_key = key_in + "." + std::to_string( mid ) ;            
            mesh_group_ptr->add_child( this_t::import_mesh_scene( 
                mesh_key, mid, node_in_ptr, fbx_mesh_ptr, params_in ) ) ;
        }
    }

    if( so_log::log::warning( mesh_group_ptr->get_num_children() == 0, 
        "[so_imex::fbx_module::import_mesh_scenes] : new mesh group has no children." ) )
    {
        mesh_group_ptr->destroy() ;
        mesh_group_ptr = nullptr ;
    }

    return mesh_group_ptr ;
}

//*************************************************************************************
so_imex::so_node::mesh_ptr_t fbx_module::import_mesh_scene( so_imex::key_cref_t key_in, 
    uint_t mid, FbxNode * node_in_ptr, FbxMesh * fbx_mesh_ptr, import_params_cref_t params_in ) 
{
    so_imex::so_node::mesh_ptr_t mesh_ptr = so_imex::so_node::mesh_t::create(
        "[so_imex::fbx_module::import_mesh_scene] : mesh" ) ;

    // assign mesh asset
    {
        so_imex::mesh_manager_t::handle_t mhnd ;
        auto const res = params_in.manager_reg_ptr->get_mesh_manager()->acquire( key_in, 
            "[so_imex::fbx_module::import_mesh_scene]", mhnd ) ;

        so_log::log::error( so_core::not(res), 
            "[so_imex::fbx_module::import_mesh_scene] : mesh does not exist : " + key_in ) ;

        if( res )
        {
            // mesh asset
            so_imex::so_node::mesh_asset_ptr_t mesh_asset_ptr = so_imex::so_node::mesh_asset_t::create(
                so_imex::so_node::mesh_asset_t( std::move( mhnd ) ),
                "[so_imex::fbx_module::import_mesh_scene] : mesh_asset" ) ;

            mesh_ptr->set_asset( mesh_asset_ptr ) ;
        }        
    }
    
#if 0
    so_imex::so_node::shader_material_ptr_t material_ptr = 
        so_imex::so_node::shader_material_t::create(
        "[so_imex::fbx_module::import_mesh_scene] : shader_material" ) ;

    {
        FbxSurfaceMaterial * mat_ptr = node_in_ptr->GetMaterial( mid ) ;

        if( so_core::is_not_nullptr(mat_ptr) ) 
        {
            int_t tex_index = 0 ;
            FBXSDK_FOR_EACH_TEXTURE( tex_index )
            {
                FbxProperty tx_prop = mat_ptr->FindProperty(
                    FbxLayerElement::sTextureChannelNames[tex_index] );

                if( so_core::not(tx_prop.IsValid()) ) continue ;

                int_t tex_count = tx_prop.GetSrcObjectCount<FbxTexture>() ;
                for(int_t j = 0; j < tex_count; ++j)
                {
                    FbxLayeredTexture * layered_ptr = tx_prop.GetSrcObject<FbxLayeredTexture>( j );

                    if(layered_ptr != nullptr)
                    {

                    }
                    else
                    {
                        FbxTexture * tex_ptr = tx_prop.GetSrcObject<FbxTexture>( j );

                        FbxFileTexture * tex_file_ptr = FbxCast<FbxFileTexture>( tex_ptr );
                        //FbxProceduralTexture * tex_proc_ptr= FbxCast<FbxProceduralTexture>(tex_ptr);

                        so_std::utf8_t absolute_path = so_std::utf8_t( tex_file_ptr->GetFileName() ) ;
                        so_std::utf8_t relative_path = so_std::utf8_t( tex_file_ptr->GetRelativeFileName() ) ;

                        std::replace( absolute_path.begin(), absolute_path.end(), '\\', '/' );

                        auto * img_mgr = params_in.manager_reg_ptr->get_image_manager() ;

                        so_imex::image_manager_t::handle_t hnd ;
                        auto const res = img_mgr->acquire_by_path( absolute_path, 
                            "[so_imex::fbx_module::import_mesh_scene] : image_asset", hnd ) ;

                        if( res )
                        {
                            auto * image_ass_ptr = so_imex::so_node::image_asset_t::create(
                                so_imex::so_node::image_asset_t( std::move(hnd) ),
                                "[so_imex::fbx_module::import_mesh_scene] : image_asset" ) ;

                            material_ptr->add_image( image_ass_ptr ) ;
                        }
                    }
                }
            }
        }
    }

#endif

#if 0
    if( false )
    {
        // shader asset
        {
            so_imex::so_node::shader_ptr_t shader_ptr = so_imex::so_node::shader_t::create( 
                "[so_imex::fbx_module::import_mesh_scene] : shader" ) ;

            // add vertex shader
            {
                so_imex::so_node::shader_asset_ptr_t sptr = so_imex::so_node::shader_asset_t::create(
                        "[so_imex::fbx_module::import_mesh_scene] : shader_asset vertex shader" ) ;
                
                // set key
                // set manager

                shader_ptr->add_shader( sptr ) ;
            }

            // add pixel shader
            {
                so_imex::so_node::shader_asset_ptr_t sptr = so_imex::so_node::shader_asset_t::create(
                        "[so_imex::fbx_module::import_mesh_scene] : shader_asset vertex shader" ) ;

                // set key
                // set manager

                shader_ptr->add_shader( sptr ) ;
            }

            // add geometry shader
            {
                so_imex::so_node::shader_asset_ptr_t sptr = so_imex::so_node::shader_asset_t::create(
                        "[so_imex::fbx_module::import_mesh_scene] : shader_asset vertex shader" ) ;

                // set key
                // set manager

                shader_ptr->add_shader( sptr ) ;
            }

            material_ptr->set_shader( shader_ptr ) ;
        }
        
    }

#endif

    // material state
    {}
    
#if 0
    
    mesh_ptr->set_material( material_ptr ) ;

#endif

    return mesh_ptr ;
}

//*************************************************************************************
so_imex::so_node::camera_asset_ptr_t fbx_module::import_camera_scene( so_imex::key_cref_t key_in, 
    FbxNode * node_ptr, FbxCamera * cam_node_ptr, import_params_cref_t params_in ) 
{
#if 0
    auto * cam_asset_ptr = so_imex::so_node::camera_asset::create( 
        so_imex::so_node::camera_asset( key_in ),
        "[so_imex::fbx_module::import_camera_scene] : camera_asset" ) ;

    return cam_asset_ptr ;
#endif
    return nullptr ;
}

//*************************************************************************************
so_imex::result fbx_module::import_all_cameras( so_imex::key_cref_t key_in, 
    FbxNode * fbx_node_ptr, so_imex::camera_manager_ptr_t cam_store_ptr ) 
{
    // Attributes
    {
        int acount = fbx_node_ptr->GetNodeAttributeCount() ;

        for( int i = 0; i < acount; ++i )
        {
            FbxNodeAttribute * attr_ptr = fbx_node_ptr->GetNodeAttributeByIndex( i ) ;
            FbxNodeAttribute::EType type = attr_ptr->GetAttributeType() ;
            if(type == FbxNodeAttribute::eCamera)
            {
                so_imex::key_t const cur_key = this_t::construct_key( key_in, fbx_node_ptr ) ;
                this_t::import_camera( cur_key, (FbxCamera*)attr_ptr, cam_store_ptr ) ;
            }
        }
    }

    // Children
    {
        int ccount = fbx_node_ptr->GetChildCount() ;
        for(int i = 0; i < ccount; ++i)
        {
            FbxNode * child_ptr = fbx_node_ptr->GetChild( i ) ;
            this_t::import_all_cameras( key_in, child_ptr, cam_store_ptr ) ;
        }
    }

    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result fbx_module::import_camera( so_imex::key_cref_t key_in, 
    FbxCamera * cam_node_ptr, so_imex::camera_manager_ptr_t cam_store_ptr ) 
{
    so_imex::generic_camera_ptr_t cam_ptr = so_imex::generic_camera::create(
            "[so_imex::fbx_module::import_camera] : generic_camera" ) ;

    // handle camera matrix
    {    
        so_math::so_3d::trafof_t rotate ;
        rotate.rotate_by_angle_fr( so_math::vec3f_t(
            so_math::angle<float_t>::degree_to_radian(180),0.0f,0.0f) ) ;
        
        cam_ptr->set_position( rotate * this_t::fbx_vector_swizzle( 
            this_t::conv_vector(cam_node_ptr->Position.Get()) ) ) ;

        cam_ptr->set_up_vector( rotate *  this_t::fbx_vector_swizzle( 
            this_t::conv_vector(cam_node_ptr->UpVector.Get()) ) ) ;

#if 0 // invalid now - need look vector
        cam_ptr->set_look( rotate *  this_t::fbx_vector_swizzle(
            this_t::conv_vector(cam_node_ptr->InterestPosition.Get()) ) ) ;
#endif
        // as a little fall-back, if the up and the dir are looking in
        // same direction, just "ortho any" the up vector.
        so_math::vec3f_t const up = cam_ptr->get_up_vector().normalized() ;
        so_math::vec3f_t const dir = so_math::vec3f_t(
            cam_ptr->get_look()).normalized() ;
        float_t const udd = up.dot( dir ) ;
        if( std::abs(udd*udd-1.0) < std::numeric_limits<float_t>::epsilon() )
        {
            cam_ptr->set_up_vector( up.ortho_any() ) ;
        }
    }

#if 0
    // make lens
    {
        so_imex::pinhole_lens_ptr_t lens_ptr = so_imex::pinhole_lens::create(
            "[so_imex::fbx_module::import_camera] : pinhole_lens" ) ;
        
        // handle projection
        {
            float_t const aspect = float_t(
                cam_node_ptr->AspectWidth.Get() / cam_node_ptr->AspectHeight.Get() ) ;

            float_t far_plane = float_t( cam_node_ptr->FarPlane.Get() ) ;
            float_t near_plane = float_t( cam_node_ptr->NearPlane.Get() ) ;
            float_t fov = so_math::angle<float_t>::degree_to_radian(
                float_t( cam_node_ptr->FieldOfView.Get() ) ) ;

            lens_ptr->set_aspect( aspect ) ;
            lens_ptr->set_far( far_plane ) ;
            lens_ptr->set_near( near_plane ) ;
            lens_ptr->set_fov( fov ) ;
        }

        cam_ptr->add_lens( lens_ptr ) ;
    }

#endif
#if 0
    // put into manager
    {        
        so_imex::camera_manager_t::manage_params mp ;
        mp.cam_ptr = cam_ptr ;        

        auto res = cam_store_ptr->insert_and_acquire( key_in, mp ) ;
        if( so_log::log::error( so_imex::no_success( res ),
            "[so_imex::fbx_module::import_camera] : failed to insert camera." ) )
        {
            cam_ptr->destroy() ;
            return so_imex::failed ;
        }
    }
#endif
    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result fbx_module::import_all_images( so_imex::key_cref_t key_in, 
    FbxNode * fbx_node_ptr, so_imex::image_manager_ptr_t img_mgr_ptr, 
    import_params_cref_t params_in, so_thread::task_graph_ref_t tg_outer ) 
{
    so_imex::key_t node_key = this_t::construct_key( key_in, fbx_node_ptr ) ;
    // 
    {
        int mcount = fbx_node_ptr->GetMaterialCount() ;

        for(int i = 0; i < mcount; ++i)
        {
            FbxSurfaceMaterial * mat_ptr = fbx_node_ptr->GetMaterial( i ) ;
            
            if( mat_ptr == nullptr ) continue ;
           
            int_t tex_index = 0 ;
            FBXSDK_FOR_EACH_TEXTURE( tex_index )
            {
                FbxProperty tx_prop = mat_ptr->FindProperty(
                    FbxLayerElement::sTextureChannelNames[tex_index] );

                if( so_core::not(tx_prop.IsValid()) ) continue ;
                
                int_t tex_count = tx_prop.GetSrcObjectCount<FbxTexture>() ;
                for( int_t j=0; j<tex_count; ++j )
                {
                    FbxLayeredTexture * layered_ptr = tx_prop.GetSrcObject<FbxLayeredTexture>(j);
                    
                    if( so_core::is_not_nullptr(layered_ptr) )
                    {
                        
                    }
                    else
                    {
                        FbxTexture * tex_ptr = tx_prop.GetSrcObject<FbxTexture>(j);
                        
                        FbxFileTexture * tex_file_ptr = FbxCast<FbxFileTexture>(tex_ptr);
                        //FbxProceduralTexture * tex_proc_ptr= FbxCast<FbxProceduralTexture>(tex_ptr);
                                                
                        so_std::utf8_t absolute_path = so_std::utf8_t(tex_file_ptr->GetFileName()) ;
                        so_std::utf8_t relative_path = so_std::utf8_t(tex_file_ptr->GetRelativeFileName()) ;

                        std::replace( absolute_path.begin(), absolute_path.end(), '\\', '/' );
                        std::replace( relative_path.begin(), relative_path.end(), '\\', '/' );

                        so_imex::key_t key_for_image = key_in + ".image." +
                                img_mgr_ptr->unique_number_as_string() ;
                        
                        so_std::utf8_t path_no_name ;
                        so_io::path_helper::path_without_filename( params_in.path_to_file, path_no_name ) ;
                        
                        so_std::utf8_t path_to_use = path_no_name + relative_path ;

                        // at this stage, the images are only collected in order to
                        // generate unique keys for the images by path. the acquisition will
                        // be done during graph creation.
                        auto const res = img_mgr_ptr->reserve_by_path( 
                            path_to_use, key_for_image ) ;

                        // if reserved, we need to set the data still
                        if( res == so_imex::reserved )
                        {
                            so_imex::image_manager::manage_params mp ;
                            mp.file_path = path_to_use ;
                            mp.data_ptr = nullptr ;

                            img_mgr_ptr->exchange( key_for_image, mp ) ;

                            {
                                so_imex::iimage_module::import_params ip;
                                ip.key = key_for_image;
                                ip.img_mgr_ptr = img_mgr_ptr;
                                ip.path_to_file = path_to_use;
                                ip.ios_ptr = params_in.ios_ptr ;

                                tg_outer.in_between( params_in.module_reg_ptr->import_image( ip ) );
                            }
                        }
                        // nothing to do if found
                        // the image is already inserted previously and the
                        // load task is also issued already
                        //else if( res == so_imex::found ) {}
                        
                        
                        so_log::log::error( res == so_imex::invalid, 
                            "[so_imex::fbx_module::import_all_images] : image key" ) ;
                    }
                }
            }
        }
    }

    // Children
    {
        int ccount = fbx_node_ptr->GetChildCount() ;
        for(int i = 0; i < ccount; ++i)
        {
            FbxNode * child_ptr = fbx_node_ptr->GetChild( i ) ;
            this_t::import_all_images( key_in, child_ptr, img_mgr_ptr, params_in, tg_outer ) ;
        }
    }

    return so_imex::ok ;
}
