cmake_minimum_required ( VERSION 2.8 FATAL_ERROR )

project ( "HPCPM-backend" )

macro ( Find_lib var name )
	message ( STATUS "Searching for ${var} ..." )
	find_library ( ${var} ${name} )
	if ( EXISTS ${${var}} )
		message ( STATUS "\tFound  at: ${${var}}" )
	else ()
		message ( FATAL_ERROR "${var} not found!" )
	endif ()
endmacro ()

# Clear CMAKE compilation flags - we will set them manually later on
set ( CMAKE_CXX_FLAGS_DEBUG "" )
set ( CMAKE_CXX_FLAGS_RELEASE "" )

set ( GCC_EXTENSIVE_ERROR_CHECKS "-Werror -Wfatal-errors -Wall -Wextra -pedantic -pedantic-errors -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith -Wstrict-overflow=5 -Wcast-qual -Wunreachable-code -Wwrite-strings -Wuninitialized -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls -fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing -Winit-self" )
# -Winline -Wundef -Wshadow

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_EXTENSIVE_ERROR_CHECKS}" )

include_directories ( ${CMAKE_SOURCE_DIR}/include )
include_directories ( ${CMAKE_SOURCE_DIR}/src )

# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module%3aFindBoost
find_package ( Boost )
if ( Boost_FOUND )
	include_directories ( ${Boost_INCLUDE_DIRS} )
else ()
	message ( FATAL_ERROR "Could not find Boost" )
endif()

file ( GLOB_RECURSE SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp" )

set ( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/lib/ )

if ( CMAKE_BUILD_TYPE STREQUAL "debug" OR CMAKE_BUILD_TYPE STREQUAL "test" )
	set ( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/lib/debug/ )
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3" )
elseif ( CMAKE_BUILD_TYPE STREQUAL "release" )
	set ( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/lib/release/ )
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG" )
else ()
	message ( FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}, legal values are: debug, release, test" )
endif ()

message ( STATUS "Configuring ${CMAKE_BUILD_TYPE} build..." )

Find_lib ( CASABLANCA cpprest )
Find_lib ( NVIDIA-ML nvidia-ml )
Find_lib ( PPLX pplx_test )

set ( SHARED_LIBS ${CASABLANCA} ${NVIDIA-ML} ${PPLX} )
set ( STATIC_LIBS )

if ( CMAKE_BUILD_TYPE STREQUAL "test" )
	file ( GLOB_RECURSE TESTS_SRCS "${CMAKE_SOURCE_DIR}/test/*.cpp" )
	list ( REMOVE_ITEM SRCS "${CMAKE_SOURCE_DIR}/src/main.cpp" )
	list ( APPEND SRCS ${TESTS_SRCS} )

	Find_lib( GTEST gtest_main )
	Find_lib( GMOCK gmock_main )
	list ( APPEND STATIC_LIBS ${GTEST} ${GMOCK} pthread )

	set ( EXECUTABLE "runUnitTests" )
else ()
	set ( EXECUTABLE "HPCPM-backend-daemon" )
endif ()

add_executable ( ${EXECUTABLE} ${SRCS} )
target_link_libraries ( ${EXECUTABLE} ${SHARED_LIBS} ${STATIC_LIBS} )

if ( INSTALL_LOCALLY STREQUAL "true" )
	set ( CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} )
endif ()

#TODO: Improve the installation procedure
install (	TARGETS ${EXECUTABLE}
			RUNTIME DESTINATION bin/${CMAKE_BUILD_TYPE}
			CONFIGURATIONS DEBUG RELEASE )

install (	FILES ${SHARED_LIBS}
			DESTINATION bin/${CMAKE_BUILD_TYPE}
			CONFIGURATIONS DEBUG RELEASE )

install (	DIRECTORY ${CMAKE_SOURCE_DIR}/res
			DESTINATION bin/${CMAKE_BUILD_TYPE}
			CONFIGURATIONS DEBUG RELEASE )
