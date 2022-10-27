#include "thread.hpp"
#include <assert.h>

namespace cpplib
{
thread::thread ( void )
    {
	running = false;
    }

bool thread::create ( uint32_t ( *function_pointer ) ( void * ), void *parameters )
    {
    if ( is_running() )
        return false;
	assert ( pthread_create ( &thread_handle, nullptr, function_pointer, parameters ) == 0 );
    if ( thread_handle == NULL )
        return false;
	running = true;
    return true;
    }

bool thread::join ( void )
    {
    if ( !is_running() )
        return false;
	assert ( pthread_join ( &thread_handle, nullptr ) == 0 );
	running = false;
    return true;
    }

bool thread::is_running ( void )
    {
	return running;
    }

uint32_t thread::get_id ( void )
    {
	return 0;
    }

int get_core_count ( void )
	{
	return sysconf ( _SC_NPROCESSORS_ONLN );
	}
}
