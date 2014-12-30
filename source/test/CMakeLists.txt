
# ====================== tests =======================
# Project name
project( ttrts-test )

include_directories( 
	../ttrts 
	../maths
)

set( SOURCES 
	test.cpp
)

# Add the executable
add_executable( ttrts-test ${SOURCES} )

target_link_libraries( ttrts-test ttrts )
