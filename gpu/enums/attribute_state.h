//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_ATTRIBUTE_STATE_H_
#define _SNAKEOIL_GPU_ATTRIBUTE_STATE_H_

#include "../typedefs.h"

namespace so_gpu
{    
    enum class attribute_state
    {
        color,
        blend,
        depth,
        stencil,
        polygon,
        viewport,
        scissor
    };    

    class attribute_state_field
    {
        so_this_typedefs( attribute_state_field ) ;

    private:

        size_t _bits ;

    public:
        
        attribute_state_field( void_t ) : _bits(0) {}

        this_ref_t add( attribute_state const ps ) 
        { 
            _bits |= size_t(1) << size_t(ps) ; 
            return *this ; 
        }

        this_ref_t remove( attribute_state const ps ) 
        { 
            _bits ^= ((size_t(1) << size_t(ps)) & _bits) ; 
            return *this ; 
        }

        this_ref_t clear( void_t ) 
        { 
            _bits = 0 ; return *this ; 
        }

        size_t get_bit( attribute_state const ps ) const 
        { 
            return (_bits >> size_t(ps)) & size_t(1) ; 
        }

        bool_t has_set( attribute_state const ps ) const 
        { 
            return get_bit(ps) != 0 ; 
        }

        bool_t is_empty( void_t ) const 
        { 
            return _bits == 0 ; 
        }
    };
    so_typedef( attribute_state_field ) ;
}

#endif

