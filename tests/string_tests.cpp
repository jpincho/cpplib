#include <stdio.h>
#include "../string.hpp"
#include <string.h>
#include <assert.h>

void string_test_assign ( void )
    {
    const char reference_value[] = __FUNCTION__;
    printf ( "%s\n", reference_value );
    cpplib::string string;
    string.assign ( reference_value );

    assert ( string.get_length () == strlen ( reference_value ) );
    assert ( strcmp ( string.get_data (), reference_value ) == 0 );
    }

void string_test_find ( void )
    {
    const char reference_value[] = __FUNCTION__;
    printf ( "%s\n", reference_value );
    cpplib::string string;
    string.assign ( reference_value );

    off_t position = string.find_first_of ( "abc_" );
    assert ( position == strchr ( reference_value, '_' ) - reference_value );

    position = string.find_first_of ( "xyz" );
    assert ( position == -1 );

    position = string.find_first_of ( "_r" );
    assert ( position == strchr ( reference_value, 'r' ) - reference_value );
    }

int main ()
    {
    string_test_assign();
    string_test_find();
    return 0;
    }
