//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TYPES_H_
#define _SNAKEOIL_GPU_TYPES_H_

#include <string>

namespace so_gpu
{
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
        tbool,
        num_scalar_types
    } ;

    enum class type_struct
    {
        undefined,
        vec1, // scalar
        vec2,
        vec3,
        vec4,
        mat2,
        mat3,
        mat4,
        num_type_structures
    } ;

    namespace so_internal
    {
        static std::string const type_string_array[] = 
        {
            "t_not_implemented"
        } ;

        // this must correspond to the TYPE enum
        static const size_t type_size_array[] = 
        {
            0, sizeof (char), sizeof (unsigned char), sizeof (short), sizeof (unsigned short),
            sizeof (int), sizeof ( unsigned int), sizeof (float), sizeof (double), sizeof (bool)
        } ;

        static std::string const type_structure_string_array[] = 
        {
            "ts_not_implemented"
        } ;

        // this must correspond to the TYPE_STRUCTURE enum
        static const unsigned int _type_struct_size_array[] = 
        {
            0, 1, 2, 3, 4, 4, 9, 16
        } ;

        // a vector every has 1 row, so vectors must stored in memory
        // like a row vector, of course.
        static const size_t type_struct_rows[] = 
        {
            0, 1, 1, 1, 1, 2, 3, 4
        } ;

        static const size_t type_struct_columns[] = 
        {
            0, 1, 2, 3, 4, 2, 3, 4
        } ;

    }

    static std::string const & to_string( type /*t*/ )
    {
        return so_internal::type_string_array[0] ;
    }

    static std::string const & to_string( type_struct /*t*/ )
    {
        return so_internal::type_structure_string_array[0] ;
    }

    static size_t size_of( type t )
    {
        return so_internal::type_size_array[size_t(t)] ;
    }

    static size_t size_of( type_struct ts )
    {
        return so_internal::_type_struct_size_array[size_t(ts)] ;
    }

    /// returns the number of rows a type structure has.
    /// Dont forget. Vectors have a row count of 1
    static size_t rows_of( type_struct ts )
    {
        return so_internal::type_struct_rows[size_t(ts)] ;
    }

    /// returns the number of columns a type struct has.
    static size_t columns_of( type_struct ts )
    {
        return so_internal::type_struct_columns[size_t(ts)] ;
    }

    
}

#endif

