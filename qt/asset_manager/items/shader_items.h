//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_ASSET_MANAGER_SHADER_ITEMS_H_
#define _SNAKEOIL_QT_ASSET_MANAGER_SHADER_ITEMS_H_

#include "../shader_types.h"

#include <QTreeWidgetItem>

#include <functional>

namespace so_qt
{
    ///
    class program_item : public QTreeWidgetItem
    {
    private:

        std::string _key ;
        program_type _type = program_type::invalid ;

    public:

        program_item( QString const & display, std::string const & key, program_type type ) :
            QTreeWidgetItem( QStringList(display) ), _key(key), _type(type)
        {
            //this->setData( 0, Qt::UserRole, data ) ;
        }

        program_type program_type( void ) const { return _type ; }
        std::string const & get_key( void ) const { return _key ; }
    };

    ///
    class shader_item : public QTreeWidgetItem
    {
    private:

        std::string _key ;
        shader_type _type = shader_type::invalid ;

    public:

        shader_item( QString const & display, std::string const & key, shader_type type ) :
            QTreeWidgetItem( QStringList(display) ), _key(key), _type(type)
        {
            //this->setData( 0, Qt::UserRole, data ) ;
        }

        shader_type shader_type( void ) const { return _type ; }

        std::string const & get_key( void ) const { return _key ; }
    };
}

#endif
