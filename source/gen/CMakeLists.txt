# ====================== gen =======================
# Project name
project( ttrts-gen )

include_directories(
	../ttrts
	../maths
)

set( SOURCES
	gen.cpp
)

# Add the executable
add_executable( ttrts-gen ${SOURCES} )

target_link_libraries( ttrts-gen ttrts )

# Run the gen_usage script to generate our usage header
add_custom_target(
	ttrts-gen-maps ALL
    ${CMAKE_SOURCE_DIR}/scripts/gen_maps.sh "${CMAKE_CURRENT_BINARY_DIR}/ttrts-gen"
)

add_dependencies(ttrts-gen-maps ${PROJECT_NAME})

# Installation target
install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/maps DESTINATION /usr/local/share/ttrts )