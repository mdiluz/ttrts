#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <sys/stat.h>

#include "game.h"

static const char* sk_usage =
"NAME\n"
"\tttrts - Tiny Terminal RTS\n"
"\n"
"SYNOPSYS\n"
"\tttrts [OPTIONS...] MAPFILE\n"
"\n"
"DESCRIPTION\n"
"\tttrts is a tiny terminal based RTS where that uses text\n"
"\tfiles as order lists to control it's units.\n"
"\n"
"\tThis means that any user, program or cat that can read\n"
"\tand write to text files can play the game.\n"
"\n"
"USAGE\n"
"\tWhen invoked, ttrts will set up a full game and output a\n"
"\tsingle file representing the current gamestate into a \n"
"\tlocal directory called ttrts_{GAME_NAME}.\n"
"\n"
"\tThis file can be read in and interpretted by human, robot\n"
"\tor cat. ttrts will wait for orders files to be placed in \n"
"\tit's current working directory.\n"
"\n"
"\tOnce orders have been set for each player taking part \n"
"\tttrts will calculate the new game state and output a new \n"
"\tgamestate file for the next turn.\n"
"\n"
"\tThis process repeats until a winner is chosen!\n"
"\n"
"OPTIONS\n"
"\tMAPFILE:\n"
"\t\tFile to read in the initial game state from\n"
"\n"
"GAMESTATE FILE FORMAT\n"
"\tName\n"
"\t   Turn_{TURN_NUMBER}.txt\n"
"\tContents\n"
"\t   ===== {GAME_NAME} =====\n"
"\t   SIZE:[{X},{Y}]\n"
"\t   TURN:{TURN_NUMBER}\n"
"\t   ~~~~\n"
"\t   UNIT:{ID} tm:{TEAM} vs:{VIS} dr:{DIR(NESW)} ps:[{X},{Y}]\n"
"\t   ...\n"
"\n"
"ORDER FILE FORMAT\n"
"\tName\n"
"\t    Turn_{TURN_NUMBER}_Team_{TEAM_NUMBER}.txt\n"
"\tContents\n"
"\t    ORDER:{ORDER_CHAR} id:{UNIT_ID}\n"
"\t    ...\n"
"\n"
"ORDERS\n"
"\tF   - Move unit forward one space\n"
"\tL/R - Rotate unit left or right\n"
"\tA   - Attack space in front of unit\n";

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

// Main program entry point
int main(int argc, char* argv[])
{
	// If no args, print usage
	if ( argc == 1 )
	{
		std::cerr<<sk_usage<<std::endl;
		return 1;
	}

	// Attempt to open the game file
	std::string gameFile = argv[1];
	std::ifstream file(gameFile);

	if( file.bad() )
	{
		std::cerr<<"Error: "<<gameFile<<" file not found"<<std::endl;
		return 1;
	}

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
		return 1;
	}

	// Create the game
	CTTRTSGame game = CTTRTSGame::CreateFromString(gameDescriptor);

	// Grab the teams involved
	auto teams = game.GetTeams();

	// Current game directory
	std::string gameDir = "ttrts_" + game.GetName();

	// Remove the current game directory
	char cmd[128];
	snprintf(cmd,128, "test -e %s && rm -rf %s",gameDir.c_str(),gameDir.c_str());
	system(cmd);

	// Create the game directory
	char cmd2[128];
	snprintf(cmd2,128, "mkdir %s",gameDir.c_str());
	system(cmd2);

	// While the game hasn't been won
	Team winningTeam = Team::NUM_INVALID;
	while ( (winningTeam = game.CheckForWin()) == Team::NUM_INVALID )
	{
		// Create a turn file
		char turnFileName[128];
		snprintf(turnFileName,128,"ttrts_%s/Turn_%i.txt",game.GetName().c_str(),game.GetTurn());
		std::ofstream turnFile(turnFileName,std::ios::trunc); // truncate to overwrite if a file exists

		if ( turnFile.bad() )
		{
			std::cerr<<"Error: Failed to open new turn file "<< turnFileName <<std::endl;
			return 1;
		}

		// Output the turn description
		std::string turnDescriptor = game.GetStateAsString();
		turnFile<<turnDescriptor;
		turnFile.close();

		// Wait for order files
		for( Team team : teams )
		{
			// Construct the team order filename
			char teamOrderFileName[128];
			snprintf(teamOrderFileName, 128, "ttrts_%s/Turn_%i_Team_%i.txt", game.GetName().c_str(), game.GetTurn(), (int) team);

			// Wait for the team order file to be created
			std::cout<<"Waiting for "<<teamOrderFileName<<std::endl;
			WaitForFile(teamOrderFileName,sk_waitTime); // Wait for the file

			std::ifstream turnFile(teamOrderFileName);

			// Reserve the full order string
			std::string orders;
			turnFile.seekg(0, std::ios::end);
			orders.reserve(turnFile.tellg());
			turnFile.seekg(0, std::ios::beg);

			// Grab the string from the file
			orders.assign((std::istreambuf_iterator<char>(turnFile)),std::istreambuf_iterator<char>());

			// Issue the orders to the game
			if( game.IssueOrders(team, orders) )
				std::cerr<<"Warning: Orders for team "<<(int)team<<" failed to correctly parse"<<std::endl;
		}

		// Simulate turn
		std::cerr<<"Simulating next turn"<<std::endl;
		if ( game.SimulateToNextTurn() )
		{
			std::cerr << "Error: Failed to simulate next Turn "<<game.GetTurn()<<std::endl;
			return 1;
		}
	}

	// Print the winner!
	std::cout<<"TTRTS: Game over! Winner:"<<(int)winningTeam<<std::endl;

	return 0;
};
