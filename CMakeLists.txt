cmake_minimum_required ( VERSION 2.8 FATAL_ERROR )

#set ( CMAKE_VERBOSE_MAKEFILE ON )

project ( "HPCPM-backend" )

macro ( Find_lib var name )
	message ( STATUS "Searching for ${var}..." )
	find_library ( ${var} ${name} )
	if ( EXISTS ${${var}} )
		message ( STATUS "\tFound  at: ${${var}}" )
	else ()
		message ( FATAL_ERROR "${var} not found!" )
	endif ()
endmacro ()

#Locate and compile gtest and gmock, if googletest location is not specified try to use default one
if ( NOT DEFINED GOOGLETEST_LOCATION )
	set ( GOOGLETEST_LOCATION "${CMAKE_SOURCE_DIR}/extlibs/googletest" )
endif ()

if ( EXISTS "${GOOGLETEST_LOCATION}/googlemock/CMakeLists.txt" )
	#store flags values
	set ( ORIGINAL_CXX_FLAGS ${CMAKE_CXX_FLAGS} )

	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DGTEST_LANG_CXX11=1" )
	include_directories ( SYSTEM "${GOOGLETEST_LOCATION}/googletest/include" "${GOOGLETEST_LOCATION}/googlemock/include" )
	include_directories ( "${GOOGLETEST_LOCATION}/googletest" "${GOOGLETEST_LOCATION}/googlemock" )
	add_subdirectory( "${GOOGLETEST_LOCATION}/googlemock" "${CMAKE_CURRENT_BINARY_DIR}/googletest" )
	message ( STATUS "Found googletest at: ${GOOGLETEST_LOCATION}" )

	#restore old flags
	set ( CMAKE_CXX_FLAGS ${ORIGINAL_CXX_FLAGS} )
else ()
	message ( FATAL_ERROR "googletest not found, set GOOGLETEST_LOCATION variable" )
endif ()

#Locate boost
find_package ( Boost )
if ( Boost_FOUND )
	include_directories ( ${Boost_INCLUDE_DIRS} )
else ()
	message ( FATAL_ERROR "Could not find Boost" )
endif()

#Add include directories
include_directories ( "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/src" )

#Find all source files
file ( GLOB_RECURSE SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp" )

#Set compilation flags
set ( CMAKE_CXX_FLAGS_DEBUG "" )
set ( CMAKE_CXX_FLAGS_RELEASE "" )
set ( GCC_EXTENSIVE_ERROR_CHECKS "-Werror -Wfatal-errors -Wall -Wextra -pedantic -pedantic-errors -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith -Wstrict-overflow=5 -Wcast-qual -Wunreachable-code -Wwrite-strings -Wuninitialized -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls -fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing -Winit-self" )
	# -Winline -Wundef -Wshadow
#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_FORTIFY_SOURCE=1" ) TODO: analyze why FORTIFY_SOURCE rises redefinition error on quickbuild
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_EXTENSIVE_ERROR_CHECKS}" )

if ( USE_MOCKS STREQUAL "true" )
	 set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_COMM_PROVIDERS_MOCKS" )
endif ()

#Add libraries location
set ( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/lib/ )

#Add compilation flags and library locations based on build type
if ( CMAKE_BUILD_TYPE STREQUAL "debug" OR CMAKE_BUILD_TYPE STREQUAL "test" )
	set ( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/lib/debug/ )
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3" )
elseif ( CMAKE_BUILD_TYPE STREQUAL "release" )
	set ( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/lib/release/ )
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG" )
else ()
	message ( FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}, legal values are: debug, release, test" )
endif ()

#Find libraries
Find_lib ( CASABLANCA cpprest )
Find_lib ( NVIDIA-ML nvidia-ml )
Find_lib ( PPLX pplx_test )
Find_lib ( MICMGMT micmgmt )
Find_lib ( SCIF scif )
set ( SHARED_LIBS ${CASABLANCA} ${NVIDIA-ML} ${PPLX} ${SCIF} ${MICMGMT} )


Find_lib ( NMPRK_IPMI NmprkIpmi )
Find_lib ( NMPRK_TRANSLATION NmprkTranslation )
list ( APPEND STATIC_LIBS ${NMPRK_IPMI} ${NMPRK_TRANSLATION} )

if ( CMAKE_BUILD_TYPE STREQUAL "test" )
	file ( GLOB_RECURSE TESTS_SRCS "${CMAKE_SOURCE_DIR}/test/*.cpp" )
	list ( REMOVE_ITEM SRCS "${CMAKE_SOURCE_DIR}/src/main.cpp" )
	list ( APPEND SRCS ${TESTS_SRCS} )
	list ( APPEND STATIC_LIBS gmock )
	set ( EXECUTABLE "runUnitTests" )
else ()
	set ( EXECUTABLE "HPCPM-backend-daemon" )
endif ()

#Build executable
add_executable ( ${EXECUTABLE} ${SRCS} )
target_link_libraries ( ${EXECUTABLE} ${STATIC_LIBS} ${SHARED_LIBS} )

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
