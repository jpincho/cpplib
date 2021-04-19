#include <stdio.h>
#include "string.h"
#include <assert.h>

void string_test_assign ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::string string;
    string.assign ( __FUNCTION__ );

    assert ( string.length () == strlen ( __FUNCTION__ ) );
    assert ( strcpy ( string.data (), __FUNCTION__ ) == 0 );
    }

void string_test_find ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::string string;
    string.assign ( __FUNCTION__ );

    off_t position = string.find_first_of ( "abc_" );
    assert ( position == strchr ( __FUNCTION__, '_' ) );

    position = string.find_first_of ( "xyz" );
    assert ( position == -1 );

    position = string.find_first_of ( "_r" );
    assert ( position == strchr ( __FUNCTION__, 'r' );
    }

void string_test_iterator ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::string <uint8_t> string;
    string.push_back ( 42 );
    string.push_back ( 25 );
    string.push_back ( 11 );
    string.push_back ( 79 );

    for ( uint8_t cont = 0; cont < string.size (); ++cont )
        printf ( "%u - %u\n", cont, string[cont] );
    int cont = 0;
    for ( cpplib::string<uint8_t>::iterator iterator = string.begin (); iterator != string.end (); ++iterator, ++cont )
        {
        printf ( "%u - %u\n", cont, *iterator );
        assert ( *iterator == string[cont] );
        }
    }

void string_test_auto_iterator ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::string <uint8_t> string;
    string.push_back ( 42 );
    string.push_back ( 25 );
    string.push_back ( 11 );
    string.push_back ( 79 );

    int cont = 0;
    for ( auto iterator : string )
        {
        printf ( "%u - %u\n", cont, iterator );
        assert ( iterator == string[cont] );
        ++cont;
        }
    }


int main ()
    {
    string_test_push_back ();
    string_test_value_change ();
    string_test_iterator ();
    string_test_auto_iterator ();
    return 0;
    }
