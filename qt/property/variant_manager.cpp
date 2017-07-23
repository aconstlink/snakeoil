//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "variant_manager.h"

#include <QObject>

using namespace so_qt ;


//***********************************************************************
int variant_manager::vec2ui_type_id( void ) 
{
    return qMetaTypeId<so_math::vec2ui_t>() ;
}

//***********************************************************************
int variant_manager::vec2i_type_id( void ) 
{
    return qMetaTypeId<so_math::vec2i_t>() ;
}

//***********************************************************************
int variant_manager::vec2f_type_id( void )
{
    return qMetaTypeId<so_math::vec2f_t>() ;
}

//***********************************************************************
int variant_manager::vec3ui_type_id( void ) 
{
    return qMetaTypeId<so_math::vec3ui_t>() ;
}

//***********************************************************************
int variant_manager::vec3i_type_id( void ) 
{
    return qMetaTypeId<so_math::vec3i_t>() ;
}

//***********************************************************************
int variant_manager::vec3f_type_id( void ) 
{
    return qMetaTypeId<so_math::vec3f_t>() ;
}

//***********************************************************************
int variant_manager::vec4ui_type_id( void ) 
{
    return qMetaTypeId<so_math::vec4ui_t>() ;
}

//***********************************************************************
int variant_manager::vec4i_type_id( void )
{
    return qMetaTypeId<so_math::vec4i_t>() ;
}

//***********************************************************************
int variant_manager::vec4f_type_id( void ) 
{
    return qMetaTypeId<so_math::vec4f_t>() ;
}

//***********************************************************************
bool variant_manager::isPropertyTypeSupported( int propertyType ) const 
{
    if ( propertyType == vec2ui_type_id() ) return true ;
    if ( propertyType == vec2i_type_id() ) return true ;
    if ( propertyType == vec2f_type_id() ) return true ;
    if ( propertyType == vec3ui_type_id() ) return true ;
    if ( propertyType == vec3i_type_id() ) return true ;
    if ( propertyType == vec3f_type_id() ) return true ;
    if ( propertyType == vec4ui_type_id() ) return true ;
    if ( propertyType == vec4i_type_id() ) return true ;
    if ( propertyType == vec4f_type_id() ) return true ;

    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

//***********************************************************************
variant_manager::variant_manager( QObject * parent ) : QtVariantPropertyManager( parent )
{
    connect(this, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty *)),
                this, SLOT(slotPropertyDestroyed(QtProperty *)));
}

//***********************************************************************
variant_manager::~variant_manager( void ) 
{

}

//***********************************************************************
QVariant variant_manager::value( QtProperty const * prop ) const 
{
    auto iter = _property_to_data.find( prop ) ;
    if( iter != _property_to_data.end() )
    {
        return iter->second.value ;
    }
    return QtVariantPropertyManager::value( prop ) ;
}

//***********************************************************************
int variant_manager::valueType( int propertyType ) const 
{
    if ( propertyType == vec2ui_type_id() ) return vec2ui_type_id() ;
    if ( propertyType == vec2i_type_id() ) return vec2i_type_id() ;
    if ( propertyType == vec2f_type_id() ) return vec2f_type_id() ;

    if ( propertyType == vec3ui_type_id() ) return vec3ui_type_id() ;
    if ( propertyType == vec3i_type_id() ) return vec3i_type_id() ;
    if ( propertyType == vec3f_type_id() ) return vec3f_type_id() ;

    if ( propertyType == vec4ui_type_id() ) return vec4ui_type_id() ;
    if ( propertyType == vec4i_type_id() ) return vec4i_type_id() ;
    if ( propertyType == vec4f_type_id() ) return vec4f_type_id() ;

    return QtVariantPropertyManager::valueType( propertyType ) ;
}

//***********************************************************************
void variant_manager::setValue( QtProperty * property, const QVariant & val ) 
{
    auto iter = _property_to_data.find( property ) ;
    if( iter == _property_to_data.end() )
    {
        return QtVariantPropertyManager::setValue( property, val ) ;
    }

    data_item & item = iter->second ;
    item.value = val ;

    if( setValue_vec2ui( property, val, item ) ) return ;
    if( setValue_vec2i( property, val, item ) ) return ;
    if( setValue_vec2f( property, val, item ) ) return ;

    if( setValue_vec3ui( property, val, item ) ) return ;
    if( setValue_vec3i( property, val, item ) ) return ;
    if( setValue_vec3f( property, val, item ) ) return ;

    if( setValue_vec4ui( property, val, item ) ) return ;
    if( setValue_vec4i( property, val, item ) ) return ;
    if( setValue_vec4f( property, val, item ) ) return ;

}

//***********************************************************************
void variant_manager::initializeProperty( QtProperty * property ) 
{
    if( initialize_vec2ui(property) ) return ;
    if( initialize_vec2i(property) ) return ;
    if( initialize_vec2f(property) ) return ;

    if( initialize_vec3ui(property) ) return ;
    if( initialize_vec3i(property) ) return ;
    if( initialize_vec3f(property) ) return ;

    if( initialize_vec4ui(property) ) return ;
    if( initialize_vec4i(property) ) return ;
    if( initialize_vec4f(property) ) return ;

    QtVariantPropertyManager::initializeProperty(property);
}

//***********************************************************************
void variant_manager::uninitializeProperty( QtProperty * property )
{
    auto iter = _property_to_data.find( property ) ;
    if( iter == _property_to_data.end() ) 
        return QtVariantPropertyManager::uninitializeProperty( property ) ;
        
    data_item di = iter->second ;
    if( di.x != nullptr ) 
    {
        _x_to_property.erase( di.x ) ;
        _sub_to_group.erase( di.x ) ;
    }
    if( di.y != nullptr ) 
    {
        _y_to_property.erase( di.y ) ;
        _sub_to_group.erase( di.y ) ;
    }
    if( di.z != nullptr ) 
    {
        _z_to_property.erase( di.z ) ;
        _sub_to_group.erase( di.z ) ;
    }
    if( di.w != nullptr ) 
    {
        _w_to_property.erase( di.w ) ;
        _sub_to_group.erase( di.w ) ;
    }

    _property_to_data.erase( iter ) ;
}

//***********************************************************************
void variant_manager::slotValueChanged( QtProperty * property, const QVariant & value ) 
{
    if( slotValueChanged_vec2ui( property, value ) ) return ;
    if( slotValueChanged_vec2i( property, value ) ) return ;
    if( slotValueChanged_vec2f( property, value ) ) return ;

    if( slotValueChanged_vec3ui( property, value ) ) return ;
    if( slotValueChanged_vec3i( property, value ) ) return ;
    if( slotValueChanged_vec3f( property, value ) ) return ;

    if( slotValueChanged_vec4ui( property, value ) ) return ;
    if( slotValueChanged_vec4i( property, value ) ) return ;
    if( slotValueChanged_vec4f( property, value ) ) return ;
}

//***********************************************************************
void variant_manager::slotPropertyDestroyed( QtProperty * property )
{
    {
        auto iter = _x_to_property.find( property );
        if( iter != _x_to_property.end() )
        {
            _property_to_data[iter->second].x = nullptr ;
            _x_to_property.erase(iter) ;
            return ;
        }
    }
    {
        auto iter = _y_to_property.find( property );
        if( iter != _y_to_property.end() )
        {
            _property_to_data[iter->second].y = nullptr ;
            _y_to_property.erase(iter) ;
            return ;
        }
    }
    {
        auto iter = _z_to_property.find( property );
        if( iter != _z_to_property.end() )
        {
            _property_to_data[iter->second].z = nullptr ;
            _z_to_property.erase(iter) ;
            return ;
        }
    }
    {
        auto iter = _w_to_property.find( property );
        if( iter != _w_to_property.end() )
        {
            _property_to_data[iter->second].w = nullptr ;
            _w_to_property.erase(iter) ;
            return ;
        }
    }
}

//***********************************************************************        
QString variant_manager::valueText( QtProperty const * property ) const
{
    auto iter = _property_to_data.find( property ) ;
    if( iter == _property_to_data.end() ) return QtVariantPropertyManager::valueText(property) ;

    QString str ;

    if( valueText_vec2ui( property, iter->second, str ) ) return str ;
    if( valueText_vec2i( property, iter->second, str ) ) return str ;
    if( valueText_vec2f( property, iter->second, str ) ) return str ;

    if( valueText_vec3ui( property, iter->second, str ) ) return str ;
    if( valueText_vec3i( property, iter->second, str ) ) return str ;
    if( valueText_vec3f( property, iter->second, str ) ) return str ;

    if( valueText_vec4ui( property, iter->second, str ) ) return str ;
    if( valueText_vec4i( property, iter->second, str ) ) return str ;
    if( valueText_vec4f( property, iter->second, str ) ) return str ;
    
    return str ;
}

//***********************************************************************
bool variant_manager::initialize_vec2ui( QtProperty * property )
{
    if( propertyType(property) != vec2ui_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec2ui_t() ) ;

    this_t::addStepProperty( QVariant::Int, QVariant(1), property, di ) ;
    this_t::addXProperty( QVariant::Int, property, di ) ;
    this_t::addYProperty( QVariant::Int, property, di ) ;

    _property_to_data[property] = di ;

    return false ;
}

//***********************************************************************
bool variant_manager::initialize_vec2i( QtProperty * property ) 
{
    if( propertyType(property) != vec2i_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec2i_t() ) ;

    this_t::addStepProperty( QVariant::Int, QVariant(1), property, di ) ;
    this_t::addXProperty( QVariant::Int, property, di ) ;
    this_t::addYProperty( QVariant::Int, property, di ) ;

    _property_to_data[property] = di ;

    return false ;
}

//***********************************************************************
bool variant_manager::initialize_vec2f( QtProperty * property ) 
{
    if( propertyType(property) != vec2f_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec2f_t() ) ;

    this_t::addStepProperty( QVariant::Double, QVariant(0.1f), property, di ) ;
    this_t::addXProperty( QVariant::Double, property, di ) ;
    this_t::addYProperty( QVariant::Double, property, di ) ;

    _property_to_data[property] = di ;

    return false ;
}

//***********************************************************************
bool variant_manager::initialize_vec3ui( QtProperty * property )
{
    if( propertyType(property) != vec3ui_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec3ui_t() ) ;

    this_t::addStepProperty( QVariant::Int, QVariant(1), property, di ) ;
    this_t::addXProperty( QVariant::Int, property, di ) ;
    this_t::addYProperty( QVariant::Int, property, di ) ;
    this_t::addZProperty( QVariant::Int, property, di ) ;

    _property_to_data[property] = di ;

    return false ;
}

//***********************************************************************
bool variant_manager::initialize_vec3i( QtProperty * property ) 
{
    if( propertyType(property) != vec3i_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec3i_t() ) ;

    this_t::addStepProperty( QVariant::Int, QVariant(1), property, di ) ;
    this_t::addXProperty( QVariant::Int, property, di ) ;
    this_t::addYProperty( QVariant::Int, property, di ) ;
    this_t::addZProperty( QVariant::Int, property, di ) ;

    _property_to_data[property] = di ;

    return true ;
}

//***********************************************************************
bool variant_manager::initialize_vec3f( QtProperty * property ) 
{
    if( propertyType(property) != vec3f_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec3f_t() ) ;
    
    this_t::addStepProperty( QVariant::Double, QVariant(0.1f), property, di ) ;
    this_t::addXProperty( QVariant::Double, property, di ) ;
    this_t::addYProperty( QVariant::Double, property, di ) ;
    this_t::addZProperty( QVariant::Double, property, di ) ;

    _property_to_data[property] = di ;

    return true ;
}

//***********************************************************************
bool variant_manager::initialize_vec4ui( QtProperty * property )
{
    if( propertyType(property) != vec4ui_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec4ui_t() ) ;

    this_t::addStepProperty( QVariant::Int, QVariant(1), property, di ) ;
    this_t::addXProperty( QVariant::Int, property, di ) ;
    this_t::addYProperty( QVariant::Int, property, di ) ;
    this_t::addZProperty( QVariant::Int, property, di ) ;
    this_t::addWProperty( QVariant::Int, property, di ) ;

    _property_to_data[property] = di ;

    return true ;
}

//***********************************************************************
bool variant_manager::initialize_vec4i( QtProperty * property ) 
{
    if( propertyType(property) != vec4i_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec4i_t() ) ;

    this_t::addStepProperty( QVariant::Int, QVariant(1), property, di ) ;
    this_t::addXProperty( QVariant::Int, property, di ) ;
    this_t::addYProperty( QVariant::Int, property, di ) ;
    this_t::addZProperty( QVariant::Int, property, di ) ;
    this_t::addWProperty( QVariant::Int, property, di ) ;

    _property_to_data[property] = di ;

    return true ;
}

//***********************************************************************
bool variant_manager::initialize_vec4f( QtProperty * property ) 
{
    if( propertyType(property) != vec4f_type_id() ) return false ;

    data_item di ;
    di.value = QVariant::fromValue( so_math::vec4f_t() ) ;

    this_t::addStepProperty( QVariant::Double, QVariant(0.1f), property, di ) ;
    this_t::addXProperty( QVariant::Double, property, di ) ;
    this_t::addYProperty( QVariant::Double, property, di ) ;
    this_t::addZProperty( QVariant::Double, property, di ) ;
    this_t::addWProperty( QVariant::Double, property, di ) ;

    _property_to_data[property] = di ;

    return true ;
}

//***********************************************************************
void variant_manager::addStepProperty( int type, QVariant default_value, QtProperty * parent, data_item & di ) 
{
    QtVariantProperty * p = this->addProperty(type, "step" );
    p->setValue( default_value ) ;
    p->setAttribute( "singleStep", default_value ) ;

    parent->addSubProperty(p);
    _sub_to_group[p] = parent ;
    _step_to_property[p] = parent ;

    di.step = p ;
}

//***********************************************************************
void variant_manager::addXProperty( int type, QtProperty * parent, data_item & di ) 
{    
    di.x = this->addProperty( type, "x" ) ;
    di.x->setAttribute( "singleStep", QVariant( di.step->value() ) ) ;
    parent->addSubProperty(di.x);
    _x_to_property[di.x] = parent;
    _sub_to_group[di.x] = parent ;    
}

//***********************************************************************
void variant_manager::addYProperty( int type, QtProperty * parent, data_item & di ) 
{
    di.y = this->addProperty( type, "y" ) ;
    di.y->setAttribute( "singleStep", QVariant(di.step->value()) ) ;
    parent->addSubProperty(di.y);
    _y_to_property[di.y] = parent;
    _sub_to_group[di.y] = parent ;
}

//***********************************************************************
void variant_manager::addZProperty( int type, QtProperty * parent, data_item & di ) 
{
    di.z = this->addProperty( type, "z" ) ;
    di.z->setAttribute( "singleStep", QVariant(di.step->value()) ) ;
    parent->addSubProperty(di.z);
    _z_to_property[di.z] = parent;
    _sub_to_group[di.z] = parent ;
}

void variant_manager::addWProperty( int type, QtProperty * parent, data_item & di ) 
{
    di.w = this->addProperty(type, "w" ) ;
    di.w->setAttribute( "singleStep", QVariant(di.step->value()) ) ;
    parent->addSubProperty(di.w);
    _w_to_property[di.w] = parent;
    _sub_to_group[di.w] = parent ;
}

//***********************************************************************
bool variant_manager::setValue_vec2ui( QtProperty * property, const QVariant & val, data_item const & item )
{
    if( propertyType(property) != vec2ui_type_id() ) return false ;

    so_math::vec2ui_t vec = val.value<so_math::vec2ui_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;    
}

//***********************************************************************
bool variant_manager::setValue_vec2i( QtProperty * property, const QVariant & val, data_item const & item ) 
{
    if( propertyType(property) != vec2i_type_id() ) return false ;

    so_math::vec2i_t vec = val.value<so_math::vec2i_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;  
}

//***********************************************************************
bool variant_manager::setValue_vec2f( QtProperty * property, const QVariant & val, data_item const & item ) 
{
    if( propertyType(property) != vec2f_type_id() ) return false ;

    so_math::vec2f_t vec = val.value<so_math::vec2f_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;  
}

//***********************************************************************
bool variant_manager::setValue_vec3ui( QtProperty * property, const QVariant & val, data_item const & item )
{    
    if( propertyType(property) != vec3ui_type_id() ) return false ;

    so_math::vec3ui_t vec = val.value<so_math::vec3ui_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;
    if( item.z != nullptr ) item.z->setValue( vec.z() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;    
}

//***********************************************************************
bool variant_manager::setValue_vec3i( QtProperty * property, const QVariant & val, data_item const & item )
{
    if( propertyType(property) != vec3i_type_id() ) return false ;

    so_math::vec3ui_t vec = val.value<so_math::vec3i_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;
    if( item.z != nullptr ) item.z->setValue( vec.z() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;
}

//***********************************************************************
bool variant_manager::setValue_vec3f( QtProperty * property, const QVariant & val, data_item const & item )
{
    if( propertyType(property) != vec3f_type_id() ) return false ;

    so_math::vec3f_t vec = val.value<so_math::vec3f_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;
    if( item.z != nullptr ) item.z->setValue( vec.z() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;
}

//***********************************************************************
bool variant_manager::setValue_vec4ui( QtProperty * property, const QVariant & val, data_item const & item ) 
{
    if( propertyType(property) != vec4ui_type_id() ) return false ;

    auto vec = val.value<so_math::vec4ui_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;
    if( item.z != nullptr ) item.z->setValue( vec.z() ) ;
    if( item.w != nullptr ) item.w->setValue( vec.w() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;
}

//***********************************************************************
bool variant_manager::setValue_vec4i( QtProperty * property, const QVariant & val, data_item const & item ) 
{
    if( propertyType(property) != vec4i_type_id() ) return false ;

    auto vec = val.value<so_math::vec4i_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;
    if( item.z != nullptr ) item.z->setValue( vec.z() ) ;
    if( item.w != nullptr ) item.w->setValue( vec.w() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;
}

//***********************************************************************
bool variant_manager::setValue_vec4f( QtProperty * property, const QVariant & val, data_item const & item ) 
{
    if( propertyType(property) != vec4f_type_id() ) return false ;

    auto vec = val.value<so_math::vec4f_t>() ;
    if( item.x != nullptr ) item.x->setValue( vec.x() ) ;
    if( item.y != nullptr ) item.y->setValue( vec.y() ) ;
    if( item.z != nullptr ) item.z->setValue( vec.z() ) ;
    if( item.w != nullptr ) item.w->setValue( vec.w() ) ;

    emit propertyChanged( property );
    emit valueChanged( property, QVariant::fromValue(vec) );

    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec2ui( QtProperty const * property, data_item const & item, QString & str_out ) const
{
    if( propertyType(property) != vec2ui_type_id() ) return false ;
    
    QVariant v = item.value ;
    auto vec = v.value<so_math::vec2ui_t>() ;
    str_out = std::move( QString("(%1 : %2)").arg(vec.x()).arg(vec.y()) ) ;
    
    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec2i( QtProperty const * property, data_item const & item, QString & str_out ) const
{
    if( propertyType(property) != vec2i_type_id() ) return false ;
    
    QVariant v = item.value ;
    auto vec = v.value<so_math::vec2i_t>() ;
    str_out = std::move( QString("(%1 : %2)").arg(vec.x()).arg(vec.y()) ) ;
    
    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec2f( QtProperty const * property, data_item const & item, QString & str_out ) const
{
    if( propertyType(property) != vec2f_type_id() ) return false ;
    
    QVariant v = item.value ;
    auto vec = v.value<so_math::vec2f_t>() ;
    str_out = std::move( QString("(%1 : %2)").arg(vec.x()).arg(vec.y()) ) ;
    
    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec3ui( QtProperty const * property, data_item const & item, QString & str_out ) const
{
    if( propertyType(property) != vec3ui_type_id() ) return false ;
    
    QVariant v = item.value ;
    auto vec = v.value<so_math::vec3ui_t>() ;
    str_out = std::move(QString("(%1 : %2 : %3)").arg(vec.x()).arg(vec.y()).arg(vec.z())) ;
    
    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec3i( QtProperty const * property, data_item const & item, QString & str_out ) const
{
    if( propertyType(property) != vec3i_type_id() ) return false ;

    QVariant v = item.value ;
    auto vec = v.value<so_math::vec3i_t>() ;
    str_out = std::move(QString("(%1 : %2 : %3)").arg(vec.x()).arg(vec.y()).arg(vec.z())) ;
    
    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec3f( QtProperty const * property, data_item const & item, QString & str_out ) const
{
    if( propertyType(property) != vec3f_type_id() ) return false ;

    QVariant v = item.value ;
    auto vec = v.value<so_math::vec3f_t>() ;
    str_out = std::move( QString("(%1 : %2 : %3)").arg(vec.x()).arg(vec.y()).arg(vec.z()) );
    
    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec4ui( QtProperty const * property, data_item const & item, QString & str_out ) const 
{
    if( propertyType(property) != vec4ui_type_id() ) return false ;

    QVariant v = item.value ;
    auto vec = v.value<so_math::vec4ui_t>() ;
    str_out = std::move( QString("(%1 : %2 : %3 : %4)").arg(vec.x()).arg(vec.y()).arg(vec.z()).arg(vec.w()) );
    
    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec4i( QtProperty const * property, data_item const & item, QString & str_out ) const 
{
    if( propertyType(property) != vec4i_type_id() ) return false ;

    QVariant v = item.value ;
    auto vec = v.value<so_math::vec4i_t>() ;
    str_out = std::move( QString("(%1 : %2 : %3 : %4)").arg(vec.x()).arg(vec.y()).arg(vec.z()).arg(vec.w()) );
    
    return true ;
}

//***********************************************************************
bool variant_manager::valueText_vec4f( QtProperty const * property, data_item const & item, QString & str_out ) const
{
    if( propertyType(property) != vec4f_type_id() ) return false ;

    QVariant v = item.value ;
    auto vec = v.value<so_math::vec4f_t>() ;
    str_out = std::move( QString("(%1 : %2 : %3 : %4)").arg(vec.x()).arg(vec.y()).arg(vec.z()).arg(vec.w()) );
    
    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec2ui( QtProperty const * property, QVariant const & value) 
{
    typedef so_math::vec2ui_t vec_t ;
    typedef unsigned int type_t ;

    type_funk_t type_funk = vec2ui_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    int step = 0.0f ;
    if( this_t::isStepProperty_int( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec2i( QtProperty const * property, QVariant const & value ) 
{
    typedef so_math::vec2i_t vec_t ;
    typedef int type_t ;

    type_funk_t type_funk = vec2i_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    int step = 0.0f ;
    if( this_t::isStepProperty_int( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec2f( QtProperty const * property, QVariant const & value ) 
{
    typedef so_math::vec2f_t vec_t ;
    typedef float type_t ;

    type_funk_t type_funk = vec2f_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    float step = 0.0f ;
    if( this_t::isStepProperty_float( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec3ui( QtProperty const * property, QVariant const & value ) 
{
    typedef so_math::vec3ui_t vec_t ;
    typedef unsigned int type_t ;

    type_funk_t type_funk = vec3ui_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    int step = 0.0f ;
    if( this_t::isStepProperty_int( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
        di.z->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else if( this_t::isZProperty(property) ) vec.z( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec3i( QtProperty const * property, QVariant const & value )
{
    typedef so_math::vec3i_t vec_t ;
    typedef int type_t ;

    type_funk_t type_funk = vec3i_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    int step = 0.0f ;
    if( this_t::isStepProperty_int( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
        di.z->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else if( this_t::isZProperty(property) ) vec.z( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec3f( QtProperty const * property, QVariant const & value )
{
    typedef so_math::vec3f_t vec_t ;
    typedef float type_t ;

    type_funk_t type_funk = vec3f_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    float step = 0.0f ;
    if( this_t::isStepProperty_float( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
        di.z->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else if( this_t::isZProperty(property) ) vec.z( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec4ui( QtProperty const * property, QVariant const & value) 
{
    typedef so_math::vec4ui_t vec_t ;
    typedef unsigned int type_t ;

    type_funk_t type_funk = vec4ui_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    int step = 0.0f ;
    if( this_t::isStepProperty_int( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
        di.z->setAttribute( "singleStep", step ) ;
        di.w->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else if( this_t::isZProperty(property) ) vec.z( value.value<type_t>() ) ;
    else if( this_t::isWProperty(property) ) vec.w( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec4i( QtProperty const * property, QVariant const & value )
{
    typedef so_math::vec4i_t vec_t ;
    typedef int type_t ;

    type_funk_t type_funk = vec4i_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    int step = 0.0f ;
    if( this_t::isStepProperty_int( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
        di.z->setAttribute( "singleStep", step ) ;
        di.w->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else if( this_t::isZProperty(property) ) vec.z( value.value<type_t>() ) ;
    else if( this_t::isWProperty(property) ) vec.w( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::slotValueChanged_vec4f( QtProperty const * property, QVariant const & value )
{
    typedef so_math::vec4f_t vec_t ;
    typedef float type_t ;

    type_funk_t type_funk = vec4f_type_id ;

    QtProperty * parent_prop = nullptr;
    if( !this_t::isParentOfType( property, type_funk, parent_prop ) ) return false ;
    
    data_item di = _property_to_data[parent_prop] ;
    vec_t vec = di.value.value<vec_t>() ;
    
    float step = 0.0f ;
    if( this_t::isStepProperty_float( property, value, step ) )
    {
        di.x->setAttribute( "singleStep", step ) ;
        di.y->setAttribute( "singleStep", step ) ;
        di.z->setAttribute( "singleStep", step ) ;
        di.w->setAttribute( "singleStep", step ) ;
    }    
    else if( this_t::isXProperty(property) ) vec.x( value.value<type_t>() ) ;      
    else if( this_t::isYProperty(property) ) vec.y( value.value<type_t>() ) ;
    else if( this_t::isZProperty(property) ) vec.z( value.value<type_t>() ) ;
    else if( this_t::isWProperty(property) ) vec.w( value.value<type_t>() ) ;
    else return false ;

    setValue( parent_prop, QVariant::fromValue(vec) ) ;

    return true ;
}

//***********************************************************************
bool variant_manager::isStepProperty_float( QtProperty const * property, QVariant const & value, float & step_out ) 
{
    auto iter = _step_to_property.find( property ) ;
    if( iter == _step_to_property.end() ) return false ;

    auto & di = _property_to_data[iter->second] ;

    // negative value not possible to be used
    // as the step value.
    float step = value.value<float>() ;
    if( step < 0.0f )
    {
        step = -step ;
        di.step->setValue( step ) ;
    }

    step_out = step ;
        
    return true ;
}

//***********************************************************************
bool variant_manager::isStepProperty_int( QtProperty const * property, QVariant const & value, int & step_out ) 
{
    auto iter = _step_to_property.find( property ) ;
    if( iter == _step_to_property.end() ) return false ;

    auto & di = _property_to_data[iter->second] ;

    // negative value not possible to be used
    // as the step value.
    int step = value.value<int>() ;
    if( step < 0.0f )
    {
        step = -step ;
        di.step->setValue( step ) ;
    }

    step_out = step ;
        
    return true ;
}

//***********************************************************************
bool variant_manager::isParentOfType( QtProperty const * sub, std::function<int(void)> funk, QtProperty *& parent ) 
{
    // figure out the type by its parent.
    auto iter = _sub_to_group.find( sub ) ;
    if( iter == _sub_to_group.end() ) return false ;

    if( propertyType(iter->second) != funk() ) return false;

    parent = iter->second ;
    return true ;
}

//***********************************************************************
bool variant_manager::isXProperty( QtProperty const * property ) const 
{
    return _x_to_property.find( property ) != _x_to_property.end() ;
}

//***********************************************************************
bool variant_manager::isYProperty( QtProperty const * property ) const
{
    return _y_to_property.find( property ) != _y_to_property.end() ;
}

//***********************************************************************
bool variant_manager::isZProperty( QtProperty const * property ) const
{
    return _z_to_property.find( property ) != _z_to_property.end() ;
}

//***********************************************************************
bool variant_manager::isWProperty( QtProperty const * property ) const
{
    return _w_to_property.find( property ) != _w_to_property.end() ;
}

//***********************************************************************
