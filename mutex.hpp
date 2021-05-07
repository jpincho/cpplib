#pragma once
#include "cpplib_config.h"
#if defined ( CPPLIB_WINDOWS )
#include "windows/mutex.hpp"
#endif

namespace cpplib
{
class auto_mutex_lock
	{
	private:
		mutex &mutex_reference;
	public:
		auto_mutex_lock( mutex &new_mutex ) :mutex_reference( new_mutex )
			{
			mutex_reference.lock();
			}
		~auto_mutex_lock(void)
			{
			mutex_reference.unlock();
			}
	};
}
