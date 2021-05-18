#pragma once
#include "hash.hpp"
#include <stdint.h>
#include <math.h>

namespace cpplib
{
uint64_t hash ( const string &value )
    {
    uint64_t result = 0;
    for ( int iterator = 0; iterator < value.get_length(); ++iterator )
        {
        result += ( uint64_t ) pow ( value[iterator], iterator + 1 );
        }
    return result;
    }
}