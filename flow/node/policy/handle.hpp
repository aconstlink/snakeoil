//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_POLICY_HANDLE_HPP_
#define _SNAKEOIL_FLOW_NODE_POLICY_HANDLE_HPP_

#include "../../typedefs.h"

namespace so_flow
{
    namespace so_policy
    {
        /// this handle class is for the user of the slot policies
        template< typename T >
        class handle
        {
            so_this_typedefs( handle<T> ) ;

            so_typedefs( T, owner ) ;
            typedef typename T::slot_ptr_t sptr_t ;

        private:

            so_flow::key_t _key = "" ;
            sptr_t _slot_ptr = nullptr ;
            owner_ptr_t _owner_ptr = nullptr ;            

        public:

            so_flow::key_cref_t get_key( void_t ) const { return _key ; }
            sptr_t get_ptr( void_t ) const { return _slot_ptr ; }
            sptr_t operator-> (void_t) { return _slot_ptr ; }

        public:

            handle( void_t ) {}

            handle( so_flow::key_cref_t key_in, sptr_t sptr, owner_ptr_t owner_ptr )
            {
                _key = key_in ;
                _slot_ptr = sptr ;
                _owner_ptr = owner_ptr ;
            }

            handle( this_rref_t rhv )
            {
                _key = std::move( rhv._key ) ;
                so_move_member_ptr( _slot_ptr, rhv ) ;
                so_move_member_ptr( _owner_ptr, rhv ) ;
            }

            ~handle( void_t )
            {
                if( so_core::is_not_nullptr(_owner_ptr) )
                    _owner_ptr->untouch( _key ) ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                _key = std::move( rhv._key ) ;
                so_move_member_ptr( _slot_ptr, rhv ) ;
                so_move_member_ptr( _owner_ptr, rhv ) ;

                return *this ;
            }

        private:

            handle( this_cref_t ) {}
            this_ref_t operator = ( this_cref_t ) { return *this ; }

        };
    }
    
}


#endif
