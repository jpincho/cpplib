#pragma once
#include "cpplib_config.h"
#include "string.hpp"
#include "array.hpp"

namespace cpplib
{
namespace filesystem
{
class path
    {
    protected:
        array <string> parts;
        string root;
        char separator;

    public:
        path ( void );
        path ( const char *new_path, const char new_path_separator = get_default_separator() );
        path ( const string &new_path, const char new_path_separator = get_default_separator () );

		void set ( string new_path );
		void set ( const char *new_path );
        void set ( string new_path, const char custom_separator );
        void set ( const char *new_path, const char custom_separator );

		void set_separator ( const char new_separator );
		char get_separator ( void ) const;

        void clear ( void );
        string get_owning_path_as_string ( void ) const;
		string get_owning_path_as_string ( const char custom_separator ) const;
		string get_as_string ( void ) const;
		string get_as_string ( const char custom_separator ) const;
		void go_down ( string new_child_path );
		void go_down ( string new_child_path, const char custom_separator );
        void go_up ( void );
        size_t get_depth ( void ) const;
        bool is_parent_of ( const path &other ) const;
        void expand ( void );

        path operator + ( const path &other ) const;
        path &operator += ( const path &other );
        bool operator == ( const path &other ) const;
        bool operator != ( const path &other ) const;
        path operator + ( const string &other ) const;
        path &operator += ( const string &other );
        bool operator == ( const string &other ) const;
        bool operator != ( const string &other ) const;

		static constexpr char get_default_separator ( void )
			{
#if defined ( CPPLIB_TARGET_PLATFORM_WINDOWS )
			return '\\';
#else
			return '/';
#endif
			}
    };

}
}
