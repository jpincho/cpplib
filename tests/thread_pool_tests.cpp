#include <cpplib/thread_pool.hpp>

uint32_t function( void *params )
	{
	int secs = 10;
	while ( secs )
		{
		printf( "WORKING... %d\n", PtrToUlong( params ), secs-- );
		Sleep( 1000 );
		}
	return 0;
	}


int main( void )
	{
	cpplib::thread_pool pool;
	pool.initialize();
	for ( long long cont = 0; cont < 10; ++cont )
		{
		pool.add_task( &function, (void *) cont );
		}

	Sleep( 5000 );
	pool.shutdown();
	}