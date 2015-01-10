cmake_minimum_required( VERSION 2.8.7 )

# Set version information
set( TTRTS_VERSION_MAJOR 0 )
set( TTRTS_VERSION_MINOR 4 )
set( TTRTS_VERSION_PATCH 0 )

# Set defaults for ttrts variables
set( TTRTS_MAPS 	"/usr/local/share/ttrts/maps/" )
set( TTRTS_GAMES 	"/tmp/" )
set( TTRTS_PORT 	11715 )

# define these defaults in code
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTTRTS_MAPS=${TTRTS_MAPS}" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTTRTS_GAMES=${TTRTS_GAMES}" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTTRTS_PORT=${TTRTS_PORT}" )

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
    ${CMAKE_SOURCE_DIR}/scripts/gen_manpage.sh "${TTRTS_VERSION_MAJOR}" "${TTRTS_VERSION_MINOR}" "${TTRTS_VERSION_PATCH}" "ttrts.6" "${CMAKE_SOURCE_DIR}/source/README.md"
)

# Install the ttrts man page
if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set ( MANPAGE_LOC share/man/man6 )
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set ( MANPAGE_LOC man/man6 )
else()
    message(ERROR "Unsupported system detected")
endif()

install( FILES "${CMAKE_BINARY_DIR}/ttrts.6" DESTINATION ${MANPAGE_LOC} )

# Subprojects
add_subdirectory( source )
