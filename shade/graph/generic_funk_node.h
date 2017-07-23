//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GRAPH_FUNK_NODE_GENERIC_FUNK_NODE_H_
#define _SNAKEOIL_SHADE_GRAPH_FUNK_NODE_GENERIC_FUNK_NODE_H_

#include "node.h"

namespace so_shade
{
    namespace so_graph
    {
        class SNAKEOIL_SHADE_API generic_funk_node : public node
        {
            typedef node base_t ;
            so_this_typedefs( generic_funk_node ) ;

        private:

            class connector : public iconnector
            {
                so_this_typedefs( connector ) ;
                so_typedefs( generic_funk_node, creator ) ;
   
            private:

                creator_ptr_t _creator_ptr = nullptr ;

            public:

                connector( creator_ptr_t ) ;
                connector( this_rref_t ) ;
                virtual ~connector( void_t ) ;

                static this_ptr_t create( this_rref_t ) ;
                static void_t destroy( this_ptr_t ) ;
            
                void_t exchange_creator_by( creator_ptr_t ) ;

            public:

                virtual void_t on_disconnect( so_shade::iinput_slot_cptr_t ) ;
                virtual void_t on_disconnect( so_shade::ioutput_slot_cptr_t ) ;

            public:

                virtual bool_t on_connect( so_shade::iinput_slot_ptr_t,
                    so_shade::ioutput_slot_ptr_t, connect_direction ) ;

            public:

                virtual void_t destroy( void_t ) ;
            };
            friend class connector ;

            so_typedef( connector ) ;
            so_typedefs( so_std::vector<connector_ptr_t>, connectors ) ;
            
            connector_ptr_t _con_ptr = nullptr ;

        private:

            so_shade::so_kernel::ikernel_ptr_t _kernel_ptr = nullptr ;

        public:

            generic_funk_node( this_rref_t ) ;
            generic_funk_node( so_shade::so_kernel::ikernel_ptr_t ) ;
            virtual ~generic_funk_node( void_t ) ;
            
        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;


        public:

            virtual so_shade::result connect_by_name(
                so_std::string_cref_t, so_shade::ioutput_slot_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;

        private:

            bool_t check_kernel_slot_connectability( so_shade::ioutput_slot_cptr_t ) ;
            void_t determine_output_slot_type_from_inputs( void_t ) ;

        };
        so_typedef( generic_funk_node ) ;
    }
}

#endif
