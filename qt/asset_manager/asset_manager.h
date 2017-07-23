//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_ASSET_MANAGER_H_
#define _SNAKEOIL_QT_ASSET_MANAGER_H_

#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"

#include "items/shader_items.h"

#include <QTreeWidget>

#include <list>
#include <map>

namespace so_qt
{
    class asset_manager : public QTreeWidget
    {
        Q_OBJECT

    public:

        struct context_menu_listener 
        {
            virtual void on_recompile( shader_type, std::string const & ) = 0 ;
            virtual void on_recompile( program_type, std::string const & ) = 0 ;

            virtual std::string on_show_code( shader_type, std::string const & ) = 0 ;

        };

    public:

        typedef std::string key_t ;

        typedef std::list< context_menu_listener * > context_menu_listeners_t ;

        typedef std::list< QTreeWidgetItem * > tree_widget_items_t ;
        typedef std::list< std::string > string_list_t ;

        typedef std::map< std::string, shader_item * > key_to_shader_item_t ;
        typedef std::map< shader_type, QIcon > shader_to_icon_t ;

    public:

        struct shader_info
        {
            so_qt::shader_type type ;
            std::string key ;

            QString display_name ;
        };

        struct program_info
        {
            so_qt::program_type type ;
            std::string key ;

            QString display_name ;
        };

        struct shader_to_program_relate_info
        {
            so_qt::shader_type shader_type ;
            so_qt::program_type program_type ;
            std::string shader_key ;
            std::string program_key ;
        };

    private:

        QTreeWidgetItem * _vertex_shader = nullptr ;
        QTreeWidgetItem * _geometry_shader = nullptr ;
        QTreeWidgetItem * _pixel_shader = nullptr ;
        QTreeWidgetItem * _compute_shader = nullptr ;
        QTreeWidgetItem * _render_programs = nullptr ;
        QTreeWidgetItem * _compute_programs = nullptr ;

        context_menu_listeners_t _context_menu_listeners ;

    private:

        struct render_program_data
        {
            program_item * item ;
            string_list_t vertex ;
            string_list_t geometry ;
            string_list_t pixel ;
        };

        typedef std::map< std::string, render_program_data > render_program_key_to_data_t ;

        key_to_shader_item_t _vertex_key_to_item ;
        key_to_shader_item_t _geometry_key_to_item ;
        key_to_shader_item_t _pixel_key_to_item ;

        render_program_key_to_data_t _render_program_datas ;

        shader_to_icon_t _shader_icons ;

        std::list< QWidget * > _open_code_text ;

    public:

        asset_manager( QWidget * parent=nullptr ) ;
        virtual ~asset_manager( void ) ;

    public:

        so_qt::result add_shader( shader_info const & info ) ;
        so_qt::result add_program( program_info const & info ) ;

        so_qt::result relate( shader_to_program_relate_info const & info ) ;

    public:

        so_qt::result add( context_menu_listener * listener ) ;

    public:

        so_qt::result set_icon( so_qt::shader_type, QIcon const & icon ) ;

    protected:

        virtual void mousePressEvent( QMouseEvent * event ) ;

    protected slots:
        
        void on_show_code_widget_destroyed( QObject * ) ;
        
    private:


        
    };
}

#endif
