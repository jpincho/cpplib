#include "mutex.hpp"
#include <assert.h>

namespace cpplib
{
mutex::mutex ( void )
    {
    created = false;
    }

bool mutex::create ( void )
    {
    if ( created )
        return true;
	assert ( pthread_mutex_init ( &handle, NULL ) == 0 );
    created = true;
    return true;
    }

bool mutex::destroy ( void )
    {
    if ( !created )
        return true;
    assert(pthread_mutex_destroy ( &handle )==0);
    created = false;
    return true;
    }

bool mutex::lock ( void )
    {
    if ( !created )
        create();
	pthread_mutex_lock ( &handle );
    return true;
    }

bool mutex::try_lock ( void )
    {
    if ( !created )
        create();
    if ( pthread_mutex_trylock ( &handle ) )
        return true;
    return false;
    }

bool mutex::unlock ( void )
    {
    if ( !created )
        create();
	pthread_mutex_unlock ( &handle );
    return true;
    }
}
