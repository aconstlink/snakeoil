//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_DAE_ELEMENTS_DAE_NODES_H_
#define _SNAKEOIL_IMEX_MODULE_DAE_ELEMENTS_DAE_NODES_H_

#include "dae_protos.h"
#include "dae_node.h"
#include "dae_visitor.h"

namespace so_imex
{
    namespace so_dae
    {
        //****************************************************************************
        struct collada : public so_dae::node
        {
            so_this_typedefs( collada ) ;
            
            collada( void_t ) : so_dae::node("COLLADA")
            {}

            collada( this_rref_t rhv ) : so_dae::node( std::move(rhv) )
            {}

            virtual ~collada( void_t ) {}

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( collada ) ;

        //****************************************************************************
        struct asset : public so_dae::node
        {
            so_this_typedefs( asset ) ;

            so_std::string_t up ;

            asset( void_t ) : so_dae::node( "asset" )
            {}

            asset( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( up, rhv ) ;
            }

            virtual ~asset( void_t ) {}

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( asset ) ;

        //****************************************************************************
        struct unit : public so_dae::node
        {
            so_this_typedefs( unit ) ;

            so_std::string_t meter ;
            so_std::string_t name ;

            unit( void_t ) : so_dae::node( "unit" )
            {}

            unit( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( meter, rhv ) ;
                so_move( name, rhv ) ;
            }

            virtual ~unit( void_t ) {}

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( unit ) ;

        //****************************************************************************
        struct source : public so_dae::node
        {
            so_this_typedefs( source ) ;

            so_std::string_t id ;

            source( void_t ) : so_dae::node( "source" )
            {}

            source( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( id, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( source ) ;

        //****************************************************************************
        struct mesh : public so_dae::node
        {
            so_this_typedefs( mesh ) ;


            mesh( void_t ) : so_dae::node( "mesh" )
            {}

            mesh( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( mesh ) ;

        //****************************************************************************
        struct geometry : public so_dae::node
        {
            so_this_typedefs( geometry ) ;

            so_std::string_t id ;
            so_std::string_t name ;

            geometry( void_t ) : so_dae::node( "geometry" )
            {}

            geometry( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( id, rhv ) ;
                so_move( name, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( geometry ) ;

        //****************************************************************************
        struct float_array : public so_dae::node
        {
            so_this_typedefs( float_array ) ;

            so_std::string_t id ;
            so_std::string_t count ;

            float_array( void_t ) : so_dae::node( "float_array" )
            {}

            float_array( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( id, rhv ) ;
                so_move( count, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( float_array ) ;

        //****************************************************************************
        struct technique_common : public so_dae::node
        {
            so_this_typedefs( float_array ) ;

            technique_common( void_t ) : so_dae::node( "technique_common" )
            {}

            technique_common( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {}

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( technique_common ) ;

        //****************************************************************************
        struct accessor : public so_dae::node
        {
            so_this_typedefs( accessor ) ;

            so_std::string_t src ;
            so_std::string_t count ;
            so_std::string_t stride ;

            accessor( void_t ) : so_dae::node( "accessor" )
            {}

            accessor( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( src, rhv ) ;
                so_move( count, rhv ) ;
                so_move( stride, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( accessor ) ;

        //****************************************************************************
        struct param : public so_dae::node
        {
            so_this_typedefs( param ) ;

            so_std::string_t name ;
            so_std::string_t type ;

            param( void_t ) : so_dae::node( "param" )
            {}

            param( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( name, rhv ) ;
                so_move( type, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( param ) ;

        //****************************************************************************
        struct vertices : public so_dae::node
        {
            so_this_typedefs( vertices ) ;

            so_std::string_t id ;

            vertices( void_t ) : so_dae::node( "vertices" )
            {}

            vertices( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( id, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( vertices ) ;

        //****************************************************************************
        struct polygons : public so_dae::node
        {
            so_this_typedefs( polygons ) ;
            so_typedefs( so_std::vector<so_std::string_t>, per_polygon_indices ) ;

            so_std::string_t count ;
            so_std::string_t material ;

            polygons( void_t ) : so_dae::node( "polygons" )
            {}

            polygons( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( count, rhv ) ;
                so_move( material, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( polygons ) ;

        //****************************************************************************
        struct primitives : public so_dae::node
        {
            so_this_typedefs( primitives ) ;
            so_typedefs( so_std::vector<so_std::string_t>, per_polygon_indices ) ;

            per_polygon_indices_t indices ;

            primitives( void_t ) : so_dae::node( "polygons" )
            {}

            primitives( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( indices, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( primitives ) ;

        //****************************************************************************
        struct input : public so_dae::node
        {
            so_this_typedefs( input ) ;

            so_std::string_t semantic ;
            so_std::string_t src ;
            so_std::string_t offset ;
            so_std::string_t set ;

            input( void_t ) : so_dae::node( "input" )
            {}

            input( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( semantic, rhv ) ;
                so_move( src, rhv ) ;
                so_move( offset, rhv ) ;
                so_move( set, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( input ) ;

        //****************************************************************************
        struct library_effects : public so_dae::node
        {
            so_this_typedefs( library_effects ) ;
            
            library_effects( void_t ) : so_dae::node( "library_effects" )
            {}

            library_effects( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( library_effects ) ;

        //****************************************************************************
        struct library_materials : public so_dae::node
        {
            so_this_typedefs( library_materials ) ;

            library_materials( void_t ) : so_dae::node( "library_materials" )
            {}

            library_materials( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( library_materials ) ;

        //****************************************************************************
        struct library_geometries : public so_dae::node
        {
            so_this_typedefs( library_geometries ) ;

            library_geometries( void_t ) : so_dae::node( "library_geometries" )
            {}

            library_geometries( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( library_geometries ) ;

        //****************************************************************************
        struct data : public so_dae::node
        {
            so_this_typedefs( data ) ;

            so_std::string_t the_data ;

            data( void_t ) : so_dae::node( "data" )
            {}

            data( this_rref_t rhv ) : so_dae::node( std::move( rhv ) )
            {
                so_move( the_data, rhv ) ;
            }

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                v->visit( this ) ;
                so_dae::node::apply( v ) ;
            }
        };
        so_typedef( data ) ;
    }
}

#endif