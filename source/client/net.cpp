#include "net.h"

#include <iostream>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int WaitForOrdersFromClient(const ClientInfo info, std::mutex &mut, CTTRTSGame &game)
{
    char buffer[1028]; // buffer for orders

    std::cout<<"Waiting for "<<inet_ntoa(info.cli_addr.sin_addr)<<std::endl;

    std::string orders;

    while ( orders.find("END") == std::string::npos )
    {
        memset(buffer,0,sizeof(buffer));

        // Read in the new socket
        // read will block until the client has called write
        // up to the full size of the buffer
        if (read(info.clientsockfd,buffer,sizeof(buffer)-1) < 0)
            error("ERROR reading from client");

        // Append the received orders
        orders+=buffer;
    }

    std::cout<<"Recieved orders from "<<inet_ntoa(info.cli_addr.sin_addr)<<std::endl;

    mut.lock();
    game.IssueOrders(info.player , orders);
    mut.unlock();

    return 0;
}

void GetOrdersFromClients(std::vector<ClientInfo> &myClients, CTTRTSGame &game, std::mutex &gameMutex)
{
    // Spawn threads
    std::vector<std::thread> clientThreads;
    for(auto client : myClients)
    {
        std::thread clientThread(WaitForOrdersFromClient, client, ref(gameMutex), std::ref(game));
        clientThreads.push_back(move(clientThread));
    }

    // Join up all the threads
    for ( std::thread& thread : clientThreads )
    {
        thread.join();
    }
}

void SendGameInfoToClients(std::vector<ClientInfo> &myClients, const CTTRTSGame &game, std::mutex &gameMutex)
{
    gameMutex.lock();
    std::string gamestate_string = GetStringFromGame(game);
    gameMutex.unlock();

    for (auto client : myClients)
    {
        // Write to the socket with the buffer
        if ( write( client.clientsockfd, gamestate_string.c_str(), gamestate_string.length() ) < 0 )
            error("ERROR sending to client");
    }
}
