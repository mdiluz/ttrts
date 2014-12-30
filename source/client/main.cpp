#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include <sys/stat.h>
#include <stdio.h>
#include <stdbool.h>
#include <formatters.h>
#include <unistd.h>
#include <stdlib.h>

#include "game.h"

static const char* sk_usage =
#include "usage.h"
;
 
// Verbose mode
static const bool env_verbose = getenv("VERBOSE");

// time for waiting between file stats
static const std::chrono::milliseconds sk_waitTime = std::chrono::milliseconds(100);

// Check if a file exists
inline bool FileExists( const std::string& name )
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

// Wait for a file to exist
inline void WaitForFile( const std::string& name, const std::chrono::milliseconds& time )
{
	while( !FileExists(name) ) std::this_thread::sleep_for(time);
}

bool OutputGameStateFile(CTTRTSGame &game, std::string &gameDir)
{
	char turnFileName[128];
	snprintf(turnFileName,128,"%s/Turn_%i.txt",gameDir.c_str(),game.GetTurn());
	std::ofstream turnFile(turnFileName, std::ios_base::trunc); // truncate to overwrite if a file exists

	if ( turnFile.bad() )
	{
		return false;
	}

	// Output the turn description
	std::string turnDescriptor = GetStringFromGame(game);

	turnFile<<turnDescriptor;
	turnFile.close();

	return true;
}

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
	std::string ttrts_maps_dir = "/usr/share/ttrts/maps/";
	if( getenv("TTRTS_MAPS") )
	{
		ttrts_maps_dir = getenv("TTRTS_MAPS");

		// Additional trailing slash
		if( ttrts_maps_dir.back() != '/' )
			ttrts_maps_dir += "/";
	}

	// Default for games
	std::string ttrts_games_dir = "/tmp/";
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

	std::ifstream file(gameFile);

	std::cout<<"Launching TTRTS with "<<gameFile<<std::endl;

	std::string gameDescriptor;

	// Reserve the string needed up front
	file.seekg(0, std::ios::end);
	gameDescriptor.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	// Grab the string from the file
	gameDescriptor.assign((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());

	if( gameDescriptor.size() == 0 )
	{
		std::cerr<<"Error: failed to read in any information from "<<gameFile<<std::endl;
		return -1;
	}

	// Create the game
	CTTRTSGame game = GetGameFromString(gameDescriptor);

	// Grab the players involved
	auto players = game.GetPlayers();

	// Current game directory
	std::string gameDir = ttrts_games_dir + game.GetName();

	// Empty the current game directory
	struct stat info;
	int ret = stat( gameDir.c_str(), &info );
	if( ret == 0 && info.st_mode & S_IFDIR )
	{
		std::cout<< gameDir << " game directory already exists"<<std::endl;
		std::cout<<"Confirm to delete contents [y/N] ";
		std::string input;
		std::cin>>input;
		if( !input.size() || std::tolower(input[0]) != 'y' )
		{
			std::cerr<<"Aborting..."<<std::endl;
			return -1;
		}
	}
	else if ( ret  == 0 )
	{
		std::cerr<< gameDir << " exists but is not directory \nAborting..."<<std::endl;
		return -1;
	}

	// Create the game directory
	char cmd2[128];
	snprintf(cmd2,128, "test -d %s || mkdir %s",gameDir.c_str(),gameDir.c_str());
	if( system(cmd2) == -1) 
	{
		std::cerr<<"Error: Failed to create the game directory"<<std::endl;
		return -1;
	}

	// Clean out the game directory
	char cmd1[128];
	snprintf(cmd1,128, "rm -rf %s/*",gameDir.c_str());
	if ( system(cmd1) == -1 )
	{
		std::cerr<<"Error: Failed to clean the game directory"<<std::endl;
		return -1;
	}

	// While the game isn't finished
	while ( ! game.GameOver() )
	{
		std::cout<<"Starting turn "<<game.GetTurn()<<std::endl;

		// Create a turn file
		if( !OutputGameStateFile(game, gameDir))
		{
			std::cerr<<"Error: Failed to output new turn file" << std::endl;
			return 1;
		}

		// Wait for order files
		for( player_t player : players)
		{
			// Construct the player order filename
			char playerOrderFileName[128];
			snprintf(playerOrderFileName, 128, "%s/Player_%i_Turn_%i.txt", gameDir.c_str(), (int) player, game.GetTurn());

			// Wait for the player order file to be created
			std::cout<<"Waiting for "<< playerOrderFileName <<std::endl;
			bool hasOrderFile = false;
			while(!hasOrderFile)
			{
				WaitForFile(playerOrderFileName,sk_waitTime); // Wait for the file

				// File must have END
				// Method taken from http://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file
				std::ifstream turnFile(playerOrderFileName);
				turnFile.seekg(-1,std::ios_base::end);

				// Loop back from the end of file
				bool keepLooping = true;
				while(keepLooping) {
					char ch;
					turnFile.get(ch);                            // Get current byte's data

					if((int)turnFile.tellg() <= 1) {             // If the data was at or before the 0th byte
						turnFile.seekg(0);                       // The first line is the last line
						keepLooping = false;                // So stop there
					}
					else if(ch == '\n') {                   // If the data was a newline
						keepLooping = false;                // Stop at the current position.
					}
					else {                                  // If the data was neither a newline nor at the 0 byte
						turnFile.seekg(-2,std::ios_base::cur);        // Move to the front of that data, then to the front of the data before it
					}
				}

				// Grab this line
				std::string lastLine;
				std::getline(turnFile,lastLine);
				if(lastLine == "END")
					hasOrderFile = true;
			}

			std::ifstream turnFile(playerOrderFileName);

			// Reserve the full order string
			std::string orders;
			turnFile.seekg(0, std::ios::end);
			orders.reserve(turnFile.tellg());
			turnFile.seekg(0, std::ios::beg);

			// Grab the string from the file
			orders.assign((std::istreambuf_iterator<char>(turnFile)),std::istreambuf_iterator<char>());

			// Issue the orders to the game
			if( game.IssueOrders(player, orders) )
				std::cerr<<"Warning: Orders for player "<<(int) player <<" failed to correctly parse"<<std::endl;
		}

		// Simulate turn
		std::cout<<"Simulating this turn!"<<std::endl;
		if ( game.SimulateToNextTurn() )
		{
			std::cerr << "Error: Failed to simulate for turn "<<game.GetTurn()<<std::endl;
			return -1;
		}

	}

	// Output final gamestate
	OutputGameStateFile(game, gameDir);

	// Get the winning player
	player_t winningPlayer = game.GetWinningPlayer();

	// Print the winner!
	if ( winningPlayer != player_t::NUM_INVALID )
	{
		std::cout<<"Game over! Winner:"<<(int) winningPlayer <<std::endl;
	}
	else
	{
		std::cout<<"Game over! It was a draw!"<<std::endl;
	}

	return (int)winningPlayer;
};
