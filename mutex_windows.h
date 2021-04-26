#pragma once
#include <Windows.h>

namespace cpplib
{
class mutex
	{
	private:
		CRITICAL_SECTION handle;
		bool created;

	public:
		mutex( void );
		bool create( void );
		bool destroy( void );
		bool lock( void );
		bool try_lock( void );
		bool unlock( void );
	};
}