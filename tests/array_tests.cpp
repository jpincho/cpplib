#include <stdio.h>
#include "../array.h"
#include <stdint.h>
#include <assert.h>

void array_test_push_back ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::array <uint8_t> array;
    array.push_back ( 42 );
    array.push_back ( 25 );
    array.push_back ( 11 );
    array.push_back ( 79 );

    for ( uint8_t cont = 0; cont < array.get_size (); ++cont )
        printf ( "%u - %u\n", cont, array[cont] );

    assert ( array[0] == 42 );
    assert ( array[1] == 25 );
    assert ( array[2] == 11 );
    assert ( array[3] == 79 );
    assert ( array.get_size () == 4 );
    }

void array_test_value_change ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::array <uint8_t> array;
    array.push_back ( 42 );
    array.push_back ( 25 );
    array.push_back ( 11 );
    array.push_back ( 79 );

    array[2] = 0;
    for ( uint8_t cont = 0; cont < array.get_size(); ++cont )
        printf ( "%u - %u\n", cont, array[cont] );

    assert ( array[0] == 42 );
    assert ( array[1] == 25 );
    assert ( array[2] == 0 );
    assert ( array[3] == 79 );
    assert ( array.get_size () == 4 );
    }

void array_test_iterator ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::array <uint8_t> array;
    array.push_back ( 42 );
    array.push_back ( 25 );
    array.push_back ( 11 );
    array.push_back ( 79 );

    for ( uint8_t cont = 0; cont < array.get_size (); ++cont )
        printf ( "%u - %u\n", cont, array[cont] );
    int cont = 0;
    for ( cpplib::array<uint8_t>::iterator iterator = array.begin (); iterator != array.end (); ++iterator, ++cont )
        {
        printf ( "%u - %u\n", cont, *iterator );
        assert ( *iterator == array[cont] );
        }
    }

void array_test_auto_iterator ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::array <uint8_t> array;
    array.push_back ( 42 );
    array.push_back ( 25 );
    array.push_back ( 11 );
    array.push_back ( 79 );

    int cont = 0;
    for ( auto iterator : array )
        {
        printf ( "%u - %u\n", cont, iterator );
        assert ( iterator == array[cont] );
        ++cont;
        }
    }


int main()
    {
    array_test_push_back ();
    array_test_value_change ();
    array_test_iterator ();
    array_test_auto_iterator ();
    return 0;
    }
