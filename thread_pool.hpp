#pragma once
#include "thread.hpp"
#include "mutex.hpp"
#include "semaphore.hpp"
#include "list.hpp"

namespace cpplib
{
class thread_pool
	{
	friend class thread_pool_worker;
	public:
		typedef struct task
			{
			uint32_t( *function )( void * );
			void *parameters;
			uint32_t result;
			bool auto_release;
			}task;
	private:
		thread *thread_array;
		size_t thread_count;
		bool running;
		
		list <task*> task_list;
		mutex task_list_mutex;
		semaphore new_task_semaphore;

	public:
		thread_pool( void );
		~thread_pool( void );
		bool initialize( void );
		bool shutdown( void );
		uint32_t get_thread_count( void );
		void add_task( uint32_t( *function )( void * ), void *parameters );
		void clear_tasks( void );
	};
}
