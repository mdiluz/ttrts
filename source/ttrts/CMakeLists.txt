cmake_minimum_required(VERSION 2.8.7)

# Main ttrts library
project( ttrts )

# Include the maths
include_directories(
	../maths
)

# Add our sources 
set( SOURCES 
	game.cpp
	unit.cpp
	formatters.cpp
)

# Add this library
add_library( ttrts ${SOURCES} )