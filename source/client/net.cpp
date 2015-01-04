#include "net.h"

#include <iostream>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void WaitForOrdersFromClient(const ClientInfo info, std::mutex &mut, CTTRTSGame &game)
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
            fatal_perror("ERROR reading from client");

        // Append the received orders
        orders+=buffer;
    }

    std::cout<<"Recieved orders from "<<inet_ntoa(info.cli_addr.sin_addr)<<std::endl;

    mut.lock();
    game.IssueOrders(info.player , orders);
    mut.unlock();
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
            fatal_perror("ERROR sending to client");
    }
}


void PerformClientHandshake(int sockfd, unsigned int &player, std::string &gameNameString)
{
    char handshakeBuffer[128];
    memset(handshakeBuffer,0,sizeof(handshakeBuffer));

    if (read(sockfd, handshakeBuffer,sizeof(handshakeBuffer)-1) < 0)
        fatal_perror("ERROR recieving handshake from server");

    std::string handshake(handshakeBuffer);
    std::cout<<"Handshake:"<<handshake<< std::endl;

    if ( write( sockfd, handshake.c_str(), handshake.length()+1 ) < 0 )
        fatal_perror("ERROR sending handshake to server");

    char gameName[64];
    if ( sscanf(handshake.c_str(),TTRTS_HANDSHAKE_FORMAT,&player,gameName) < 2 )
        fatal_error("Handshake failed");

    gameNameString = gameName;
}

void PerformServerHandshakeWithClient(const ClientInfo &client, const CTTRTSGame &game)
{
    char handshake[64];
    snprintf(handshake, sizeof(handshake), TTRTS_HANDSHAKE_FORMAT,(unsigned int)client.player,game.GetName().c_str());

    // Output the handshake
    std::cout<<"Handshaking:"<<handshake<< std::endl;

    // Send handshake
    if ( write( client.clientsockfd,handshake,sizeof(handshake) ) < 0 )
        fatal_perror("ERROR sending to client");

    // Receive handshake
    char buffer[64];
    if ( read(client.clientsockfd,buffer,sizeof(buffer)-1) < 0 )
        fatal_perror("ERROR reading from client");

    std::cout<<"Received:"<<buffer<< std::endl;

    // Verify handshake
    if ( std::string(buffer) != std::string(handshake) )
        fatal_error("Error in client handshake");

    std::cout<<"Success on handshake with "<<handshake<< std::endl;
}

void TryBindSocket(int sockfd, sockaddr_in &serv_addr)
{
    std::cout<<"Binding socket"<< std::endl;
    int retry = 1;
    while (1)
    {
        if(retry > 10)
            fatal_error("Binding failed");

        // Attempt to bind our listening socket
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0)
            break;

        std::cout<<"Binding failed on try "<<retry<< std::endl;
        sleep(retry);
        retry++;
    }
}