# ====================== gen =======================
# Project name
project( ttrts-gen )

include_directories(
	../game
	../maths
)

set( SOURCES
	gen.cpp
)

# Add the executable
add_executable( ttrts-gen ${SOURCES} )

target_link_libraries( ttrts-gen game )