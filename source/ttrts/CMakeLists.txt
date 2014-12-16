# ====================== ttrts =======================
# Project name
project( ttrts )

include_directories(
	../maths
	../game
)

# Add the sources 
set( SOURCES 
	main.cpp
)

# Add the executable
add_executable( ttrts ${SOURCES} )

target_link_libraries( ttrts game )