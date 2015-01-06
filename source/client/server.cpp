#include "server.h"

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "net.h"
#include "filesystem.h"

void RunServerForGame(CTTRTSGame &game)
{
    std::cout<<"TTRTS: Setting up server"<<std::endl;

    // Server side information
    sockaddr_in serv_addr = GetLocalServerAddress();

    // socket File descriptor
    int sockfd = SetUpServerListeningSocket(serv_addr);

    // Get information about the game
    std::vector<player_t> players = game.GetPlayers();
    unsigned int numClients = players.size();
    auto player_iterator = players.begin();

    //  game mutex
    std::mutex gameMutex;

    // Set of clients
    std::vector<ClientInfo> myClients;

    std::cout<<"TTRTS: Waiting for "<<numClients<<" players"<<std::endl;

    // Loop while we're connecting the clients
    while ( myClients.size() < numClients )
    {
        // information for each client
        ClientInfo clientInfo;
        clientInfo.player = *player_iterator;
        player_iterator++;

        clientInfo = WaitForClientConnection(sockfd, game.GetName(), clientInfo);

        // Add out client info to our list
        myClients.push_back(clientInfo);

    }

    std::cout<<"TTRTS: All players connected"<< std::endl;

    std::cout<<"TTRTS: Hit enter to begin...";
    std::cin.ignore();

    // Loop for each turn
    while ( !game.GameOver() )
    {
        // Send data to clients
        std::cout<<"TTRTS: Broadcasting Gamestate"<< std::endl;
        SendGamestateToClients(myClients, game, gameMutex);

        // Wait for orders from clients
        std::cout<<"TTRTS: Waiting for orders from players"<< std::endl;
        WaitForOrdersFromClients(myClients, game, gameMutex);

        std::cout<<"TTRTS: All orders recieved, simulating turn"<< std::endl;

        // Step to the next turn
        gameMutex.lock();
        game.SimulateToNextTurn();
        gameMutex.unlock();
    }

    // Send final state to all the clients
    SendGamestateToClients(myClients, game, gameMutex);
}

int runServer(int argc, char* argv[])
{
    // argv[1] needs to be a valid game file 
    if( argc < 2 )
        fatal_error("must provide game file argument");

    // Set up game
    CTTRTSGame game = GetGameFromFile(argv[1]);
    if(game.GetNumUnits() == 0)
        fatal_error("game not valid");

    RunServerForGame(game);

    // Return winning player and output game end
    return OutputGameEnd(game);
}