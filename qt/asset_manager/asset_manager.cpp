//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "asset_manager.h"

#include "items/shader_items.h"
#include "items/script_items.h"

#include "../log.h"

#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QMenu>
#include <QLabel>

#include <string>
#include <iostream>
#include <algorithm>

using namespace so_qt ;

//***********************************************************************
asset_manager::asset_manager( QWidget * parent ) : QTreeWidget(parent)
{
    this->setColumnCount(1) ;

    // shader group
    {
        QTreeWidgetItem * parent = new QTreeWidgetItem( this, QStringList(QString("shaders")) ) ;
        _vertex_shader = new QTreeWidgetItem( parent, QStringList(QString("vertex")) ) ;
        _geometry_shader = new QTreeWidgetItem( parent, QStringList(QString("geometry")) ) ;
        _pixel_shader = new QTreeWidgetItem( parent, QStringList(QString("pixel")) ) ;
        _compute_shader = new QTreeWidgetItem( parent, QStringList(QString("compute")) ) ;
    }

    // render program 
    {
        QTreeWidgetItem * parent = new QTreeWidgetItem( this, QStringList(QString("programs")) ) ;
        _render_programs = new QTreeWidgetItem( parent, QStringList(QString("render")) ) ;
        _compute_programs = new QTreeWidgetItem( parent, QStringList(QString("compute")) ) ;
    }

    // other groups 
    {
    }
}

//***********************************************************************
asset_manager::~asset_manager( void ) 
{

}

//***********************************************************************
so_qt::result asset_manager::add_shader( shader_info const & info ) 
{
    if( info.type == so_qt::shader_type::vertex )
    {
        auto iter = _vertex_key_to_item.find( info.key ) ;
        if( iter != _vertex_key_to_item.end() )
        {
            so_qt::log::error( "[asset_manager] : vertex item already exists - " + info.key ) ;
            return so_qt::invalid_key ;
        }
        
        auto * ptr = new shader_item( info.display_name, info.key, so_qt::shader_type::vertex ) ;
        ptr->setIcon( 0, _shader_icons[so_qt::shader_type::vertex] ) ;
        _vertex_key_to_item[info.key] = ptr ;
        _vertex_shader->addChild( ptr ) ;

    }
    else if( info.type == so_qt::shader_type::geometry )
    {
        auto iter = _geometry_key_to_item.find( info.key ) ;
        if( iter != _geometry_key_to_item.end() ) 
        {
            so_qt::log::error( "[asset_manager] : geometry shader already exists - " + info.key ) ;
            return so_qt::invalid_key ;
        }
        
        auto * ptr = new shader_item( info.display_name, info.key, so_qt::shader_type::geometry ) ;
        ptr->setIcon( 0, _shader_icons[so_qt::shader_type::geometry] ) ;

        _geometry_key_to_item[info.key] = ptr ;
        _geometry_shader->addChild( ptr ) ;
    }
    else if( info.type == so_qt::shader_type::pixel )
    {
        auto iter = _pixel_key_to_item.find( info.key ) ;
        if( iter != _pixel_key_to_item.end() )
        {
            so_qt::log::error( "[asset_manager] : pixel shader already exists - " + info.key ) ;
            return so_qt::invalid_key ;
        }
        
        auto * ptr = new shader_item( info.display_name, info.key, so_qt::shader_type::pixel ) ;
        ptr->setIcon( 0, _shader_icons[so_qt::shader_type::pixel] ) ;

        _pixel_key_to_item[info.key] = ptr ;
        _pixel_shader->addChild( ptr ) ;
    }
    
    return so_qt::ok ;
}

//***********************************************************************
so_qt::result asset_manager::add_program( program_info const & info ) 
{
    if( info.type == so_qt::program_type::render )
    {
        auto iter = _render_program_datas.find( info.key ) ;
        if( iter != _render_program_datas.end() )
        {
            so_qt::log::error( "[asset_manager] : render program already in the list - " + info.key ) ;
            return so_qt::invalid_key ;
        }
        
        auto * ptr = new program_item( info.display_name, info.key, so_qt::program_type::render ) ;
        
        _render_program_datas[info.key] = {ptr} ;
        _render_programs->addChild( ptr ) ;
    }
    return so_qt::ok ;
}

//***********************************************************************
so_qt::result asset_manager::relate( shader_to_program_relate_info const & info ) 
{
    if( info.program_type == so_qt::program_type::render )
    {
        auto iter = _render_program_datas.find( info.program_key ) ;
        if( iter == _render_program_datas.end() )
        {
            so_qt::log::error( "[asset_manager] : render program does not exist - " + info.program_key ) ;
            return so_qt::invalid_key ;
        }
        if( info.shader_type == so_qt::shader_type::vertex )
        {
            auto sh_iter = _vertex_key_to_item.find( info.shader_key ) ;
            if( sh_iter == _vertex_key_to_item.end() )
            {
                so_qt::log::error( "[asset_manager] : shader does not exist - " + info.shader_key ) ;
                return so_qt::invalid_key ;
            }
            
            auto * ptr = new shader_item( sh_iter->second->text(0), 
                sh_iter->second->get_key(), so_qt::shader_type::vertex ) ;
            ptr->setIcon( 0, _shader_icons[so_qt::shader_type::vertex] ) ;

            iter->second.item->addChild( ptr ) ;
        }
        else if( info.shader_type == so_qt::shader_type::geometry )
        {
            auto sh_iter = _geometry_key_to_item.find( info.shader_key ) ;
            if( sh_iter == _geometry_key_to_item.end() )
            {
                so_qt::log::error( "[asset_manager] : shader does not exist - " + info.shader_key ) ;
                return so_qt::invalid_key ;
            }
            
            auto * ptr = new shader_item( sh_iter->second->text(0), 
                sh_iter->second->get_key(), so_qt::shader_type::geometry ) ;
            ptr->setIcon( 0, _shader_icons[so_qt::shader_type::geometry] ) ;

            iter->second.item->addChild( ptr ) ;
        }
        else if( info.shader_type == so_qt::shader_type::pixel )
        {
            auto sh_iter = _pixel_key_to_item.find( info.shader_key ) ;
            if( sh_iter == _pixel_key_to_item.end() )
            {
                so_qt::log::error( "[asset_manager] : shader does not exist - " + info.shader_key ) ;
                return so_qt::invalid_key ;
            }
            
            auto * ptr = new shader_item( sh_iter->second->text(0), 
                sh_iter->second->get_key(), so_qt::shader_type::pixel ) ;
            ptr->setIcon( 0, _shader_icons[so_qt::shader_type::pixel] ) ;

            iter->second.item->addChild( ptr ) ;
        }
    }

    return so_qt::ok ;
}

//***********************************************************************
so_qt::result asset_manager::add( context_menu_listener * listener ) 
{
    auto iter = std::find( _context_menu_listeners.begin(), _context_menu_listeners.end(), listener ) ;
    if( iter != _context_menu_listeners.end() )
    {
        so_qt::log::error( "[asset_manager] : listener already in the list." ) ;
        return so_qt::invalid_argument ;
    }
    _context_menu_listeners.push_back( listener ) ;
    return so_qt::ok ;
}

//***********************************************************************
so_qt::result asset_manager::set_icon( so_qt::shader_type type, QIcon const & icon ) 
{
    _shader_icons[ type ] = QIcon(icon) ;
    return so_qt::ok ;
}

//***********************************************************************
void asset_manager::mousePressEvent( QMouseEvent * event )
{
    if( event->button() == Qt::RightButton )
    {
        QTreeWidgetItem * item = this->itemAt( event->pos() ) ;
        if( item != nullptr )
        {
            if( dynamic_cast<shader_item*>(item) )
            {
                shader_item * sitem = static_cast<shader_item*>(item) ;
                QMenu menu ;
                menu.addAction("Recompile") ;
                menu.addAction("Show Code") ;
                QAction * chosen = menu.exec(event->globalPos()) ;
                if( chosen != nullptr )
                {
                    if( chosen->text() == QString("Recompile") )
                    {
                        for( auto l : _context_menu_listeners )
                        {
                            l->on_recompile( sitem->shader_type(), sitem->get_key() ) ;
                        }
                    }
                    else if( chosen->text() == QString("Show Code") )
                    {
                        std::string code ;
                        for( auto l : _context_menu_listeners )
                        {
                            code = std::move(l->on_show_code( sitem->shader_type(), sitem->get_key() )) ;
                            if( !code.empty() ) break ;
                        }
                        QLabel * label_ptr = new QLabel( QString(code.c_str()) ) ;
                        label_ptr->setWordWrap( true ) ;
                        label_ptr->show() ;
                        label_ptr->setAttribute( Qt::WA_DeleteOnClose ) ;
                        _open_code_text.push_back( label_ptr ) ;
                        {
                            QObject::connect( label_ptr, SIGNAL(destroyed(QObject*)),
                                this, SLOT(on_show_code_widget_destroyed(QObject*)) ) ;
                        }
                    }
                }
            }  
            else if( dynamic_cast<program_item*>(item) )
            {
                program_item * pitem = static_cast<program_item*>(item) ;                
                QMenu menu ;
                menu.addAction("Recompile") ;
                QAction * chosen = menu.exec(event->globalPos()) ;
                if( chosen != nullptr )
                {
                    if( chosen->text() == QString("Recompile") )
                    {
                        for( auto l : _context_menu_listeners )
                        {
                            l->on_recompile( pitem->program_type(), pitem->get_key() ) ;
                        }
                    }                    
                }
            }
        }
        event->accept() ;
    }
    QTreeWidget::mousePressEvent( event ) ;
}

//***********************************************************************
void asset_manager::on_show_code_widget_destroyed( QObject * obj ) 
{
    QWidget * wdg_ptr = (QWidget*) obj ;

    auto iter = std::find( _open_code_text.begin(), _open_code_text.end(), wdg_ptr ) ;
    if( iter == _open_code_text.end() )
    {
        // ups, should not happen
        // log
        return ;
    }

    // do something, maybe
    std::cout << "deleted" << std::endl ;

    _open_code_text.erase( iter ) ;
}
