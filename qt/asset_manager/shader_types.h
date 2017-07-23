//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_ASSET_MANAGER_SHADER_TYPE_H_
#define _SNAKEOIL_QT_ASSET_MANAGER_SHADER_TYPE_H_

#include <QString>
#include <string>

namespace so_qt
{
    enum class program_type
    {
        invalid,
        render,
        compute
    };

    enum class shader_type
    {
        invalid,
        vertex,
        geometry,
        pixel,
        compute
    };

    namespace so_internal
    {
        static std::string const __SHADER_TYPE_NAMES_STD[] = {
            "invalid", "vertex", "geometry", "pixel", "compute"
        } ;

        static QString const __SHADER_TYPE_NAMES_QT[] = {
            "invalid", "vertex", "geometry", "pixel", "compute"
        } ;

        static std::string const __PROGRAM_TYPE_NAMES_STD[] = {
            "invalid", "render", "compute"
        } ;

        static QString const __PROGRAM_TYPE_NAMES_QT[] = {
            "invalid", "render", "compute"
        } ;
    }

    static std::string const & to_string_std( shader_type st )
    {
        return so_internal::__SHADER_TYPE_NAMES_STD[size_t(st)] ;
    }

    static QString const & to_string_qt( shader_type st )
    {
        return so_internal::__SHADER_TYPE_NAMES_QT[size_t(st)] ;
    }

    static std::string const & to_string_std( program_type t )
    {
        return so_internal::__PROGRAM_TYPE_NAMES_STD[size_t(t)] ;
    }

    static QString const & to_string_qt( program_type t )
    {
        return so_internal::__PROGRAM_TYPE_NAMES_QT[size_t(t)] ;
    }
}

#endif
