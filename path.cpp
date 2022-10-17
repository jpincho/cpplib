#include "path.hpp"
#if defined ( CPPLIB_TARGET_PLATFORM_WINDOWS )
#define getcwd _getcwd
#include <direct.h>
#else
#include <libgen.h>
#include <unicpplib.h>
#endif
#include <cctype>
#include "string_utils.hpp"

namespace cpplib
{
namespace filesystem
{
path::path ( void )
    {
	separator = get_default_separator ();
    }

path::path ( const char *new_path, const char new_path_separator )
    {
	separator = new_path_separator;
	set ( new_path, separator );
    }

path::path ( const string &new_path, const char new_path_separator )
    {
	separator = new_path_separator;
	set ( new_path, separator );
    }

void path::set ( string new_path )
    {
	set ( new_path, separator );
    }

void path::set ( const char *new_path )
    {
	set ( new_path, separator );
    }

void path::set ( string new_path, const char custom_separator )
	{
	clear ();
	new_path.trim ();
	string_split ( new_path, custom_separator, parts, true );
	}

void path::set ( const char *new_path, const char custom_separator )
	{
	clear ();
	string string_path ( new_path );
	string_path.trim ();
	string_split ( string_path, custom_separator, parts, true );
	}

void path::set_separator ( const char new_separator )
	{
	separator = new_separator;
	}

char path::get_separator ( void ) const
	{
	return separator;
	}

void path::clear ( void )
    {
    parts.clear();
    }

string path::get_owning_path_as_string ( void ) const
	{
	return get_owning_path_as_string ( separator );
	}

string path::get_owning_path_as_string ( const char custom_separator ) const
	{
	string result;
	result = root;
	for ( size_t index = 0; parts.get_size () > 0 && index < parts.get_size () - 1; ++index )
		{
		result += parts[index];
		result += custom_separator;
		}
	return result;
	}

string path::get_as_string ( void ) const
    {
	return get_as_string ( separator );
    }

string path::get_as_string ( const char custom_separator ) const
	{
	string result;
	result = root;
	for ( size_t index = 0; index < parts.get_size (); ++index )
		{
		result += parts[index];
		result += custom_separator;
		}
	return result;
	}

void path::go_down ( string new_child_path )
    {
    new_child_path.trim();
    string_split ( new_child_path, separator, parts, true );
    }

void path::go_down ( string new_child_path, const char custom_separator )
	{
	new_child_path.trim ();
	string_split ( new_child_path, custom_separator, parts, true );
	}

void path::go_up ( void )
    {
    parts.pop_back();
    }

size_t path::get_depth ( void ) const
    {
    return parts.get_size();
    }

bool path::is_parent_of ( const path &other ) const
    {
    if ( parts.get_size() > other.parts.get_size() )
        return false;
    for ( size_t index = 0; index < parts.get_size(); ++index )
        {
        if ( parts[index] != other.parts[index] )
            return false;
        }
    return true;
    }

void path::expand ( void )
    {
    for ( size_t index = 0; index < parts.get_size(); )
        {
        if ( parts[index].compare ( "." ) == 0 )
            {
            parts.erase ( index );
            continue;
            }

        if ( ( parts[index].compare ( ".." ) == 0 ) && ( index >= 1 ) )
            {
            parts.erase ( index, 2 );
            continue;
            }
        ++index;
        }
    }

path path::operator+ ( const path &other ) const
    {
    path result;
    result = *this;
    result.parts += other.parts;
    return result;
    }

path &path::operator+= ( const path &other )
    {
    parts += other.parts;
    return ( *this );
    }

bool path::operator== ( const path &other ) const
    {
    return ( parts == other.parts );
    }

bool path::operator!= ( const path &other ) const
    {
    return ( parts != other.parts );
    }

path path::operator+ ( const string &other ) const
    {
    path result;
    result = *this;
    result.parts += path ( other ).parts;
    return result;
    }

path &path::operator+= ( const string &other )
    {
    parts += path ( other ).parts;
    return ( *this );
    }

bool path::operator== ( const string &other ) const
    {
    return ( get_as_string() == other );
    }

bool path::operator!= ( const string &other ) const
    {
    return ( !operator== ( other ) );
    }

}
}
