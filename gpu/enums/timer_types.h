//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_TIMER_TYPES_H_
#define _SNAKEOIL_GPU_TIMER_TYPES_H_

#include <string>

namespace so_gpu
{
    enum class timer_query_type
    {
        undefined,
		time_elapsed,
		time_stamp
    } ;

	enum class timer_time_unit
	{
		undefined,
		nano
	} ;

    namespace so_internal
    {
        static std::string const timer_type_array[] = 
        {
            "undefined","time_elapsed","time_stamp"
        } ;

		static std::string const timer_time_unit_array[] = 
        {
			"undefined","nano"
		} ;

		static double_t const timer_time_unit_scales[] = 
        {
			0.0, 0.000000001
		} ;
    }

    static std::string const & to_string( timer_query_type t )
    {
        return so_internal::timer_type_array[size_t(t)] ;
    }
	static std::string const & to_string( timer_time_unit t )
    {
		return so_internal::timer_time_unit_array[size_t(t)] ;
	}

	static double_t get_time_unit_scale( timer_time_unit unit )
    {
		return so_internal::timer_time_unit_scales[size_t(unit)] ;
	}
}

#endif

