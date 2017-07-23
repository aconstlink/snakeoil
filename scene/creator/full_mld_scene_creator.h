//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SCENE_CREATOR_FULL_MLD_SCENE_CREATOR_H_
#define _SNAKEOIL_SCENE_CREATOR_FULL_MLD_SCENE_CREATOR_H_

#include "base_scene_creator.h"

namespace so_scene
{
    /// create a renderable scene graph from an imported scene
    class SNAKEOIL_SCENE_API full_mld_scene_creator : public base_scene_creator
    {
        so_this_typedefs( full_mld_scene_creator ) ;

    private:

        // non-copyable
        full_mld_scene_creator( this_cref_t ) {}

    public:

        full_mld_scene_creator( void_t ) ;
        full_mld_scene_creator( init_package_cref_t ) ;
        full_mld_scene_creator( this_rref_t ) ;
        virtual ~full_mld_scene_creator( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    private:

        virtual void_t on_prepare_scene( so_imex::so_node::node_ptr_t, so_thread::itask_ptr_t,
            so_scene::scene_creator_context_ptr_t ) ;
        virtual so_scene::so_node::group_ptr_t on_create_renderable( 
            so_imex::so_node::mesh_ptr_t, so_scene::scene_creator_context_ptr_t ) ;


    };
    so_typedef( full_mld_scene_creator ) ;
}

#endif
