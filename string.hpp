#pragma once
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <assert.h>

namespace cpplib
{
class string
    {
    private:
        char *elements;
        size_t length, capacity;
        static size_t strlen ( const char *other )
            {
            if ( !other )
                return 0;
            size_t length = 0;
            while ( *other != 0 )
                {
                ++other;
                ++length;
                }
            return length;
            }
    public:
        string ( void );
        string ( const char *new_string );
        string ( const string &other );
        string ( string &&other );
        ~string ( void );

        void reserve ( const size_t new_capacity );
        void free ( void );
        void resize ( const size_t new_capacity );
        void assign ( const char *new_string );
        void assign ( const char *new_string, size_t desired_length );
        void assign ( const char *new_string_begin, const char *new_string_end );
        void assign ( const string &new_string );
		void append ( const string &new_string );
        void append ( const char *new_string );
        void append ( const char &new_char );
        void clear ( void );
        off_t find_first ( const char token, const off_t start_offset = 0 ) const;
        off_t find_last ( const char token, off_t start_offset = -1 ) const;
        off_t find_first_substr ( const string &token, const off_t start_offset = 0 ) const;
        off_t find_first_not ( const char token, const off_t start_offset = 0 ) const;
        off_t find_last_not ( const char token, off_t start_offset = -1 ) const;

        off_t find_first_of ( const char *tokens, const off_t start_offset = 0 ) const;
        off_t find_last_of ( const char *tokens, off_t start_offset = -1 ) const;
        off_t find_first_not_of ( const char *tokens, const off_t start_offset = 0 ) const;
        off_t find_last_not_of ( const char *tokens, off_t start_offset = -1 ) const;
        string substr ( const size_t start ) const;
        string substr ( const size_t start, size_t desired_length ) const;
        void replace_char ( const char old_value, const char new_value );
        void trim ( const char *trim_characters = " \n\r\t" );
        bool compare ( const char *other ) const;
        bool compare ( const string &other ) const;
        size_t get_length ( void ) const;
        bool is_empty ( void ) const;
        bool is_numeric ( void ) const;
        char *get_data ( void ) const;
        char operator [] ( const size_t index ) const;
        string &operator = ( const string &other );
        char *operator () ( void ) const;
        operator bool ( void ) const;
        bool operator == ( const string &other ) const;
        bool operator != ( const string &other ) const;
        operator char * ( void ) const;
        string operator + ( const string &other ) const;
        string operator + ( const char *other ) const;
        string operator + ( const char &other ) const;
        string &operator += ( const string &other );
        string &operator += ( const char *other );
        string &operator += ( const char &other );
    };
}
