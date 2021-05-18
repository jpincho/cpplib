#pragma once
#include "string.hpp"
#include "array.hpp"

namespace cpplib
{
void string_split ( const string &original_string, const char delimiter, array<string> &result, const bool ignore_empty_lines = false, const bool remove_delimiter = true );
}