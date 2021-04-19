#include <stdio.h>
#include "vectorized_container.h"
#include <set>

void vectorized_container_test ( void )
    {
    cpplib::vectorized_container <uint8_t> container;
    std::set <intptr_t> indices;

    container.reserve ( 30 );
    container.debug ();
    for ( unsigned cont = 0; cont < 30; ++cont )
        {
        intptr_t index = container.get_free_index ();
        container[index] = cont;
        }

    container.debug ();

    for ( unsigned cont = 0; cont < 10; ++cont )
        {
        intptr_t index;
        do
            {
            index = rand () % container.size ();
            }
        while ( indices.find ( index ) != indices.end () );
        indices.insert ( index );
        }

    for ( auto index : indices )
        {
        printf ( "Removing index %Ii\n", index );
        container.release_index ( index );
        }

    indices.clear ();
    container.debug ();

    for ( unsigned cont = 0; cont < 5; ++cont )
        {
        intptr_t index = container.get_free_index ();
        container[index] = cont;
        }

    container.debug ();

    printf ( "Automatic iterator test\n" );
    for ( auto &iterator : container )
        {
        printf ( "%u\n", iterator );
        }

    printf ( "iterator test\n" );
    for ( cpplib::vectorized_container <uint8_t>::iterator iterator = container.begin (); iterator != container.end (); ++iterator )
        {
        printf ( "%u\n", *iterator );
        }
    }

int main ()
    {
    vectorized_container_test ();
    return 0;
    }
