#include "string_utils.hpp"

namespace cpplib
{
void string_split ( const string &original_string, const char delimiter, array<string> &result, const bool ignore_empty_lines, const bool remove_delimiter )
    {
    off_t last_position = 0, current_position = 0;

    while ( ( current_position = original_string.find_first ( delimiter, last_position ) ) != -1 )
        {
        string substring = original_string.substr ( last_position, ( remove_delimiter ? current_position : current_position + 1 ) - last_position );
        if ( ( ignore_empty_lines == false ) || ( substring.get_length() != 0 ) )
            result.push_back ( substring );
        last_position = current_position + 1;
        }
    if ( last_position < original_string.get_length() )
        {
        result.push_back ( original_string.substr ( last_position ) );
        }
    }
}