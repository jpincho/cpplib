#pragma once
#if defined ( _MSC_VER )
#include <windows.h>
#endif

namespace cpplib
{
class mutex
    {
    private:
        CRITICAL_SECTION handle;
        bool created;
        mutex &operator = ( const mutex &other ) = delete;
    public:
        mutex ( void );
        bool create ( void );
        bool destroy ( void );
        bool lock ( void );
        bool try_lock ( void );
        bool unlock ( void );
    };
}
