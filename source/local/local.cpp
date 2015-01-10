#include "game.h"
#include "filesystem.h"
#include "error.h"
#include "net.h"

#include <iostream>

// =====================================================================================================================
int main(int argc, char* argv[])
{
	// must provide information
    if (argc < 2)
        fatal_error("Usage: ttrts-local MAPFILE");

    std::string gamefile = argv[1];

    std::cout<<"TTRTS: Launching with "<<gamefile<<std::endl;
    CTTRTSGame game = GetGameFromFile(gamefile);

    // Grab the players involved
    auto players = game.GetPlayers();

    // Default for games
    std::string ttrts_games_dir = getGamesDir();

    // Empty the current game directory
    if ( CreateAndCleanGameDir(game.GetName()) < 0)
        return -1;

    // While the game isn't finished
    while ( ! game.GameOver() )
    {
        std::cout<<"TTRTS: Starting turn "<<game.GetTurn()<<std::endl;

        // Create a turn file
        if( !OutputGameStateFile(game))
            fatal_error("Error: Failed to output new turn file");

        // Wait for order files
        for( player_t player : players)
        {
            // Construct the player order filename
            std::string orders = GetOrdersFromPlayerFile(game, player);

            // Issue the orders to the game
            if( game.IssueOrders(player, orders) )
                std::cerr<<"Warning: Orders for player "<<(int) player <<" failed to correctly parse"<<std::endl;
        }

        // Simulate turn
        std::cout<<"TTRTS: Simulating this turn!"<<std::endl;
        if ( game.SimulateToNextTurn() )
            fatal_error("Failed to simulate game turn");

    }

    // Output final gamestate
    OutputGameStateFile(game);

    return OutputGameEnd( game );
}

