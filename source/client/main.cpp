#include "game.h"
#include "filesystem.h"
#include "server.h"
#include "client.h"

#include <iostream>

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
	std::string arg1 = argv[1];

	if( arg1 == "client" )
	{
		if( argc == 3 )
			return runClient(argv[2]);
	}
	else if ( arg1 == "server" )
	{
		return runServer();
	}
	else
		return runFromFilesystem(arg1);

};
