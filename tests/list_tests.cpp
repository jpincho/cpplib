#include <stdio.h>
#include "../list.hpp"
#include <stdint.h>
#include <assert.h>

void list_test_push_back ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::list<uint8_t> list;
    for ( uint8_t cont = 0; cont < 20; ++cont )
        {
        list.add_at_end ( cont );
        }

    int cont = 0;
    for ( cpplib::list<uint8_t>::iterator iterator = list.begin (); iterator != list.end (); ++iterator, ++cont )
        {
        printf ( "%u\n", *iterator );
        assert ( *iterator == cont );
        }
    printf ( "List size is %zu\n", list.get_size () );
    assert ( list.get_size () == 20 );

    printf ( "Fifth element is %u\n", * ( list.begin () + 5 ) );
    assert ( * ( list.begin () + 5 ) == 5 );

    printf ( "Tenth element is %u\n", * ( list.begin () + 10 ) );
    assert ( * ( list.begin () + 10 ) == 10 );
    }

void list_test_delete_element ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::list<uint8_t> list;
    for ( uint8_t cont = 0; cont < 20; ++cont )
        {
        list.add_at_end ( cont );
        }

    printf ( "Deleting eighth element\n" );
    list.erase ( list.begin () + 8 );

    printf ( "Fifth element is %u\n", * ( list.begin () + 5 ) );
    assert ( * ( list.begin () + 5 ) == 5 );

    printf ( "Tenth element is %u\n", * ( list.begin () + 10 ) );
    assert ( * ( list.begin () + 10 ) == 11 );
    }

void list_test_find_element ( void )
    {
    printf ( "%s\n", __FUNCTION__ );
    cpplib::list<uint8_t> list;
    for ( uint8_t cont = 0; cont < 20; ++cont )
        {
        list.add_at_end ( cont );
        }

    printf ( "Searching for number 12\n" );
    cpplib::list <uint8_t>::iterator iterator = list.find ( 12 );
    assert ( iterator != list.end () );
    assert ( iterator == list.begin () + 11 );

    printf ( "Searching for number 24\n" );
    iterator = list.find ( 24 );
    assert ( iterator == list.end () );
    }

int main()
    {
    list_test_push_back ();
    list_test_delete_element ();
    list_test_find_element ();
    return 0;
    }
