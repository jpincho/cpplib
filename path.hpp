#pragma once
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
        path ( const char *new_path );
        path ( const string &new_path );
        void set ( const string &new_path );
        void set ( const char *new_path );

        void clear ( void );
        string get_owning_path ( void ) const;
        string get_as_string ( void ) const;
        void go_down ( const string &new_child_path );
        void go_up ( void );
        size_t get_depth ( void ) const;
        bool is_parent_of ( const path &other ) const;
        void expand ( void );

        path operator + ( const path &other ) const;
        path operator + ( const string &other ) const;
        path &operator += ( const path &other );
        path &operator += ( const string &other );
        bool operator == ( const path &other ) const;
        bool operator == ( const string &other ) const;
        bool operator != ( const path &other ) const;
        bool operator != ( const string &other ) const;
    };

}
}