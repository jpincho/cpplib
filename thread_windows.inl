#include "thread_windows.h"

namespace cpplib
{
thread::thread( void )
	{
	thread_handle = nullptr;
	thread_id = 0;
	}

bool thread::create( uint32_t( *function_pointer )( void * ), void *parameters )
	{
	if ( is_running() )
		return false;
	thread_handle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) function_pointer, parameters, 0, &thread_id );
	if ( thread_handle == NULL )
		return false;
	return true;
	}

bool thread::join( void )
	{
	if ( !is_running() )
		return false;
	if ( WaitForSingleObject( thread_handle, INFINITE ) != WAIT_OBJECT_0 )
		return false;
	CloseHandle( thread_handle );
	thread_handle = nullptr;
	return true;
	}

bool thread::is_running( void )
	{
	return (thread_handle != nullptr);
	}
}
