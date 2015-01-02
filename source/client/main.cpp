#include "game.h"
#include "filesystem.h"

#include <iostream>
#include <unistd.h>

#define STRINGIFY(x) _STRINGIFY(x)
#define _STRINGIFY(x) #x

static const char* sk_usage =
#include "usage.h"
;

// Main program entry point
int main(int argc, char* argv[])
{
	// If no args, print usage
	if ( argc == 1 )
	{
		std::cout<<sk_usage<<std::endl;
		return -1;
	}

	// Attempt to open the game file
	std::string gameFile = argv[1];

	// Default for maps
	std::string ttrts_maps_dir = STRINGIFY(TTRTS_MAPS);
	if( getenv("TTRTS_MAPS") )
	{
		ttrts_maps_dir = getenv("TTRTS_MAPS");

		// Additional trailing slash
		if( ttrts_maps_dir.back() != '/' )
			ttrts_maps_dir += "/";
	}

	// Default for games
	std::string ttrts_games_dir = STRINGIFY(TTRTS_GAMES);
	if( getenv("TTRTS_GAMES") )
	{
		ttrts_games_dir = getenv("TTRTS_GAMES");

		// Additional trailing slash
		if( ttrts_games_dir.back() != '/' )
			ttrts_games_dir += "/";
	}
	
	// If file path is not local path and file doesn't exist
	if( gameFile.find("/") == std::string::npos
		&& access( gameFile.c_str(), F_OK ) == -1 )
	{
		gameFile = ttrts_maps_dir + gameFile;
	}

	// If still not good
	if( access( gameFile.c_str(), F_OK ) == -1 )
	{
		std::cerr<<"Error: "<<gameFile<<" file not found"<<std::endl;
		return -1;
	}

	return runFromFilesystem(ttrts_games_dir, gameFile);

};
