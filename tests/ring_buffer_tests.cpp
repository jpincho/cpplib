#include <stdio.h>
#include "../ring_buffer.hpp"
#include <stdint.h>

void ring_buffer_test ( void )
    {
    printf ( "Ring buffer test\n" );
    cpplib::ring_buffer <uint8_t> ring ( 10 );
    for ( unsigned cont = 0; cont < 20; ++cont )
        {
        ring.push_back ( cont );
        }
    for ( unsigned cont = 0; cont < 5; ++cont )
        {
        ring.pop_back ();
        }
    for ( unsigned cont = 0; cont < 20; ++cont )
        {
        ring.push_front ( cont );
        }
    for ( unsigned cont = 0; cont < 5; ++cont )
        {
        ring.pop_front ();
        }
    }

int main ()
    {
    ring_buffer_test ();
    return 0;
    }


