#pragma once

#include "macros/typedef.h"

#include <chrono>

namespace so_core
{
    typedef std::chrono::high_resolution_clock clock_t ;
    typedef std::chrono::seconds seconds_t ;
    typedef std::chrono::milliseconds milliseconds_t ;
}