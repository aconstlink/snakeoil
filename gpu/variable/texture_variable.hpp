//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_VARIABLE_TEXTURE_VARIABLE_HPP_
#define _SNAKEOIL_GPU_VARIABLE_TEXTURE_VARIABLE_HPP_

#include "variable.h"

#include "../typedefs.h"
#include "../protos.h"

#include "../converter/convert_image_to_shader_variable_type.h"
#include "../enums/type_traits.h"
#include "../texture/texture_2d.h"

namespace so_gpu
{        
    // just an interface for simplifying things.
    // used for all texture variables being able to connect
    // to shaders.
    class SNAKEOIL_GPU_API itexture_variable : public so_gpu::variable
    {
    public:

        itexture_variable( void_t ){}
        itexture_variable( itexture_variable && rhv ) : variable(std::move(rhv) ){}
        virtual ~itexture_variable( void_t ) {}

    public:

        virtual so_gpu::texture_ptr_t get_texture( void_t ) = 0 ;
        virtual so_gpu::texture_cptr_t get_texture( void_t ) const = 0 ;
        virtual so_gpu::texture_ptr_t exchange_texture( so_gpu::texture_ptr_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    } ;

    /// this texture variable will take the required image info
    /// from the image interface. It is more practical to do in this way.
    /// the compile-time type is not always easily retrievable.
    class texture_variable_dyn : public itexture_variable
    {
        typedef itexture_variable base_t ;
        so_this_typedefs( texture_variable_dyn ) ;

    private:

        texture_ptr_t _tex_ptr = nullptr ;
        bool_t _has_changed = true ;

    public:

        static this_ptr_t create( this_rref_t rhv, std::string const & purpose ) 
        {
            return so_gpu::memory::alloc(std::move(rhv), purpose ) ;
        }

        static void_t destroy( this_ptr_t ptr ) 
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        texture_variable_dyn( so_gpu::image_type it, so_gpu::pixel_type pt ) 
        {
            base_t::set_shader_variable_type(
                    so_gpu::convert_image_to_shadar_type( it, pt )
                ) ;
        }
        
        texture_variable_dyn( texture_ptr_t ptr, so_gpu::image_type it, so_gpu::pixel_type pt ) 
        {
            _tex_ptr = ptr ;
            base_t::set_shader_variable_type(
                    so_gpu::convert_image_to_shadar_type( it, pt )
                ) ;
        }
        
        texture_variable_dyn( this_rref_t rhv ) : base_t( std::move( rhv) )
        {
            _tex_ptr = rhv._tex_ptr ;
            rhv._tex_ptr = nullptr ;
            _has_changed = rhv._has_changed ;
        }

        virtual ~texture_variable_dyn( void_t ){}

    public:

        void_t set( texture_ptr_t ptr ) 
        {
            _tex_ptr = ptr ;
            _has_changed = true ;
        }
        texture_cptr_t get( void_t ) const 
        {
            return _tex_ptr ;
        }
        texture_ptr_t get( void_t ) 
        {
            return _tex_ptr ;
        }

    public:

        bool_t has_changed( void_t ) const 
        {
            return _has_changed ;
        }
        void_t set_changed( bool_t b) 
        {
            _has_changed = b ;
        }

    public: // interface

        virtual so_gpu::texture_ptr_t get_texture( void_t )
        {
            return _tex_ptr ;
        }

        virtual so_gpu::texture_cptr_t get_texture( void_t ) const
        {
            return _tex_ptr ;
        }

        virtual so_gpu::texture_ptr_t exchange_texture( so_gpu::texture_ptr_t ptr )
        {
            auto * tmp = _tex_ptr ;
            _tex_ptr = ptr ;
            return tmp ;
        }

    //private:

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }

    };

    /// the image template parameter is required for deducing the run-time
    /// shader variable type up front. This is also coherent with
    /// the data_variable api.
    template< typename image_t >
    class texture_variable : public texture_variable_dyn
    {
        typedef texture_variable_dyn base_t ;
        so_this_typedefs( texture_variable<image_t> ) ;

    private:

        texture_ptr_t _tex_ptr = nullptr ;
        bool_t _has_changed = true ;

    public:

        static this_ptr_t create( void_t ) 
        {
            return this_t::create( this_t(), "[so_gpu::texture_variable::create]" ) ;
        }

        static this_ptr_t create( this_rref_t rhv, std::string const & purpose ) 
        {
            return so_gpu::memory::alloc(std::move(rhv), purpose ) ;
        }

        static void_t destroy( this_ptr_t ptr ) 
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        texture_variable( void_t ) 
        {
            base_t::set_shader_variable_type(
                    so_gpu::convert_image_to_shadar_type(
                        image_t::get_image_type(), image_t::get_pixel_type()
                    )
                ) ;
        }
        
        texture_variable( texture_ptr_t ptr ) 
        {
            _tex_ptr = ptr ;
            base_t::set_shader_variable_type(
                    so_gpu::convert_image_to_shadar_type(
                        image_t::get_image_type(), image_t::get_pixel_type()
                    )
                ) ;
        }
        
        texture_variable( this_rref_t rhv ) : base_t( std::move( rhv) )
        {
            _tex_ptr = rhv._tex_ptr ;
            rhv._tex_ptr = nullptr ;
            _has_changed = rhv._has_changed ;
        }

        virtual ~texture_variable( void_t ){}

    public:

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }

    };
}

#endif

