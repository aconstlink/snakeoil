//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_TIME_CONVERTER_H_
#define _SNAKEOIL_QT_TIME_CONVERTER_H_

#include "../typedefs.h"

#include <QString>

namespace so_qt
{
    class time_converter
    {
    public:

        time_converter( void_t ) ;
        ~time_converter( void_t ) ;

    public:

        // convert number to milli seconds
        static QString to_mil( unsigned long long number ) ;
        // convert number to seconds
        static QString to_sec( unsigned long long number ) ;
        // convert number to minutes
        static QString to_min( unsigned long long number ) ;
        // convert number to hours
        static QString to_hrs( unsigned long long number ) ;
    };
}

#endif
