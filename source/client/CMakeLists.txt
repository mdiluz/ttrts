# ====================== ttrts =======================
# Project name
project( ttrts-client )

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
	../ttrts
)

# Add the sources
set( SOURCES
	main.cpp
	client.cpp
	server.cpp
	net.cpp
)

# Set defaults for ttrts variables
set( TTRTS_MAPS "/usr/local/share/ttrts/maps/" )
set( TTRTS_GAMES "/tmp/" )
set( TTRTS_PORT 11715 )

# define these defaults in code
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTTRTS_MAPS=${TTRTS_MAPS}" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTTRTS_GAMES=${TTRTS_GAMES}" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTTRTS_PORT=${TTRTS_PORT}" )

# Add the executable
add_executable( ${PROJECT_NAME} ${SOURCES} )

# Set our output name to ttrts
set_target_properties( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME ttrts )

# dependent on main ttrts libary
target_link_libraries( ${PROJECT_NAME} ttrts )

# Installation target
install( TARGETS ${PROJECT_NAME} DESTINATION bin )

# Run the gen_usage script to generate our usage header
add_custom_target(
	ttrts-client-usage
    cd ${CMAKE_CURRENT_SOURCE_DIR} && ${CMAKE_SOURCE_DIR}/scripts/gen_usage.sh "${CMAKE_CURRENT_BINARY_DIR}/usage.h"
)

add_dependencies(${PROJECT_NAME} ttrts-client-usage)
