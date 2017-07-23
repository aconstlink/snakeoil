//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GEOMETRY_BUFFER_CREATOR_H_
#define _SNAKEOIL_MANAGER_GEOMETRY_BUFFER_CREATOR_H_

#include "../../typedefs.h"
#include "../../result.h"
#include "../../api.h"
#include "../../protos.h"

#include <snakeoil/geometry/mesh/flat_tri_mesh.h>
#include <snakeoil/geometry/mesh/flat_line_mesh.h>

#include <snakeoil/geometry/mesh/flat_tri_mesh.h>

#include <snakeoil/gpu/result.h>
#include <snakeoil/gpu/protos.h>
#include <snakeoil/gpu/enums/primitive_types.h>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API buffer_creator
        {
            so_this_typedefs( buffer_creator ) ;

        private:

            so_gpu::primitive_type _pt = so_gpu::primitive_type::undefined ;
            so_gpu::ivertex_buffer_ptr_t _vb = nullptr ;
            so_gpu::iindex_buffer_ptr_t _ib = nullptr ;

        public:

            buffer_creator( this_rref_t ) ;
            buffer_creator( so_geo::flat_tri_mesh_cref_t ) ;
            buffer_creator( so_geo::flat_line_mesh_cref_t ) ;
            ~buffer_creator( void_t ) ;

        public:

            so_gpu::primitive_type get_primitive_type( void_t ) const ;
            so_gpu::ivertex_buffer_ptr_t move_vertex_buffer( void_t ) ;
            so_gpu::iindex_buffer_ptr_t move_index_buffer( void_t ) ;

        private:

            void_t construct_from( so_geo::flat_tri_mesh_cref_t ) ;
            void_t construct_from( so_geo::flat_line_mesh_cref_t ) ;

        };

    }
}

#endif