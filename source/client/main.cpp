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

	// Either run the client, the server, or from local filesystem
	if( arg1 == "client" )
		return runClient(argc-1,argv+1);
	else if ( arg1 == "server" )
		return runServer(argc-1,argv+1);
	else
		return runFromFilesystem(argc,argv);

};
