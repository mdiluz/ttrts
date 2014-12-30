# ====================== ttrts =======================
# Project name
project( ttrts-client )

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	../maths
	../ttrts
)

# Add the sources
set( SOURCES
	main.cpp
)

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