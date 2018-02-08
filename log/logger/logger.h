//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../api.h"
#include "../typedefs.h"
#include "../result.h"
#include "../protos.h"
#include "../log_level.h"

namespace so_log
{
    class SNAKEOIL_LOG_API ilogger
    {
    public:

        virtual so_log::result log( so_log::log_level ll, std::string const & msg ) = 0 ;
    };
}
