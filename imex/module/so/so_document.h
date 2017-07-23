//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_SNAKEOIL_SNAKEOIL_DOCUMENT_H_
#define _SNAKEOIL_IMEX_MODULE_SNAKEOIL_SNAKEOIL_DOCUMENT_H_

namespace so_imex
{
    namespace so_jso
    {
        struct fragment
        {
            so_this_typedefs( fragment ) ;

            so_io::path_t uri ;


        public:

            fragment( this_cref_t rhv ) : uri(rhv.uri)
            {}

            fragment( this_rref_t rhv ) : uri( std::move(rhv.uri) )
            {}

        public:

            this_ref_t operator = ( this_cref_t rhv )
            {
                return *this = this_t( rhv ) ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                uri = std::move( rhv.uri ) ;                
                return *this ;
            }

        };
        so_typedef( fragment ) ;

        struct shader
        {
            so_this_typedefs( shader ) ;

            struct parameter_entry
            {
                so_std::string_t type ;
                so_std::string_t type_struct ;
                so_std::string_t binding_point ;
            };
            so_typedef( parameter_entry ) ;

        public:

            so_typedefs( so_std::vector<so_std::string_t>, fragment_ids ) ;

            typedef so_std::map< so_std::string_t, parameter_entry > __parameters_t ;
            so_typedefs( __parameters_t, parameters );
            
            so_std::string_t type ;
            fragment_ids_t fragment_ids ;
            parameters_t inputs ;
            parameters_t outputs ;
            parameters_t parameters ;

        public:

            shader( void_t ) {}
            shader( this_cref_t rhv ) : type( rhv.type ), fragment_ids( rhv.fragment_ids ),
                inputs( rhv.inputs ), outputs( rhv.outputs ),
                parameters( rhv.parameters )
            {}

            shader( this_rref_t rhv ) : type(rhv.type), fragment_ids(std::move(rhv.fragment_ids)),
                inputs(std::move(rhv.inputs)), outputs(std::move(rhv.outputs)), 
                parameters(std::move(rhv.parameters))
            {}
            ~shader( void_t ){}

        public:

            this_ref_t operator = (this_cref_t rhv)
            {
                return *this = this_t( rhv ) ;
            }

            this_ref_t operator = (this_rref_t rhv)
            {
                type = std::move( rhv.type ) ;
                fragment_ids = std::move( rhv.fragment_ids ) ;
                inputs = std::move( rhv.inputs ) ;
                outputs = std::move( rhv.outputs ) ;
                parameters = std::move( rhv.parameters ) ;
                return *this ;
            }
        };
        so_typedef( shader ) ;

        ///
        struct effect
        {
            so_this_typedefs( effect ) ;

            so_std::string_t vertex_shader ;
            so_std::string_t geometry_shader ;
            so_std::string_t pixel_shader ;

            effect( void_t ) {}
            effect( this_cref_t rhv ) : vertex_shader( rhv.vertex_shader ),
                geometry_shader( rhv.geometry_shader), pixel_shader( rhv.pixel_shader )
            {}

            effect( this_rref_t rhv ) : vertex_shader(std::move(rhv.vertex_shader)),
                geometry_shader(std::move(rhv.geometry_shader)), pixel_shader(std::move(rhv.pixel_shader))
            {}

        public:

            this_ref_t operator = ( this_cref_t rhv )
            {
                return *this = this_t( rhv ) ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                vertex_shader = std::move( rhv.vertex_shader ) ;
                geometry_shader = std::move( rhv.geometry_shader ) ;
                pixel_shader = std::move( rhv.pixel_shader ) ;
                return *this ;
            }
        };
        so_typedef( effect ) ;

        ///
        struct render_config
        {
            so_this_typedefs( render_config ) ;

        public: // variable_set

            struct variable_set
            {
                // tbd
            };
            so_typedef( variable_set ) ;
            
            typedef so_std::map< so_std::string_t, variable_set > __variable_sets_t ;
            so_typedefs( __variable_sets_t, variable_sets ) ;

        public: // render_config

            struct renderstate_set
            {
                // tbd
            };
            so_typedef( renderstate_set ) ;
            typedef so_std::map< so_std::string_t, renderstate_set_t > __renderstate_sets_t ;
            so_typedefs( __renderstate_sets_t, renderstate_sets ) ;

        public: // members

            so_std::string_t effect_name ;
            so_std::string_t geometry_name ;
            variable_sets_t variable_sets ;
            renderstate_sets_t renderstate_sets ;
        };
        so_typedef( render_config ) ;

        ///
        struct assets
        {
            so_this_typedefs( assets ) ;

            typedef so_std::map< so_std::string_t, so_io::path_t > __paths_t ;
            so_typedefs( __paths_t, paths ) ;
            
            paths_t sources ;
            paths_t images ;
            paths_t geos ;
            paths_t scenes ;

            assets( void_t ) {}
            assets( this_cref_t rhv ) : sources( rhv.sources ), images( rhv.images ),
                geos( rhv.geos ), scenes( rhv.scenes )
            {}

            assets( this_rref_t rhv ) : sources(std::move(rhv.sources)), images(std::move(rhv.images)),
                geos(std::move(rhv.geos)), scenes(std::move(rhv.scenes))
            {}
            ~assets( void_t ){}

        public:

            this_ref_t operator = ( this_cref_t rhv )
            {
                return *this = this_t(rhv) ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                sources = std::move( rhv.sources ) ;
                images = std::move( rhv.images ) ;
                geos = std::move( rhv.geos ) ;
                scenes = std::move( rhv.scenes ) ;
                return *this ;
            }
        };
        so_typedef( assets ) ;

        ///
        struct objects
        {
            so_this_typedefs( objects ) ;

            typedef so_std::map< so_std::string_t, shader_t > __shaders_t ;
            so_typedefs( __shaders_t, shaders ) ;

            typedef so_std::map< so_std::string_t, effect_t > __effects_t ;
            so_typedefs( __effects_t, effects ) ;

            typedef so_std::map< so_std::string_t, render_config_t > __render_configs_t ;
            so_typedefs( __render_configs_t, render_configs ) ;

            shaders_t shaders ;
            effects_t effects ;
            render_configs_t render_configs ;


            objects( void_t ) {}
            objects( this_cref_t rhv ) : shaders( rhv.shaders ), effects( rhv.effects ), 
                render_configs(rhv.render_configs)
            {}
            objects( this_rref_t rhv ) : shaders(std::move(rhv.shaders)), effects(std::move(rhv.effects)),
                render_configs(std::move(rhv.render_configs))
            {}
            ~objects( void_t ) {}

        public:

            this_ref_t operator = ( this_cref_t rhv )
            {
                return *this = this_t(rhv) ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                return *this = this_t( std::move(rhv) ) ;
            }

        };
        so_typedef( objects ) ;

        struct scene
        {
            struct node
            {
                so_std::string_t type ;

            };
        };

        ///
        struct document
        {
            so_this_typedefs( document ) ;

            so_typedefs( so_std::vector<so_io::path_t>, paths ) ;

            paths_t files_data ;
            assets_t assets_data ;
            objects_t objects_data ;

            document( void_t ) {}
            document( this_cref_t rhv ) : files_data( rhv.files_data ),
                assets_data( rhv.assets_data ), objects_data( rhv.objects_data )
            {}

            document( this_rref_t rhv ) : files_data(std::move(rhv.files_data)),
                assets_data(std::move(rhv.assets_data)), objects_data(std::move(rhv.objects_data))
            {}

            ~document( void_t ) {}


        public:

            this_ref_t operator = ( this_cref_t rhv )
            {                
                return *this = this_t(rhv) ;
            }

            this_ref_t operator = ( this_rref_t rhv )
            {
                files_data = std::move(rhv.files_data) ;
                assets_data = std::move(rhv.assets_data) ;
                objects_data = std::move(rhv.objects_data) ;
                return *this ;
            }

        };
        so_typedef( document ) ;
    };    
}

#endif
