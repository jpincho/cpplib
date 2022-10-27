#include "../thread_pool.hpp"
#include <stdio.h>

uint32_t function ( void *params )
    {
    int secs = 10;
    while ( secs )
        {
        printf ( "WORKING... %d %d\n", PtrToUlong ( params ), secs-- );
        Sleep ( 1000 );
        }
    return 0;
    }


int main ( void )
    {
    cpplib::thread_pool pool;
    pool.initialize();
    for ( long long cont = 0; cont < 50; ++cont )
        {
        pool.add_task ( &function, ( void * ) cont );
        }
	printf ( "Added %u tasks\n", pool.get_task_count () );
	while ( pool.get_task_count()>0 )
		Sleep ( 1000 );
	printf ( "Shutting down\n" );
    pool.shutdown();
    }
