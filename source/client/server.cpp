#include "server.h"

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <game.h>
#include <formatters.h>

#include "net.h"
#include "filesystem.h"

// Struct for net client info
struct ClientInfo
{
    sockaddr_in cli_addr;
    int clientsockfd;
};

int waitForOrdersFromClient(const ClientInfo info, std::mutex& mut, CTTRTSGame& game )
{
    char buffer[1028]; // buffer for orders
    memset(buffer,0,sizeof(buffer));

    std::cout<<"Waiting for "<<inet_ntoa(info.cli_addr.sin_addr)<<std::endl;

    // Read in the new socket
    // read will block until the client has called write
    // up to the full size of the buffer
    if (read(info.clientsockfd,buffer,sizeof(buffer)-1) < 0)
        error("ERROR reading from client");

    std::cout<<"Recieved orders from "<<inet_ntoa(info.cli_addr.sin_addr)<<std::endl;
    std::cout<<buffer<<std::endl;

    mut.lock();
    game.IssueOrders(player_t::Red , buffer);
    mut.unlock();

    return 0;
}

int runServer(int argc, char* argv[])
{
    std::cout<<"Setting up server on port "<<TTRTS_PORT<<std::endl;

    // Server side information
    int sockfd; 	        // socket File descriptor
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

    std::cout<<"Opening socket"<<std::endl;
    // Create a new socket
    // AF_INET is general internet socket domain
    // SOCK_STREAM as messages will be read in on this socket, SOCK_DGRAM would be for packets
    // 0 is for default protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // bind our socket to this server address
    std::cout<<"Binding socket"<<std::endl;
    int retry = 1;
    while (1)
    {
        if(retry > 10)
        {
            error("Binding failed after retries");
        }

        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0)
            break;

        std::cout<<"Binding failed on try "<<retry<<std::endl;
        sleep(retry);
        retry++;
    }

    // Listen on the socket for messages
    // Second param is length of backlog queue, the maximum number of connections
    // that can be waiting while the process is handling a single connection
    // max is usually set to 5
    listen(sockfd,5);

    // Set up game
    CTTRTSGame game = GetGameFromFile("Tiny2Player.txt");
    unsigned int numClients = game.GetPlayers().size();

    //  game mutex
    std::mutex gameMutex;

    // Set of clients
    std::vector<ClientInfo> myClients;

    std::cout<<"Waiting for clients"<<std::endl;

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
            error("ERROR on accept");

        std::cout<<"Client connected from "<<inet_ntoa(cli_addr.sin_addr)<<" socket "<<clientsockfd<<std::endl;
        myClients.push_back({cli_addr,clientsockfd});
    }

    std::cout<<"All clients connected"<<std::endl;

    // Loop for each turn
    while ( !game.GameOver() )
    {
        // Grab the current game state string
        gameMutex.lock();
        std::string gamestate_string = GetStringFromGame(game);
        gameMutex.unlock();

        // Send data to clients
        std::cout<<"Sending clients gamedata"<<std::endl;
        for (auto client : myClients)
        {
            // Write to the socket with the buffer
            if ( write( client.clientsockfd, gamestate_string.c_str(), gamestate_string.length() ) < 0 )
                error("ERROR sending to client");
        }

        // Wait for orders from clients
        std::cout<<"Waiting for client orders"<<std::endl;

        std::vector<std::thread> clientThreads;
        for(auto client : myClients)
        {
            std::thread clientThread(waitForOrdersFromClient, client, std::ref(gameMutex), std::ref(game));
            clientThreads.push_back(std::move(clientThread));
        }

        // Join up all the threads
        for ( std::thread& thread : clientThreads )
        {
            thread.join();
        }

        std::cout<<"Orders recieved, simulating turn"<<std::endl;
        // Step to the next turn
        gameMutex.lock();
        game.SimulateToNextTurn();
        gameMutex.unlock();
    }


    // end game and disconnect clients

    // Return
    return 0;
}