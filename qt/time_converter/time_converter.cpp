//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "time_converter.h"

#include <snakeoil/animation/utility/time_converter.h>

using namespace so_qt ;

//**************************************************************************************************************
time_converter::time_converter( void_t ) 
{

}

//**************************************************************************************************************
time_converter::~time_converter( void_t )
{

}

//**************************************************************************************************************
QString time_converter::to_mil( unsigned long long number )
{
    unsigned long long const mil = so_ani::time_converter::to_milli( number ) ;
    return QString("%1").arg(mil, 3, 10, QChar('0')) ;
}

//**************************************************************************************************************
QString time_converter::to_sec( unsigned long long number )
{
    unsigned long long const sec = so_ani::time_converter::to_sec( number ) ;      
    return QString("%1").arg(sec, 2, 10, QChar('0')) ;
}

//**************************************************************************************************************
QString time_converter::to_min( unsigned long long number ) 
{
    unsigned long long const value = so_ani::time_converter::to_min( number ) ;
    return QString("%1").arg(value, 2, 10, QChar('0'));
}

//**************************************************************************************************************
QString time_converter::to_hrs( unsigned long long number ) 
{
    unsigned long long const value = so_ani::time_converter::to_hrs( number ) ;
    return QString("%1").arg(value, 2, 10, QChar('0'));
}
