#pragma once
#include <Windows.h>
#include <stdint.h>

namespace cpplib
{
class thread
	{
	private:
		DWORD thread_id;
		HANDLE thread_handle;
	public:
		thread( void );
		bool create( uint32_t( *function_pointer )( void * ), void *parameters );
		bool join( void );
		bool is_running( void );
	};
}