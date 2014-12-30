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

# Add bash completion to install
install( FILES scripts/ttrts_complete DESTINATION /etc/bash_completion.d/ )

# Add target to generate man page
# Run the gen_usage script to generate our usage header
add_custom_target(
	ttrts-gen-manpage ALL
    ${CMAKE_SOURCE_DIR}/scripts/gen_manpage.sh "${TTRTS_VERSION_MAJOR}" "${TTRTS_VERSION_MINOR}" "${TTRTS_VERSION_PATCH}" "ttrts.6" "${CMAKE_SOURCE_DIR}/source/client/README.md"
)

# Install the ttrts man page
install( FILES "${CMAKE_BINARY_DIR}/ttrts.6" DESTINATION man/man6/ )

# Subprojects
add_subdirectory( source/ttrts )
add_subdirectory( source/client )

# Auxhilary binaries
add_subdirectory( source/test )
add_subdirectory( source/gen )
