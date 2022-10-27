#pragma once
#if defined ( _MSC_VER )
#include <string.h>
#include <stdio.h>
#endif

namespace cpplib
{
class version
    {
    public:
        unsigned minor, major;

        version ( void )
            {
            major = minor = 0;
            }
        void set ( unsigned new_major, unsigned new_minor )
            {
            major = new_major;
            minor = new_minor;
            }
        bool parse_string ( const char *string )
            {
            // Safe values
            major = 0;
            minor = 0;

            const char *dot_position = strchr ( string, '.' );
            if ( dot_position == nullptr )
                return false;

            const char *version_start;
            for ( version_start = dot_position; ( version_start > string ) && ( *version_start != ' ' ); --version_start );
            if ( *version_start != ' ' ) // If there was nothing before the version numbers, assume the whole string
                version_start = string;

#if defined ( _MSC_VER )
            sscanf_s ( version_start, "%u.%u", &major, &minor );
#else
            sscanf ( version_start, "%u.%u", &major, &minor );
#endif
            return true;
            }
        bool operator < ( const version &other )
            {
            return lesser ( other.major, other.minor );
            }
        bool operator <= ( const version &other )
            {
            return lesser_equal ( other.major, other.minor );
            }
        bool operator > ( const version &other )
            {
            return greater ( other.major, other.minor );
            }
        bool operator >= ( const version &other )
            {
            return greater_equal ( other.major, other.minor );
            }
        bool lesser ( const unsigned other_major, const unsigned other_minor )
            {
            return ( ( major < other_major ) ||
                     ( ( major == other_major ) && ( minor < other_minor ) ) );
            }
        bool greater ( const unsigned other_major, const unsigned other_minor )
            {
            return ( ( major > other_major ) ||
                     ( ( major == other_major ) && ( minor > other_minor ) ) );
            }
        bool lesser_equal ( const unsigned other_major, const unsigned other_minor )
            {
            return ( equal ( other_major, other_minor ) ) || ( lesser ( other_major, other_minor ) );
            }
        bool greater_equal ( const unsigned other_major, const unsigned other_minor )
            {
            return ( equal ( other_major, other_minor ) ) || ( greater ( other_major, other_minor ) );
            }
        bool equal ( const unsigned other_major, const unsigned other_minor )
            {
            return ( ( major == other_major ) && ( minor == other_minor ) );
            }
    };
}
