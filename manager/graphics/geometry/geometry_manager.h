//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
    //------------------------------------------------------------
// Copyright 2010 - 2016 Alexis Constantin Link
// License under lic/snakeoil.lic.txt
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_GEOEMTRY_GEOMETRY_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_GEOEMTRY_GEOMETRY_H_

#include "../../api.h"
#include "../../result.h"
#include "../../typedefs.h"
#include "../../protos.h"

#include <snakeoil/geometry/mesh/flat_tri_mesh.h>
#include <snakeoil/geometry/mesh/flat_line_mesh.h>

#include <snakeoil/gpu/enums/primitive_types.h>
#include <snakeoil/gpu/protos.h>
#include <snakeoil/gpu/buffer/vertex_buffer.hpp>
#include <snakeoil/gpu/buffer/index_buffer.hpp>
#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/event/event.h>
#include <snakeoil/gpx/driver/driver_async.h>
#include <snakeoil/gpx/system/iuser_system.h>

#include <snakeoil/thread/typedefs.h>

#include <snakeoil/resource/handle/handle.hpp>
#include <snakeoil/resource/manager/resource_manager.hpp>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API geometry_manager
        {
            so_this_typedefs( geometry_manager ) ;

        private:

            struct store_item
            {
                so_gpu::primitive_type pt = so_gpu::primitive_type::undefined ;
                so_gpu::ivertex_buffer_ptr_t vb = nullptr ;
                so_gpu::iindex_buffer_ptr_t ib = nullptr ;
                so_gpx::event_ptr_t gpu_memory_evt_ptr = nullptr ;
            };
            so_typedef( store_item ) ;

            so_typedefs( so_resource::resource_manager<store_item_t>, resource_manager ) ;
            resource_manager_t _res_mgr ;

        public:

            so_typedefs( resource_manager_t::handle_t, handle ) ;
            so_typedefs( resource_manager_t::handles_t, handles ) ;

            struct manage_params
            {
                so_gpu::primitive_type pt = so_gpu::primitive_type::undefined ;
                so_gpu::ivertex_buffer_ptr_t vb = nullptr ;
                so_gpu::iindex_buffer_ptr_t ib = nullptr ;
            };
            so_typedef( manage_params ) ;

        private:

            so_thread::mutex_t _mtx_counter ;
            size_t _counter = 0 ;

        private:

            so_gpx::iuser_system_ptr_t _gpxs = nullptr ;

        private: // shutdown

            so_thread::mutex_t _mtx ;

            so_gpx::event _shutdown_ready ;

            /// security variable.
            bool_t _shutdown_called = false ;

        public:

            geometry_manager( so_gpx::iuser_system_ptr_t ) ;
            geometry_manager( this_rref_t ) ;
            virtual ~geometry_manager( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

#if 0
            template< typename vertex_t, typename index_t >
            so_manager::result create( so_gpu::primitive_type pt, so_manager::key_cref_t key_in )
            {
                so_thread::lock_t lk(_mtx) ;
                {
                    auto const iter = _key_to_data.find( key_in ) ;
                    if( iter != _key_to_data.end() ) 
                        return so_manager::invalid_argument ;
                }

                typedef so_gpu::vertex_buffer< vertex_t > vertex_buffer_t ;
                typedef so_gpu::index_buffer< index_t > index_buffer_t ;

                vertex_buffer_t * vb = 
                    vertex_buffer_t::create("[geometry_manager::create] : vertex_buffer") ;

                index_buffer_t * ib = 
                    index_buffer_t::create("[geometry_manager::create] : index_buffer") ;
                
                store_item d = store_item{pt, vb, ib} ;

                _key_to_data[key_in] = d ;

                _gpxs->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
                {
                    drv_ptr->create_buffer( vb ) ;
                    drv_ptr->create_buffer( ib ) ;
                } ) ;

                return so_manager::ok ;
            }

            template< typename vertex_t >
            so_manager::result create( so_gpu::primitive_type pt, so_manager::key_cref_t key_in )
            {
                so_thread::lock_t lk(_mtx) ;
                {
                    auto const iter = _key_to_data.find( key_in ) ;
                    if( iter != _key_to_data.end() ) 
                        return so_manager::invalid_argument ;
                }

                typedef so_gpu::vertex_buffer< vertex_t > vertex_buffer_t ;

                vertex_buffer_t * vb = 
                    vertex_buffer_t::create("[geometry_manager::create] : vertex_buffer") ;

                store_item d = store_item{pt, vb, nullptr} ;

                _key_to_data[key_in] = d ;

                _gpxs->execute( [=]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
                {
                    drv_ptr->create_buffer( vb ) ;
                } ) ;

                return so_manager::ok ;
            }
#endif

        public:

            bool_t acquire( so_manager::key_cref_t key_in,
                so_resource::purpose_cref_t p, handle_out_t hnd_out ) ;
            
            bool_t release( handle_rref_t hnd ) ;

        public: // insert/reserve/exchange

            /// does not acquire the resource. It only inserts it.
            so_manager::result insert( so_manager::key_cref_t key_in, manage_params_cref_t mp ) ;
            so_manager::result insert_and_create( so_manager::key_cref_t key_in, 
                manage_params_cref_t mp ) ;

            /// reserves the resource by key so that no other entity can get the name
            //so_manager::result reserve_by_key( so_manager::key_cref_t key_in ) ;

        public:

            virtual so_manager::result construct_from( so_geo::flat_tri_mesh_cref_t, 
                so_manager::key_cref_t ) ;

            virtual so_manager::result construct_from( so_geo::flat_line_mesh_cref_t, 
                so_manager::key_cref_t ) ;

            virtual so_manager::result upload( so_manager::key_cref_t ) ;
            virtual so_manager::result upload_and_wait( so_manager::key_cref_t ) ;

        public:

            virtual so_manager::result destroy( void_t ) ;
            virtual so_manager::result shutdown( void_t ) ;

        };
        so_typedef( geometry_manager ) ;
    }
}

#endif
