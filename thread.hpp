#pragma once
#include "cpplib_config.h"
#if defined ( CPPLIB_TARGET_PLATFORM_WINDOWS )
#include "windows/thread.hpp"
#elif defined ( CPPLIB_TARGET_PLATFORM_LINUX )
#include "linux/thread.hpp"
#endif
