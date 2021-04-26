#pragma once
#include "mutex_windows.h"

namespace cpplib
{
class auto_mutex
	{
	private:
		mutex &mutex_reference;
	public:
		auto_mutex( mutex &new_mutex ) :mutex_reference( new_mutex )
			{
			mutex_reference.lock();
			}
		~auto_mutex(void)
			{
			mutex_reference.unlock();
			}
	};
}