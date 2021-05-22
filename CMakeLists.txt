project ( cpplib )
cmake_minimum_required ( VERSION 3.14 )

set ( CMAKE_CXX_STANDARD 11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )

file ( GLOB SOURCES "*.cpp" "*.hpp" "filesystem/*.cpp" "filesystem/*.hpp" )

string ( TOUPPER ${CMAKE_SYSTEM_NAME} UPPERCASE_SYSTEM_NAME )
set ( CPPLIB_HOST_PLATFORM ${UPPERCASE_SYSTEM_NAME} )
set ( CPPLIB_TARGET_PLATFORM ${UPPERCASE_SYSTEM_NAME} )

set ( CPPLIB_HOST_PLATFORM_${CPPLIB_HOST_PLATFORM} TRUE )
set ( CPPLIB_TARGET_PLATFORM_${CPPLIB_TARGET_PLATFORM} TRUE )

if ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    set ( CPPLIB_COMPILER_CLANG TRUE )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    set ( CPPLIB_COMPILER_GNU TRUE )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
    set ( CPPLIB_COMPILER_MSVC TRUE )
endif()

message ( STATUS "${PROJECT_NAME} - Building under ${CMAKE_SYSTEM_NAME}")
if ( CPPLIB_TARGET_PLATFORM_WINDOWS )
    message ( STATUS "${PROJECT_NAME} - Building for Windows")
elseif ( CPPLIB_TARGET_PLATFORM_LINUX )
    message ( STATUS "${PROJECT_NAME} - Building for Linux")
endif()

if ( MSVC )
	file ( GLOB THREAD_SOURCES "windows/*" )
	list ( APPEND SOURCES ${THREAD_SOURCES} )
endif()
configure_file ( cpplib_config.h.in ${CMAKE_CURRENT_LIST_DIR}/cpplib_config.h )
add_library ( cpplib ${SOURCES} cpplib_config.h )

enable_testing()
add_executable ( array_tests "tests/array_tests.cpp" )
add_test ( NAME "array_tests" COMMAND array_tests )

add_executable ( list_tests "tests/list_tests.cpp" )
add_test ( NAME "list_tests" COMMAND list_tests )

add_executable ( string_tests "tests/string_tests.cpp" )
target_link_libraries ( string_tests cpplib )
add_test ( NAME "string_tests" COMMAND string_tests )

add_executable ( ring_buffer_tests "tests/ring_buffer_tests.cpp" )
add_test ( NAME "ring_buffer_tests" COMMAND ring_buffer_tests )

add_executable ( vectorized_container_tests "tests/vectorized_container_tests.cpp" )
add_test ( NAME "vectorized_container_tests" COMMAND vectorized_container_tests )

