#include "string.hpp"

namespace cpplib
{
string::string ( void )
    {
    length = capacity = 0;
    elements = nullptr;
    }

string::string ( const char *new_string )
    {
    length = capacity = 0;
    elements = nullptr;
    assign ( new_string );
    }

string::string ( const string &other )
    {
    length = capacity = 0;
    elements = nullptr;
    assign ( other );
    }

string::string ( string &&other )
    {
    length = other.length;
    capacity = other.capacity;
    elements = other.elements;
    other.length = other.capacity = 0;
    other.elements = nullptr;
    }

string::~string ( void )
    {
    if ( elements )
        ::free ( elements );
    elements = nullptr;
    length = capacity = 0;
    }

void string::reserve ( const size_t new_capacity )
    {
    if ( new_capacity <= capacity )
        return;
    char *new_pointer = ( char * ) realloc ( elements, new_capacity + 1 );
    assert ( new_pointer );
    elements = new_pointer;
    capacity = new_capacity;
    }

void string::free ( void )
    {
    if ( elements )
        ::free ( elements );
    elements = nullptr;
    capacity = 0;
    length = 0;
    }

void string::resize ( const size_t new_capacity )
    {
    if ( new_capacity == 0 )
        return;
    reserve ( new_capacity );
    for ( size_t index = length; index < new_capacity; ++index )
        elements[index] = ' ';
    length = new_capacity;
    elements[length] = 0;
    }

void string::assign ( const char *new_string )
    {
    size_t new_length = strlen ( new_string );
    if ( new_length == 0 )
        return;
    reserve ( new_length );
    memcpy ( elements, new_string, new_length + 1 );
    length = new_length;
    elements[length] = 0;
    }

void string::assign ( const char *new_string, size_t desired_length )
    {
    if ( desired_length == 0 )
        return;
    size_t max_length = strlen ( new_string );
    if ( desired_length > max_length )
        desired_length = max_length;
    reserve ( desired_length );
    memcpy ( elements, new_string, desired_length );
    length = desired_length;
    elements[length] = 0;
    }

void string::assign ( const char *new_string_begin, const char *new_string_end )
    {
    assert ( new_string_end >= new_string_begin );
    size_t desired_length = new_string_end - new_string_begin;
    if ( desired_length == 0 )
        return;
    reserve ( desired_length );
    memcpy ( elements, new_string_begin, desired_length );
    length = desired_length;
    elements[length] = 0;
    }

void string::assign ( const string &new_string )
    {
    if ( new_string.length == 0 )
        return;
    reserve ( new_string.length );
    memcpy ( elements, new_string.elements, new_string.length + 1 );
    length = new_string.length;
    elements[length] = 0;
    }

void string::append ( const char *new_string )
    {
    size_t other_length = strlen ( new_string );
    if ( other_length == 0 )
        return;
    reserve ( length + other_length );
    memcpy ( elements + length, new_string, other_length + 1 );
    length += other_length;
    elements[length] = 0;
    }

void string::append ( const char &new_char )
    {
    reserve ( length + 1 );
    elements[length] = new_char;
    ++length;
    elements[length] = 0;
    }

void string::clear ( void )
    {
    length = capacity = 0;
    }

off_t string::find_first ( const char token, const off_t start_offset ) const
    {
    for ( off_t iterator = start_offset; iterator < length; ++iterator )
        {
        if ( elements[iterator] == token )
            return iterator;
        }
    return -1;
    }

off_t string::find_last ( const char token, off_t start_offset ) const
    {
    if ( start_offset == -1 )
        start_offset = ( off_t ) length - 1;
    for ( off_t iterator = start_offset; iterator >= 0; --iterator )
        {
        if ( elements[iterator] == token )
            return iterator;
        }
    return -1;
    }

off_t string::find_first_substr ( const string &token, const off_t start_offset ) const
    {
    const char *pointer = strstr ( elements + start_offset, token );
    if ( pointer != nullptr )
        return ( pointer - elements );
    return -1;
    }

off_t string::find_first_not ( const char token, const off_t start_offset ) const
    {
    for ( off_t iterator = start_offset; iterator < length; ++iterator )
        {
        if ( elements[iterator] != token )
            return iterator;
        }
    return -1;
    }

off_t string::find_last_not ( const char token, off_t start_offset ) const
    {
    if ( start_offset == -1 )
        start_offset = ( off_t ) length - 1;
    for ( off_t iterator = start_offset; iterator >= 0; --iterator )
        {
        if ( elements[iterator] != token )
            return iterator;
        }
    return -1;
    }

off_t string::find_first_of ( const char *tokens, const off_t start_offset ) const
    {
    size_t token_count = strlen ( tokens );
    for ( off_t iterator = start_offset; iterator < length; ++iterator )
        {
        for ( size_t token_iterator = 0; token_iterator < token_count; ++token_iterator )
            {
            if ( elements[iterator] == tokens[token_iterator] )
                return iterator;
            }
        }
    return -1;
    }

off_t string::find_last_of ( const char *tokens, off_t start_offset ) const
    {
    size_t token_count = strlen ( tokens );
    if ( start_offset == -1 )
        start_offset = ( off_t ) length - 1;
    for ( off_t iterator = start_offset; iterator >= 0; --iterator )
        {
        for ( size_t token_iterator = 0; token_iterator < token_count; ++token_iterator )
            {
            if ( elements[iterator] == tokens[token_iterator] )
                return iterator;
            }
        }
    return -1;
    }

off_t string::find_first_not_of ( const char *tokens, const off_t start_offset ) const
    {
    size_t token_count = strlen ( tokens );
    for ( off_t iterator = start_offset; iterator < length; ++iterator )
        {
        for ( size_t token_iterator = 0; token_iterator < token_count; ++token_iterator )
            {
            if ( elements[iterator] != tokens[token_iterator] )
                return iterator;
            }
        }
    return -1;
    }

off_t string::find_last_not_of ( const char *tokens, off_t start_offset ) const
    {
    size_t token_count = strlen ( tokens );
    if ( start_offset == -1 )
        start_offset = ( off_t ) length - 1;
    for ( off_t iterator = start_offset; iterator >= 0; --iterator )
        {
        for ( size_t token_iterator = 0; token_iterator < token_count; ++token_iterator )
            {
            if ( elements[iterator] != tokens[token_iterator] )
                return iterator;
            }
        }
    return -1;
    }

string string::substr ( const size_t start ) const
    {
    return substr ( start, length - start );
    }

string string::substr ( const size_t start, size_t desired_length ) const
    {
    string result;
    if ( start >= length )
        return result;
    if ( start + desired_length > length )
        desired_length = length - start;
    result.assign ( elements + start, desired_length );
    return result;
    }

void string::replace_char ( const char old_value, const char new_value )
    {
    for ( size_t iterator = 0; iterator < length; ++iterator )
        {
        if ( elements[iterator] == old_value )
            elements[iterator] = new_value;
        }
    }

void string::trim ( const char *trim_characters )
    {
    off_t start = find_first_not_of ( trim_characters );
    if ( start == -1 )
        return;
    off_t end = find_last_not_of ( trim_characters );
    memcpy ( elements, elements + start, end - start + 1 );

    length = end - start + 1;
    elements[length] = 0;
    }

bool string::compare ( const char *other ) const
    {
    // if only one of them is null
    if ( ( ( elements == nullptr ) || ( other == nullptr ) ) && ( elements != other ) )
        return false;
    // if both are null, return true
    if ( ( elements == nullptr ) && ( other == nullptr ) )
        return true;
    size_t other_length = strlen ( other );
    if ( length != other_length )
        return false;
    return memcmp ( elements, other, length ) == 0;
    }

bool string::compare ( const string &other ) const
    {
    // if only one of them is null
    if ( ( ( elements == nullptr ) || ( other.elements == nullptr ) ) && ( elements != other.elements ) )
        return false;
    // if both are null, return true
    if ( ( elements == nullptr ) && ( other.elements == nullptr ) )
        return true;
    if ( length != other.length )
        return false;
    return memcmp ( elements, other.elements, length ) == 0;
    }

size_t string::get_length ( void ) const
    {
    return length;
    }

bool string::is_empty ( void ) const
    {
    return length == 0;
    }

bool string::is_numeric ( void ) const
    {
    if ( length == 0 )
        return false;
    for ( size_t iterator = 0; iterator < length; ++iterator )
        {
        if ( ( elements[iterator] < '0' ) || ( elements[iterator] > 9 ) )
            return false;
        }
    return true;
    }

char *string::get_data ( void ) const
    {
    return elements;
    }

char string::operator [] ( const size_t index ) const
    {
    if ( index >= length )
        {
        throw "trying to access position out of bounds";
        }
    return elements[index];
    }

string &string::operator = ( const string &other )
    {
    free();
    assign ( other );
    return *this;
    }

char *string::operator () ( void ) const
    {
    return elements;
    }

string::operator bool ( void ) const
    {
    return length != 0;
    }

bool string::operator == ( const string &other ) const
    {
    return compare ( other );
    }

bool string::operator != ( const string &other ) const
    {
    return ! ( compare ( other ) );
    }

string::operator char * ( void ) const
    {
    return elements;
    }

string string::operator + ( const string &other ) const
    {
    string result ( *this );
    result.append ( other );
    return result;
    }

string string::operator + ( const char *other ) const
    {
    string result ( *this );
    result.append ( other );
    return result;
    }

string string::operator + ( const char &other ) const
    {
    string result ( *this );
    result.append ( other );
    return result;
    }

string &string::operator += ( const string &other )
    {
    append ( other );
    return *this;
    }

string &string::operator += ( const char *other )
    {
    append ( other );
    return *this;
    }

string &string::operator += ( const char &other )
    {
    append ( other );
    return *this;
    }
}
