//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_LOG_RESULT_H_
#define _SNAKEOIL_LOG_RESULT_H_

#include <string>

namespace so_log
{
	enum result
	{
		ok,
		failed,
        invalid_argument
	};

	namespace so_internal
	{
		static const std::string __result_strings[] = {
			"need to fill"
		} ;
	}

	static bool success( result r ){ return r == ok ; }
	static bool no_success( result r ) { return !success(r) ; }
}

#endif

