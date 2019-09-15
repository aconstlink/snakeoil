//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_VARIABLE_VARIABLE_SET_H_
#define _SNAKEOIL_GPU_VARIABLE_VARIABLE_SET_H_

#include "../result.h"
#include "../protos.h"
#include "../object.h"

#include "../enums/type_traits.h"

#include "data_variable.hpp"
#include "texture_variable.hpp"
#include "data_buffer_variable.hpp"

#include <utility>

namespace so_gpu
{
    class SNAKEOIL_GPU_API variable_set : public so_gpu::object
    {
        typedef so_gpu::object base_t ;
        so_this_typedefs( variable_set ) ;

        typedef std::pair< variable_ptr_t, std::string > var_name_t ;
        typedef so_std::vector< var_name_t > variable_container_t ;
        
        typedef std::pair< itexture_variable_ptr_t, std::string > texture_name_t ;
        typedef so_std::vector< texture_name_t > texture_container_t ;

        typedef std::pair< idata_buffer_variable_ptr_t, std::string > buffer_name_t ;
        typedef so_std::vector< buffer_name_t > data_buffer_container_t ;

    private:

        variable_container_t _data_variables ;
        texture_container_t _texture_variables ;
        data_buffer_container_t _data_buffer_variables ;

        /// if true, it requires validation
        bool_t _changed = true ;
        
    public:

        variable_set( void_t ) ;
        variable_set( variable_set && rhv ) ;
        virtual ~variable_set( void_t ) ;

    public:

        static this_ptr_t create( void_t ) ;
        static this_ptr_t create( std::string const & purpose ) ;
        static this_ptr_t create( this_rref_t, std::string const & purpose ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        template< typename type_t >
        so_gpu::result bind_data( std::string const & name, type_t * data_ptr )
        {
            auto iter = std::find_if( _data_variables.begin(), _data_variables.end(), 
                [&](var_name_t const & var) { return var.second == name ; } ) ;
            
            // @todo if type match, do pointer exchange
            // beware of async variable processing!
            if( iter != _data_variables.end() ) return so_gpu::failed ;

            auto * var_ptr = so_gpu::memory::alloc( so_gpu::data_variable< type_t >(data_ptr), 
                "[variable_set] : bind_data" ) ;
            _data_variables.push_back( var_name_t(var_ptr, name) ) ;

            set_changed( true ) ;
            return so_gpu::ok ;
        }

        so_gpu::result bind_texture( std::string const & name, texture_ptr_t ptr )
        {
            if( ptr == nullptr ) 
                return so_gpu::invalid_argument ;

            return this_t::bind_texture( name, ptr, ptr->get_image() ) ;
        }

        so_gpu::result bind_texture( std::string const & name, 
            texture_ptr_t ptr, so_gpu::image_ptr_t iptr )
        {
            if( iptr == nullptr )
                return so_gpu::invalid_argument ;

            auto iter = std::find_if( _texture_variables.begin(), _texture_variables.end(), 
                [=](texture_name_t const & item) { return item.second == name ; } ) ;

            // @todo if type match, do pointer exchange
            // beware of async variable processing!
            if( iter != _texture_variables.end() ) return so_gpu::failed ;
            
            auto * var_ptr = so_gpu::texture_variable_dyn::create( 
                so_gpu::texture_variable_dyn( ptr, iptr->get_type(), iptr->get_pixel_type() ),
                "[variable_set] : texture_variable_dyn" ) ;

            _texture_variables.push_back( texture_name_t(var_ptr, name) ) ;            
            
            set_changed( true ) ;
            return so_gpu::ok ;
        }

        /// do we really need this function anymore? Binding the texture with the compile-time
        /// image is very unpractical. Maybe when slots come into play.
        template< typename image_t >
        so_gpu::result bind_texture( std::string const & name, texture_ptr_t ptr )
        {
            auto iter = std::find_if( _texture_variables.begin(), _texture_variables.end(), 
                [=](texture_name_t const & item) { return item.second == name ; } ) ;

            // @todo if type match, do pointer exchange
            // beware of async variable processing!
            if( iter != _texture_variables.end() ) return so_gpu::failed ;
            
            auto * var_ptr = so_gpu::memory::alloc( 
                so_gpu::texture_variable< image_t >(ptr), 
                "[variable_set] : bind_texture" ) ;

            _texture_variables.push_back( texture_name_t(var_ptr, name) ) ;            
            
            set_changed( true ) ;
            return so_gpu::ok ;
        }

        template< typename type_t >
        so_gpu::result bind_buffer( std::string const & name, idata_buffer_ptr_t ptr )
        {
            auto iter = std::find_if( _data_buffer_variables.begin(), _data_buffer_variables.end(), 
                [=](buffer_name_t const & item) { return item.second == name ; } ) ;

            // @todo if type match, do pointer exchange
            // beware of async variable processing!
            if( iter != _data_buffer_variables.end() ) return so_gpu::failed ;

            auto * var_ptr = so_gpu::memory::alloc( so_gpu::data_buffer_variable< type_t >(ptr), 
                "[variable_set] : bind_buffer" ) ;
            _data_buffer_variables.push_back( buffer_name_t(var_ptr, name) ) ;
            
            set_changed( true ) ;
            return so_gpu::ok ;
        }

    public:

        void_t set_changed( bool_t b ) {_changed = b ;}
        bool_t has_changed( void_t ) const { return _changed ; }

    public:

        variable_container_t & data_variables( void_t ) 
        {
            return _data_variables ;
        }

        texture_container_t & texture_variables( void_t )
        {
            return _texture_variables ;
        }

        data_buffer_container_t & data_buffer_variables( void_t )
        {
            return _data_buffer_variables ;
        }

        void_t clear( void_t )  ;

    public: // interface

        virtual void_t destroy( void_t ) ;

    };
    so_typedef( variable_set ) ;
}


#endif

