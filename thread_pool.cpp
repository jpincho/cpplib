#include "thread_pool.hpp"
#include <stdio.h>

namespace cpplib
{

class thread_pool_worker
	{
	private:
		thread_pool *pool;
	public:
		thread_pool_worker( thread_pool *new_pool );
		void work( void );
	};


uint32_t worker_function( void *parameters )
	{
	thread_pool *pool = (thread_pool *) parameters;
	thread_pool_worker worker( pool );
	worker.work();
	return 0;
	}

thread_pool_worker::thread_pool_worker( thread_pool *new_pool ) : pool( new_pool )
	{
	}
void thread_pool_worker::work( void )
	{
	uint32_t current_id = thread::get_id();
	printf( "Thread %lu starting\n", current_id );
	while ( pool->running )
		{
		thread_pool::task *current_task;
		printf( "Thread %lu waiting\n", current_id );
		pool->new_task_semaphore.decrement(); // Wait here until a new task is available
		printf( "Thread %lu ended waiting\n", current_id );
		if ( pool->running == false )
			{
			printf( "Thread %lu quitting\n", current_id );
			return;
			}
		pool->task_list_mutex.lock();
		if ( pool->task_list.get_size() == 0 )
			{
			pool->task_list_mutex.unlock();
			continue;
			}

		current_task = pool->task_list.front(); // TODO Move instead of copy
		pool->task_list.pop_front();
		pool->task_list_mutex.unlock(); // Unlocks so that another thread can use the tasklist

		printf( "Thread %lu working\n", current_id );
		uint32_t task_result = current_task->function( current_task->parameters );// Run the task itself
		if ( current_task->auto_release )
			delete current_task;
		}
	}

thread_pool::thread_pool( void )
	{
	thread_array = nullptr;
	thread_count = 8;
	running = false;
	}

thread_pool::~thread_pool( void )
	{
	shutdown();
	}

bool thread_pool::initialize( void )
	{
	if ( running )
		return false;
	if ( new_task_semaphore.create( 0 ) == false )
		return false;
	running = true;
	printf( "Thread pool initializing with %zu threads\n", thread_count );
	thread_array = new thread[thread_count];
	for ( int index = 0; index < thread_count; ++index )
		{
		thread_array[index].create( worker_function, this );
		}
	printf( "Thread pool finished initializing\n");
	return true;
	}

bool thread_pool::shutdown( void )
	{
	if ( !running )
		return false;
	running = false;
	printf( "Thread pool shutting down\n");
	clear_tasks();
	for ( int index = 0; index < thread_count; ++index )
		{
		printf( "Waiting for thread %u\n", index + 1 );
		new_task_semaphore.increment();
		thread_array[index].join();
		}

	delete[] thread_array;
	thread_array = nullptr;
	printf( "Thread pool shutdown\n");
	return true;
	}

uint32_t thread_pool::get_thread_count( void )
	{
	return thread_count;
	}

void thread_pool::add_task( uint32_t( *function )( void * ), void *parameters )
	{
	auto_mutex_lock lock( task_list_mutex);
	task_list.push_back( new task{ function,parameters } );
	new_task_semaphore.increment();
	}

void thread_pool::clear_tasks( void )
	{
	auto_mutex_lock lock( task_list_mutex );
	for ( auto &iterator : task_list )
		{
		if ( iterator->auto_release )
			delete iterator;
		}
	task_list.clear();
	}
}