
# ====================== tests =======================
# Project name
project( ttrts-test )

include_directories( 
	../game 
	../maths
)

set( SOURCES 
	test.cpp
)

# Set to use c++11, because we're cool like that
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11" )

# Add the executable
add_executable( ttrts-test ${SOURCES} )

target_link_libraries( ttrts-test game )