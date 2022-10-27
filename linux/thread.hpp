#pragma once
#include <pthread.h>
#include <stdint.h>

namespace cpplib
{
class thread
    {
    private:
		pthread_t thread_handle;
		bool running;
        thread &operator = ( const thread &other ) = delete;
    public:
        thread ( void );
        bool create ( uint32_t ( *function_pointer ) ( void * ), void *parameters );
        bool join ( void );
        bool is_running ( void );
        static uint32_t get_id ( void );
    };

int get_core_count ( void );
}
