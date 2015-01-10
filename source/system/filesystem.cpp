#include "filesystem.h"
#include "net.h"
#include "error.h"


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

// =====================================================================================================================
// time for waiting between file stats
static const std::chrono::milliseconds sk_waitTime = std::chrono::milliseconds(100);

// Check if a file exists
bool FileExists( const std::string& name )
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

// Wait for a file to exist
void WaitForFile( const std::string& name, const std::chrono::milliseconds& time )
{
    while( !FileExists(name) ) std::this_thread::sleep_for(time);
}

bool OutputGameStateFile(CTTRTSGame &game)
{
    char turnFileName[128];
    snprintf(turnFileName,128,"%s%s/Turn_%i.txt", getGamesDir().c_str(),game.GetName().c_str(),game.GetTurn());
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

std::string getMapsDir()
{
    std::string maps = STRINGIFY(TTRTS_MAPS);
    if( getenv("TTRTS_MAPS") )
    {
        maps = getenv("TTRTS_MAPS");

        // Additional trailing slash
        if( maps.back() != '/' )
            maps += "/";
    }

    return maps;
}

std::string getGamesDir()
{
    std::string dir = STRINGIFY(TTRTS_GAMES);
    if( getenv("TTRTS_GAMES") )
    {
        dir = getenv("TTRTS_GAMES");

        // Additional trailing slash
        if( dir.back() != '/' )
            dir += "/";
    }
    return dir;
}

CTTRTSGame GetGameFromFile( const std::string& filename )
{
    std::string gamefile = filename;

    // Default for maps
    std::string ttrts_maps_dir = getMapsDir();

    // If file path is not local path and file doesn't exist
    if( gamefile.find("/") == std::string::npos
            && access( gamefile.c_str(), F_OK ) == -1 )
    {
        gamefile = ttrts_maps_dir + gamefile;
    }

    // If still not good
    if( access( gamefile.c_str(), F_OK ) == -1 )
        fatal_perror("Could not open game file");

    std::ifstream file(gamefile);

    std::string gameDescriptor;

    // Reserve the string needed up front
    file.seekg(0, std::ios::end);
    gameDescriptor.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    // Grab the string from the file
    gameDescriptor.assign((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());

    if( gameDescriptor.size() == 0 )
        fatal_error("failed to read in any information from gamefile");

    // Create the game
    return GetGameFromString(gameDescriptor);
}

std::string GetOrdersFromPlayerFile(const CTTRTSGame &game, player_t &player)
{
    std::string gameDir = getGamesDir();

    char playerOrderFileName[128];
    snprintf(playerOrderFileName, 128, "%s%s/Player_%i_Turn_%i.txt", gameDir.c_str(),game.GetName().c_str(),(int) player, game.GetTurn());

    // Wait for the player order file to be created
    std::clog<<"TTRTS: Waiting for "<< playerOrderFileName << std::endl;
    bool hasOrderFile = false;
    while(!hasOrderFile)
            {
                WaitForFile(playerOrderFileName,sk_waitTime); // Wait for the file

                // File must have END
                // Method taken from http://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file
                std::ifstream turnFile(playerOrderFileName);
                turnFile.seekg(-1, std::ios_base::end);

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
                        turnFile.seekg(-2, std::ios_base::cur);        // Move to the front of that data, then to the front of the data before it
                    }
                }

                // Grab this line
                std::string lastLine;
                getline(turnFile,lastLine);
                if(lastLine == "END")
                    hasOrderFile = true;
            }

    std::ifstream turnFile(playerOrderFileName);

    // Reserve the full order string
    std::string orders;
    turnFile.seekg(0, std::ios_base::end);
    orders.reserve(turnFile.tellg());
    turnFile.seekg(0, std::ios_base::beg);

    // Grab the string from the file
    orders.assign((std::istreambuf_iterator<char>(turnFile)), std::istreambuf_iterator<char>());
    return orders;
}

int CreateAndCleanGameDir(const std::string& gameName)
{
    std::string gameDir = getGamesDir()+gameName;
    struct stat info;
    int ret = stat( gameDir.c_str(), &info );
    if( ret == 0 && info.st_mode & S_IFDIR )
    {
        std::cout<<"TTRTS: " << gameDir << " game directory already exists"<<std::endl;
        std::cout<<"TTRTS: Confirm to delete contents [y/N] ";
        std::string input;
        std::cin>>input;
        if( !input.size() || std::tolower(input[0]) != 'y' )
            return -1;
    }
    else if ( ret  == 0 )
    {
        fatal_error("TTRTS_GAMES exists but is not directory \nAborting...");
    }

    // Create the game directory
    char cmd2[128];
    snprintf(cmd2,128, "test -d %s || mkdir %s",gameDir.c_str(),gameDir.c_str());
    if( system(cmd2) == -1)
        fatal_error("Error: Failed to create the game directory");

    // Clean out the game directory
    char cmd1[128];
    snprintf(cmd1,128, "rm -rf %s/*",gameDir.c_str());
    if ( system(cmd1) == -1 )
        fatal_error("Error: Failed to clean the game directory");

    return 0;
}

int OutputGameEnd(const CTTRTSGame &game) {
    std::cout<<"TTRTS: Game Over!"<< std::endl;

    // Get the winning player
    player_t winningPlayer = game.GetWinningPlayer();

    // Print the winner!
    if ( winningPlayer != player_t::NUM_INVALID )
    {
        std::cout<<"TTRTS: Winner:"<<(int) winningPlayer <<std::endl;
    }
    else
    {
        std::cout<<"TTRTS: It was a draw!"<<std::endl;
    }

    return (int)winningPlayer;
}

int OutputgameEnd(const CTTRTSGame &game) {
    std::cout<<"TTRTS: Game Over!"<< std::endl;

    // Get the winning player
    player_t winningPlayer = game.GetWinningPlayer();

    // Print the winner!
    if ( winningPlayer != player_t::NUM_INVALID )
    {
        std::cout<<"TTRTS: Winner:"<<(int) winningPlayer <<std::endl;
    }
    else
    {
        std::cout<<"TTRTS: It was a draw!"<<std::endl;
    }

    return (int)winningPlayer;
}