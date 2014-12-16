cmake_minimum_required( VERSION 2.8.7 )

# Use c++1y (14)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++1y" )

# Turn on all warnings
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-reorder" )

# Turn off reorder warnings as they're kind of irrelevant
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder" )

if( CMAKE_BUILD_TYPE MATCHES "Debug" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
endif()

# Subprojects
add_subdirectory( ttrts )
add_subdirectory( game )
add_subdirectory( ui )

# Auxhilary binaries
add_subdirectory( test )
add_subdirectory( gen )
