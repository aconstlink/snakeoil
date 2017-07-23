//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "feedback_buffer.h"

using namespace so_gpu ;

//************************************************************************************
feedback_buffer::this_ptr_t feedback_buffer::create( std::string const & purpose ) 
{
    return this_t::create( this_t(), purpose ) ;
}

//************************************************************************************
feedback_buffer::this_ptr_t feedback_buffer::create( this_rref_t rhv, std::string const & purpose )
{
    return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
}

//************************************************************************************
void_t feedback_buffer::destroy( this_ptr_t ptr ) 
{
    so_gpu::memory::dealloc( ptr ) ;
}

//************************************************************************************
feedback_buffer::feedback_buffer( void_t ) 
{

}

//************************************************************************************
feedback_buffer::feedback_buffer( this_rref_t rhv ) : base_t(std::move(rhv))
{
    _num_prims_written = rhv._num_prims_written;
    _buffers = std::move( rhv._buffers ) ;
    _has_changed = rhv._has_changed ;
}

//************************************************************************************
feedback_buffer::~feedback_buffer( void_t ) 
{

}

//************************************************************************************
size_t feedback_buffer::get_num_primitives_written( void_t ) const 
{
    return _num_prims_written ;
}

//************************************************************************************
void_t feedback_buffer::set_num_primitives_written( size_t nw) 
{
    _num_prims_written = nw ;
}

//************************************************************************************
feedback_buffer::this_ref_t feedback_buffer::add_buffer( so_gpu::buffer_ptr_t ptr ) 
{
    if( ptr == nullptr ) return *this ;

    auto iter = std::find( _buffers.begin(), _buffers.end(), ptr ) ;
    if( iter != _buffers.end() ) return *this ;

    _buffers.push_back( ptr ) ;
    set_changed(true) ;

    return *this ;
}

//************************************************************************************
size_t feedback_buffer::get_num_buffers( void_t ) 
{
    return _buffers.size() ;
}

//************************************************************************************
so_gpu::buffer_ptr_t feedback_buffer::get_buffer( size_t i ) 
{
    return _buffers[i] ;
}

//************************************************************************************
so_gpu::buffer_cptr_t feedback_buffer::get_buffer( size_t i ) const 
{
    return _buffers[i] ;
}

//************************************************************************************
feedback_buffer::buffers_ref_t feedback_buffer::get_buffers( void_t ) 
{
    return _buffers ;
}

//************************************************************************************
bool_t feedback_buffer::has_changed( void_t ) const 
{
    return _has_changed ;
}

//************************************************************************************
void_t feedback_buffer::set_changed( bool_t b ) 
{
    _has_changed = b ;
}

//************************************************************************************
void_t feedback_buffer::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

