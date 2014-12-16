cmake_minimum_required( VERSION 2.8.7 )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11" )

if( CMAKE_BUILD_TYPE MATCHES "Debug" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
endif()

# Subprojects
add_subdirectory( ttrts )
add_subdirectory( game )
add_subdirectory( test )
add_subdirectory( ui )
