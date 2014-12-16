# ====================== ttrts =======================
# Project name
project( ttrts )

# Add the sources 
set( SOURCES 
	main.cpp
)

# Set to use c++11, because we're cool like that
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11" )

# Add the executable
add_executable( ttrts ${SOURCES} )