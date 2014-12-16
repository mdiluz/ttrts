cmake_minimum_required(VERSION 2.8.7)

# game project
project( game )

include_directories(
	../maths
)

# Set to use c++11, because we're cool like that
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11" )

# Add the sources 
set( SOURCES 
	game.cpp
	board.cpp
)

add_library( game ${SOURCES} )