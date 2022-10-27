#include "semaphore.hpp"
#include <stdio.h>

namespace cpplib
{
semaphore::semaphore ( void )
    {
    created = false;
	handle = nullptr;
	lock_count = 0;
    }

bool semaphore::create ( int32_t initial_count, int32_t max_count )
    {
    if ( created )
        return true;
    handle = CreateSemaphore ( NULL, initial_count, max_count, NULL );
    if ( !handle )
        {
        DWORD last_error = GetLastError();
        LPSTR pBuffer = NULL;
        FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, last_error, 0, ( LPSTR ) &pBuffer, 512, 0 );
        printf ( "Error - %s\n", pBuffer );
        free ( pBuffer );
        return false;
        }
    created = true;
    lock_count = initial_count;
    return true;
    }

bool semaphore::destroy ( void )
    {
    if ( !created )
        return true;
    CloseHandle ( &handle );
    created = false;
    return true;
    }

bool semaphore::decrement ( void )
    {
    if ( !created )
        return false;
    WaitForSingleObject ( handle, INFINITE );
    ++lock_count;
    return true;
    }

bool semaphore::try_decrement ( void )
    {
    if ( !created )
        return false;
    unsigned result;
    result = WaitForSingleObject ( handle, 0 );
    if ( result == WAIT_OBJECT_0 )
        {
        ++lock_count;
        return true;
        }
    return false;
    }

bool semaphore::increment ( const size_t count )
    {
    if ( !created )
        return false;
    ReleaseSemaphore ( handle, count, NULL );
    lock_count -= count;
    return true;
    }

int32_t semaphore::get_count ( void ) const
    {
    if ( !created )
        return false;
    return lock_count;
    }

}
