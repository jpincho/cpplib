#pragma once
#include "cpplib_config.h"
#if defined ( CPPLIB_TARGET_PLATFORM_WINDOWS )
#include "windows/semaphore.hpp"
#else
#include "linux/semaphore.hpp"
#endif
