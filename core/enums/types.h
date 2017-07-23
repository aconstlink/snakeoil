//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_ENUMS_TYPES_H_
#define _SNAKEOIL_ENUMS_TYPES_H_

#include <string>

namespace so_core
{
    /// @note 
    /// changing this type should be thought of carefully. it is used 
    /// in other places for mapping to other types.
    enum class type
    {
        undefined,
        tchar,
        tuchar,
        tshort,
        tushort,
        tint,
        tuint,
        tfloat,
        tdouble,
        tbool
    } ;

    /// @note 
    /// changing this type should be thought of carefully. it is used 
    /// in other places for mapping to other types.
    enum class type_structure
    {
        undefined,
        vec1,
        vec2,
        vec3,
        vec4,
        mat2,
        mat3,
        mat4
    } ;

    enum class type_reference
    {
        undefined,
        value,
        ref,
        cref,
        ptr,
        cptr,
        ptr_ref,
        cptr_ref
    } ;

    static bool is_undefined( type t )
    {
        return t == type::undefined ;
    }

    static bool is_undefined( type_structure t )
    {
        return t == type_structure::undefined ;
    }

    namespace so_internal
    {
        static std::string type_string_array[] = {"t_not_implemented"} ;

        // this must correspond to the TYPE enum
        static const size_t type_size_array[] = {
            0, sizeof (char), sizeof (unsigned char), sizeof (short), sizeof (unsigned short),
            sizeof (int), sizeof ( unsigned int), sizeof (float), sizeof (double), sizeof (bool)
        } ;

        static std::string type_structure_string_array[] = {"ts_not_implemented"} ;

        // this must correspond to the TYPE_STRUCTURE enum
        static const unsigned int _type_struct_size_array[] = {
            0, 1, 2, 3, 4, 4, 9, 16
        } ;

        // a vector every has 1 row, so vectors must stored in memory
        // like a row vector, of course.
        static const size_t type_struct_rows[] = {
            0, 1, 1, 1, 1, 2, 3, 4
        } ;

        static const size_t type_struct_columns[] = {
            0, 1, 2, 3, 4, 2, 3, 4
        } ;

    }

    static std::string const & to_string( type /*t*/ )
    {
        return so_internal::type_string_array[0] ;
    }

    static std::string const & to_string( type_structure /*t*/ )
    {
        return so_internal::type_structure_string_array[0] ;
    }

    static size_t size_of( type t )
    {
        return so_internal::type_size_array[( size_t ) t] ;
    }

    static size_t size_of( type_structure ts )
    {
        return so_internal::_type_struct_size_array[( size_t ) ts] ;
    }

    /// returns the number of rows a type structure has.
    /// Dont forget. Vectors have a row count of 1
    static size_t rows_of( type_structure ts )
    {
        return so_internal::type_struct_rows[size_t(ts)] ;
    }

    /// returns the number of columns a type struct has.
    static size_t columns_of( type_structure ts )
    {
        return so_internal::type_struct_columns[size_t(ts)] ;
    }

    /// from human readable strings.
    /// accepts float instead of t_float and so forth.
    /// so to_type_human("t_float") will not work.
    /// but to_type_human("float") will work.
    static type to_type_human( std::string const & name )
    {
        if( name == "char" ) return type::tchar ;
        else if( name == "uchar" ) return type::tuchar ;
        else if( name == "short" ) return type::tshort ;
        else if( name == "ushort" ) return type::tushort ;
        else if( name == "int" ) return type::tint ;
        else if( name == "uint" ) return type::tuint ;
        else if( name == "float" ) return type::tfloat ;
        else if( name == "double" ) return type::tdouble ;
        else if( name == "bool" ) return type::tbool ;

        return type::undefined ;
    }

    /// from human readable strings.
    /// accepts float instead of t_float and so forth.
    /// so to_type_struct_human("ts_scalar") will not work.
    /// but to_type_struct_human("scalar") will work.
    static type_structure to_type_struct_human( std::string const & name )
    {
        if( name == "scalar" ) return type_structure::vec1;
        else if( name == "vec2" ) return type_structure::vec2 ;
        else if( name == "vec3" ) return type_structure::vec3 ;
        else if( name == "vec4" ) return type_structure::vec4 ;
        else if( name == "mat2" ) return type_structure::mat2 ;
        else if( name == "mat3" ) return type_structure::mat3 ;
        else if( name == "mat4" ) return type_structure::mat4 ;

        return type_structure::undefined ;
    }
}

#endif



