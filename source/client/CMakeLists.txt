# ====================== ttrts =======================
# Project name
project( ttrts-client )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ../system
	../ttrts
)

# Add the sources
set( SOURCES
	client.cpp
)

# Add the executable
add_executable( ${PROJECT_NAME} ${SOURCES} )

# dependent on main ttrts libary
target_link_libraries( ${PROJECT_NAME} ttrts ttrts-system )

# Installation target
install( TARGETS ${PROJECT_NAME} DESTINATION bin )
