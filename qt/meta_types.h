//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_META_TYPES_H_
#define _SNAKEOIL_QT_META_TYPES_H_

#include <QObject>

#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

Q_DECLARE_METATYPE( so_math::vec2ui_t )
Q_DECLARE_METATYPE( so_math::vec2i_t )
Q_DECLARE_METATYPE( so_math::vec2f_t )

Q_DECLARE_METATYPE( so_math::vec3ui_t )
Q_DECLARE_METATYPE( so_math::vec3i_t ) 
Q_DECLARE_METATYPE( so_math::vec3f_t ) 

Q_DECLARE_METATYPE( so_math::vec4ui_t )
Q_DECLARE_METATYPE( so_math::vec4i_t ) 
Q_DECLARE_METATYPE( so_math::vec4f_t )

#endif
