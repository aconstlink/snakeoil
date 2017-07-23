//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_VARIANT_MANAGER_H_
#define _SNAKEOIL_QT_VARIANT_MANAGER_H_

#include "../meta_types.h"

#include <QtVariantPropertyManager>

#include <map>
#include <utility>
#include <functional>

namespace so_qt
{
    class variant_manager : public QtVariantPropertyManager
    {
        Q_OBJECT

    private:

        typedef variant_manager this_t ;
        typedef std::function< int(void) > type_funk_t ;

    private:

        struct data_item
        {
            QVariant value ;
            QtVariantProperty * x ;
            QtVariantProperty * y ;
            QtVariantProperty * z ;
            QtVariantProperty * w ;
            QtVariantProperty * step ;
        };
        
        typedef std::map< QtProperty const *, data_item > property_to_data_t ;
        typedef std::map< QtProperty const *, QtProperty * > property_to_property_t ;

    private:

        property_to_data_t _property_to_data ;

        property_to_property_t _x_to_property ;
        property_to_property_t _y_to_property ;
        property_to_property_t _z_to_property ;
        property_to_property_t _w_to_property ;

        property_to_property_t _step_to_property ;

        /// maps a sub-property to its group-property for
        /// easier type check.
        property_to_property_t _sub_to_group ;

    public:

        variant_manager( QObject * parent = nullptr ) ;
        virtual ~variant_manager( void ) ;


    public: // interface

        virtual QVariant value( QtProperty const * p ) const ;
        virtual int valueType( int propertyType ) const ;
        virtual bool isPropertyTypeSupported( int propertyType ) const ;

    public slots:

        virtual void setValue( QtProperty * property, const QVariant & val ) ;

    protected:

        virtual void initializeProperty( QtProperty * property ) ;
        virtual void uninitializeProperty( QtProperty * property ) ;

    private slots:

        void slotValueChanged( QtProperty * property, const QVariant & value ) ;
        void slotPropertyDestroyed( QtProperty * property ) ;

    public:
        
        QString valueText( const QtProperty * property ) const ;

    public: // static 

        static int vec2ui_type_id( void ) ;
        static int vec2i_type_id( void ) ;
        static int vec2f_type_id( void ) ;

        static int vec3ui_type_id( void ) ;
        static int vec3i_type_id( void ) ;
        static int vec3f_type_id( void ) ;

        static int vec4ui_type_id( void ) ;
        static int vec4i_type_id( void ) ;
        static int vec4f_type_id( void ) ;

    private:

        bool initialize_vec2ui( QtProperty * property ) ;
        bool initialize_vec2i( QtProperty * property ) ;
        bool initialize_vec2f( QtProperty * property ) ;

        bool initialize_vec3ui( QtProperty * property ) ;
        bool initialize_vec3i( QtProperty * property ) ;
        bool initialize_vec3f( QtProperty * property ) ;

        bool initialize_vec4ui( QtProperty * property ) ;
        bool initialize_vec4i( QtProperty * property ) ;
        bool initialize_vec4f( QtProperty * property ) ;

        bool setValue_vec2ui( QtProperty * property, const QVariant & val, data_item const & item ) ;
        bool setValue_vec2i( QtProperty * property, const QVariant & val, data_item const & item ) ;
        bool setValue_vec2f( QtProperty * property, const QVariant & val, data_item const & item ) ;

        bool setValue_vec3ui( QtProperty * property, const QVariant & val, data_item const & item ) ;
        bool setValue_vec3i( QtProperty * property, const QVariant & val, data_item const & item ) ;
        bool setValue_vec3f( QtProperty * property, const QVariant & val, data_item const & item ) ;

        bool setValue_vec4ui( QtProperty * property, const QVariant & val, data_item const & item ) ;
        bool setValue_vec4i( QtProperty * property, const QVariant & val, data_item const & item ) ;
        bool setValue_vec4f( QtProperty * property, const QVariant & val, data_item const & item ) ;

        bool valueText_vec2ui( QtProperty const * property, data_item const & item, QString & str_out ) const ;
        bool valueText_vec2i( QtProperty const * property, data_item const & item, QString & str_out ) const ;
        bool valueText_vec2f( QtProperty const * property, data_item const & item, QString & str_out ) const ;

        bool valueText_vec3ui( QtProperty const * property, data_item const & item, QString & str_out ) const ;
        bool valueText_vec3i( QtProperty const * property, data_item const & item, QString & str_out ) const ;
        bool valueText_vec3f( QtProperty const * property, data_item const & item, QString & str_out ) const ;

        bool valueText_vec4ui( QtProperty const * property, data_item const & item, QString & str_out ) const ;
        bool valueText_vec4i( QtProperty const * property, data_item const & item, QString & str_out ) const ;
        bool valueText_vec4f( QtProperty const * property, data_item const & item, QString & str_out ) const ;

        bool slotValueChanged_vec2ui( QtProperty const * property, QVariant const & value) ;
        bool slotValueChanged_vec2i( QtProperty const * property, QVariant const & value ) ;
        bool slotValueChanged_vec2f( QtProperty const * property, QVariant const & value ) ;

        bool slotValueChanged_vec3ui( QtProperty const * property, QVariant const & value) ;
        bool slotValueChanged_vec3i( QtProperty const * property, QVariant const & value ) ;
        bool slotValueChanged_vec3f( QtProperty const * property, QVariant const & value ) ;

        bool slotValueChanged_vec4ui( QtProperty const * property, QVariant const & value) ;
        bool slotValueChanged_vec4i( QtProperty const * property, QVariant const & value ) ;
        bool slotValueChanged_vec4f( QtProperty const * property, QVariant const & value ) ;

    private:

        bool isStepProperty_float( QtProperty const * property, QVariant const & value, float & step_out ) ;
        bool isStepProperty_int( QtProperty const * property, QVariant const & value, int & step_out ) ;

    private:

        bool isParentOfType( QtProperty const * sub, type_funk_t funk, QtProperty *& parent ) ;

        bool isXProperty( QtProperty const * property ) const ;
        bool isYProperty( QtProperty const * property ) const ;
        bool isZProperty( QtProperty const * property ) const ;
        bool isWProperty( QtProperty const * property ) const ;

    private:

        void addStepProperty( int type, QVariant default_value, QtProperty * parent, data_item & di_io ) ;
        void addXProperty( int type, QtProperty * parent, data_item & di_io ) ;
        void addYProperty( int type, QtProperty * parent, data_item & di_io ) ;
        void addZProperty( int type, QtProperty * parent, data_item & di_io ) ;
        void addWProperty( int type, QtProperty * parent, data_item & di_io ) ;

    };
}

#endif
