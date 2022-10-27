#pragma once
#include <stdint.h>
#include <semaphore.h>

namespace cpplib
{
class semaphore
    {
    private:
        sem_t handle;
        bool created;
        semaphore &operator = ( const semaphore &other ) = delete;
        int32_t lock_count;
    public:
		semaphore ( void );
		bool create ( int32_t initial_count, int32_t max_count = 0x7FFFFFFF );
		bool destroy ( void );
		bool decrement ( void );
		bool try_decrement ( void );
		bool increment ( void );
		int32_t get_count ( void ) const;
    };
}
