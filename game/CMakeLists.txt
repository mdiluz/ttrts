cmake_minimum_required(VERSION 2.8.7)

# game project
project( game )

include_directories(
	../maths
)

# Add the sources 
set( SOURCES 
	game.cpp
	unit.cpp
	orders.cpp
)

add_library( game ${SOURCES} )