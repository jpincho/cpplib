#include "mutex.hpp"
#include <Windows.h>

namespace cpplib
{
mutex::mutex( void )
	{
	created = false;
	}

bool mutex::create( void )
	{
	if ( created )
		return true;
	InitializeCriticalSection( &handle );
	created = true;
	return true;
	}

bool mutex::destroy( void )
	{
	if ( !created )
		return true;
	DeleteCriticalSection( &handle );
	created = false;
	return true;
	}

bool mutex::lock(void)
	{
	if ( !created )
		create();
	EnterCriticalSection( &handle );
	return true;
	}

bool mutex::try_lock( void )
	{
	if ( !created )
		create();
	if ( TryEnterCriticalSection( &handle ) )
		return true;
	return false;
	}

bool mutex::unlock( void )
	{
	if ( !created )
		create();
	LeaveCriticalSection( &handle );
	return true;
	}
}
