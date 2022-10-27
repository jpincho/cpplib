#pragma once
#include <pthread.h>

namespace cpplib
{
class mutex
    {
    private:
		pthread_mutex_t handle;
        bool created;
        mutex &operator = ( const mutex &other ) = delete;
    public:
        mutex ( void );
        bool create ( void );
        bool destroy ( void );
        bool lock ( void );
        bool try_lock ( void );
        bool unlock ( void );
    };
}
