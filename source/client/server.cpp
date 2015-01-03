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

CTTRTSGame &RunServerForGame(CTTRTSGame &game)
{
    std::cout<<"Setting up server on port "<<TTRTS_PORT<< std::endl;

    // Server side information
    int sockfd;            // socket File descriptor
    sockaddr_in serv_addr;  // Server address
    int portno = TTRTS_PORT;

    // empty the server address
    memset(&serv_addr,0, sizeof(serv_addr));
    // Set the server address family to AF_INET
    serv_addr.sin_family = AF_INET;
    // htons swaps from host byte order to network byte order
    serv_addr.sin_port = htons(portno);
    // The host for this address is this current machine's IP, INADDR_ANY fetches this
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    std::cout<<"Opening socket"<< std::endl;
    // Create a new socket
    // AF_INET is general internet socket domain
    // SOCK_STREAM as messages will be read in on this socket, SOCK_DGRAM would be for packets
    // 0 is for default protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal_perror("ERROR opening socket");

    // bind our socket to this server address
    std::cout<<"Binding socket"<< std::endl;
    int retry = 1;
    while (1)
    {
        if(retry > 10)
        {
            fatal_error("Binding failed after retries");
        }

        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0)
            break;

        std::cout<<"Binding failed on try "<<retry<< std::endl;
        sleep(retry);
        retry++;
    }

    // Listen on the socket for messages
    // Second param is length of backlog queue, the maximum number of connections
    // that can be waiting while the process is handling a single connection
    // max is usually set to 5
    listen(sockfd,5);

    std::vector<player_t> players = game.GetPlayers();
    unsigned int numClients = players.size();
    auto player_iterator = players.begin();

    //  game mutex
    std::mutex gameMutex;

    // Set of clients
    std::vector<ClientInfo> myClients;

    std::cout<<"Waiting for clients"<< std::endl;

    // Loop while we're connecting the clients
    while ( myClients.size() < numClients )
    {
        // information for each client
        sockaddr_in cli_addr;   // Client address
        int clientsockfd; 	    // new socket File descriptor

        socklen_t clilen = sizeof(sockaddr_in);

        // accept waits for a connection from a client
        // it returns a new socket file descriptor for this connection
        // client information will be stored in cli_addr
        clientsockfd = accept(sockfd, (sockaddr *) &cli_addr, &clilen);
        if (clientsockfd < 0)
            fatal_perror("ERROR on accept");

        std::cout<<"Client connected from "<<inet_ntoa(cli_addr.sin_addr)<<" socket "<<clientsockfd<< std::endl;

        ClientInfo clientInfo;
        clientInfo.cli_addr = cli_addr;
        clientInfo.clientsockfd = clientsockfd;
        clientInfo.player = *player_iterator;

        player_iterator++;
        myClients.push_back(clientInfo);
    }

    // Perform the initial handshake with clients
    for( auto client : myClients )
    {
        // Handshake currently just player
        char handshake[64];
        snprintf(handshake, sizeof(handshake), TTRTS_HANDSHAKE_FORMAT,(unsigned int)client.player,game.GetName().c_str());

        // Output the handshake
        std::cout<<"Handshaking:"<<handshake<< std::endl;

        // Send handshake
        if ( write( client.clientsockfd,handshake,sizeof(handshake) ) < 0 )
            fatal_perror("ERROR sending to client");

        // Recieve handshake
        char buffer[64];
        if (read(client.clientsockfd,buffer,sizeof(buffer)-1) < 0)
            fatal_perror("ERROR reading from client");

        std::cout<<"Received:"<<buffer<< std::endl;

        // Verify handshake
        if ( std::string(buffer) != std::string(handshake) )
            fatal_error("Error in client handshake");

        std::cout<<"Success on handshake with "<<handshake<< std::endl;
    }

    std::cout<<"All clients connected"<< std::endl;

    std::cout<<"Hit enter to begin...";
    std::cin.ignore();

    // Loop for each turn
    while ( !game.GameOver() )
    {
        // Send data to clients
        std::cout<<"Sending clients gamedata"<< std::endl;
        SendGameInfoToClients(myClients, game, gameMutex);

        // Wait for orders from clients
        std::cout<<"Waiting for client orders"<< std::endl;
        GetOrdersFromClients(myClients, game, gameMutex);

        std::cout<<"Orders recieved, simulating turn"<< std::endl;

        // Step to the next turn
        gameMutex.lock();
        game.SimulateToNextTurn();
        gameMutex.unlock();
    }

    // Send final state to all the clients
    SendGameInfoToClients(myClients, game, gameMutex);
    return game;
}

int runServer(int argc, char* argv[])
{
    // Set up game
    CTTRTSGame game = GetGameFromFile(argv[1]);
    if(game.GetNumUnits() == 0)
        fatal_error("game not valid");

    RunServerForGame(game);

    // Get the winning player
    player_t winningPlayer = game.GetWinningPlayer();

    // Print the winner!
    if ( winningPlayer != player_t::NUM_INVALID )
        std::cout<<"Game over! Winner:"<<(int) winningPlayer <<std::endl;
    else
        std::cout<<"Game over! It was a draw!"<<std::endl;

    // Return
    return (int)winningPlayer;
}