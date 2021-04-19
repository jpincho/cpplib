project ( cpplib )
cmake_minimum_required ( VERSION 3.14 )

set ( CMAKE_CXX_STANDARD 11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )

file ( GLOB SOURCES "*.h" string.cpp )
add_library ( cpplib ${SOURCES} )

enable_testing()
add_executable ( array_tests "array_tests.cpp" )
add_test ( NAME "array_tests" COMMAND array_tests )

add_executable ( list_tests "list_tests.cpp" )
add_test ( NAME "list_tests" COMMAND list_tests )

add_executable ( string_tests "string_tests.cpp" )
target_link_libraries ( string_tests cpplib )
add_test ( NAME "string_tests" COMMAND string_tests )

add_executable ( ring_buffer_tests "ring_buffer_tests.cpp" )
add_test ( NAME "ring_buffer_tests" COMMAND ring_buffer_tests )

add_executable ( vectorized_container_tests "vectorized_container_tests.cpp" )
add_test ( NAME "vectorized_container_tests" COMMAND vectorized_container_tests )

