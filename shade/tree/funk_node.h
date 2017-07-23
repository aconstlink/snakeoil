//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_NODE_FUNK_FUNK_NODE_H_
#define _SNAKEOIL_SHADE_NODE_FUNK_FUNK_NODE_H_

#include "group_node.h"

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/list.hpp>

namespace so_shade
{
    namespace so_tree
    {
        class SNAKEOIL_SHADE_API funk_node : public group
        {
            typedef group base_t ;
            so_this_typedefs( funk_node ) ;

            struct arg_item
            {
                so_std::string_t name ;
                /// the original argument node
                so_shade::so_tree::arg_node_ptr_t arg_ptr = nullptr ;
                /// the replaced argument tree
                so_shade::so_tree::node_ptr_t rep_ptr = nullptr ;
            };
            so_typedef( arg_item ) ;

            so_typedefs( so_std::vector<arg_item_t>, args ) ;

        public:

            so_typedefs( so_std::list<so_std::string_t>, string_list ) ;

        private:

            /// the name of the function
            so_std::string_t _name ;
            args_t _args ;

        public:

            funk_node( so_std::string_cref_t ) ;
            funk_node( this_rref_t ) ;
            virtual ~funk_node( void_t ) ;

        public:

            so_std::string_cref_t get_name( void_t ) const ;

            so_shade::result exchange_by_name( so_std::string_cref_t, node_ptr_t ) ;

            string_list_t get_arg_name_list( void_t ) const ;

            so_shade::result add_arg( so_std::string_cref_t,
                so_shade::so_tree::arg_node_ptr_t ) ;      

            so_shade::result create_arg( so_std::string_cref_t ) ;

        public: // interface

            virtual node_ptr_t clone( void_t ) const = 0 ;
            virtual void_t destroy( void_t ) = 0 ;
        };
        so_typedef( funk_node ) ;
    }
}

#endif
