cmake_minimum_required( VERSION 2.8.7 )

# Set version information
set( TTRTS_VERSION_MAJOR 0 )
set( TTRTS_VERSION_MINOR 3 )
set( TTRTS_VERSION_PATCH 0 )

# Use c++1y (14)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++1y" )

# Turn on all warnings
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-reorder" )

# Turn off reorder warnings as they're kind of irrelevant
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder" )

# This shouldn't be needed, but it looks like IDE's like clion can forget to set -g for Debug
if( CMAKE_BUILD_TYPE MATCHES "Debug" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
endif()

# Add definitions for the version number
add_definitions(
	-DTTRTS_VERSION_MAJOR=${TTRTS_VERSION_MAJOR}
	-DTTRTS_VERSION_MINOR=${TTRTS_VERSION_MINOR}
	-DTTRTS_VERSION_PATCH=${TTRTS_VERSION_PATCH}
	-DTTRTS_VERSION_STRING=\"${TTRTS_VERSION_MAJOR}.${TTRTS_VERSION_MINOR}.${TTRTS_VERSION_PATCH}\"
)


# Subprojects
add_subdirectory( ttrts )
add_subdirectory( client )

# Auxhilary binaries
add_subdirectory( test )
add_subdirectory( gen )
