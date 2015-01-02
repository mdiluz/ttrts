cmake_minimum_required(VERSION 2.8.7)

# Main ttrts library
project( ttrts )

# Include the maths
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Add our sources 
set( SOURCES 
	game.cpp
	unit.cpp
	formatters.cpp
)

# Add this library
add_library( ttrts ${SOURCES} )

add_custom_target(
	ttrts-version-header ALL
	${CMAKE_SOURCE_DIR}/scripts/gen_version_header.sh ${TTRTS_VERSION_MAJOR} ${TTRTS_VERSION_MINOR} ${TTRTS_VERSION_PATCH}
)

# ttrts is dependent on this
add_dependencies( ${PROJECT_NAME} ttrts-version-header )

# Install headers
install( 
	FILES 
	game.h
	unit.h
	order.h
	formatters.h
	vector2.h
	orderunitpair.h
	gametypes.h
	${CMAKE_CURRENT_BINARY_DIR}/version.h

	DESTINATION 
	include/ttrts 
)

# Install the ttrts static lib
install( TARGETS ttrts DESTINATION lib )
