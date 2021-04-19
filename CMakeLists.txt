project ( cpplib )
cmake_minimum_required ( VERSION 3.14 )

set ( CMAKE_CXX_STANDARD 11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )

file ( GLOB HEADERS "*.h" )

enable_testing()
add_executable ( array_tests "array_tests.cpp" )
add_test ( NAME "Array tests" COMMAND array_tests )

add_executable ( list_tests "list_tests.cpp" )
add_test ( NAME "List tests" COMMAND list_tests )

add_executable ( string_tests "string_tests.cpp" )
add_test ( NAME "String tests" COMMAND string_tests )
