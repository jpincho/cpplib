#include "semaphore.hpp"
#include <stdio.h>
#include <assert.h>

namespace cpplib
{
semaphore::semaphore ( void )
    {
    created = false;
	lock_count = 0;
    }

bool semaphore::create ( int32_t initial_count, int32_t max_count )
    {
    if ( created )
        return true;
	assert ( sem_init ( &handle, 0, initial_count ) == 0 );

    created = true;
    lock_count = initial_count;
    return true;
    }

bool semaphore::destroy ( void )
    {
    if ( !created )
        return true;
	sem_destroy ( &handle );
    created = false;
    return true;
    }

bool semaphore::decrement ( void )
    {
    if ( !created )
        return false;
	sem_wait ( &handle );
    ++lock_count;
    return true;
    }

bool semaphore::try_decrement ( void )
    {
    if ( !created )
        return false;
    int result = sem_trywait ( &handle );
    if ( result == 0 )
        {
        ++lock_count;
        return true;
        }
    return false;
    }

bool semaphore::increment ( void )
    {
    if ( !created )
        return false;
	assert ( sem_post ( &handle ) == 0 );
    --lock_count;
    return true;
    }

int32_t semaphore::get_count ( void ) const
    {
    if ( !created )
        return false;
    return lock_count;
    }

}
