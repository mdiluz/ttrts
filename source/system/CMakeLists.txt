cmake_minimum_required(VERSION 2.8.7)

# Main ttrts library
project( ttrts-system )

# Include the maths
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ../ttrts
)

# Add our sources 
set( SOURCES 
	net.cpp
	filesystem.cpp
)

# Add this library
add_library( ${PROJECT_NAME} ${SOURCES} )

target_link_libraries( ${PROJECT_NAME} ttrts pthread )
