#include "net.h"

#include <netdb.h>

#include <iostream>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void WaitForOrdersFromClient(const ClientInfo info,  CTTRTSGame &game, std::mutex &mut)
{
    char buffer[1028]; // buffer for orders

    std::clog<<"TTRTS: Waiting for player "<<(int)info.player<<" at "<<inet_ntoa(info.cli_addr.sin_addr)<<std::endl;

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

    std::clog<<"TTRTS: Recieved orders from "<<inet_ntoa(info.cli_addr.sin_addr)<<std::endl;

    mut.lock();
    game.IssueOrders(info.player , orders);
    mut.unlock();
}

void WaitForOrdersFromClients(std::vector<ClientInfo> &myClients, CTTRTSGame &game, std::mutex &gameMutex)
{
    // Spawn threads
    std::vector<std::thread> clientThreads;
    for(auto client : myClients)
    {
        std::thread clientThread(WaitForOrdersFromClient, client, std::ref(game), ref(gameMutex));
        clientThreads.push_back(move(clientThread));
    }

    // Join up all the threads
    for ( std::thread& thread : clientThreads )
    {
        thread.join();
    }
}

void SendGamestateToClients(std::vector<ClientInfo> &myClients, const CTTRTSGame &game, std::mutex &gameMutex)
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

    if ( write( sockfd, handshake.c_str(), handshake.length()+1 ) < 0 )
        fatal_perror("ERROR sending handshake to server");

    char gameName[64];
    if ( sscanf(handshake.c_str(),TTRTS_HANDSHAKE_FORMAT,&player,gameName) < 2 )
        fatal_error("Handshake failed");

    gameNameString = gameName;
}

void PerformServerHandshake(const ClientInfo &client, const std::string &game)
{
    char handshake[64];
    snprintf(handshake, sizeof(handshake), TTRTS_HANDSHAKE_FORMAT,(unsigned int)client.player,game.c_str());

    // Send handshake
    if ( write( client.clientsockfd,handshake,sizeof(handshake) ) < 0 )
        fatal_perror("ERROR sending to client");

    // Receive handshake
    char buffer[64];
    if ( read(client.clientsockfd,buffer,sizeof(buffer)-1) < 0 )
        fatal_perror("ERROR reading from client");

    // Verify handshake
    if ( std::string(buffer) != std::string(handshake) )
        fatal_error("Error in client handshake");

    std::clog<<"TTRTS: Success on handshake with player "<<(int)client.player<< std::endl;
}

void TryBindSocket(int sockfd, const sockaddr_in &serv_addr)
{
    std::clog<<"TTRTS: Binding to socket"<< std::endl;
    int retry = 1;
    while (1)
    {
        if(retry > 10)
            fatal_error("Binding failed");

        // Attempt to bind our listening socket
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0)
            break;

        std::cerr<<"Warning: Binding failed on try "<<retry<< std::endl;
        sleep(retry);
        retry++;
    }
}

ClientInfo &WaitForClientConnection(int sockfd, const std::string &game, ClientInfo &clientInfo)
{
    socklen_t clilen = sizeof(sockaddr_in);

    // accept waits for a connection from a client
    // it returns a new socket file descriptor for this connection
    // client information will be stored in cli_addr
    clientInfo.clientsockfd = accept(sockfd, (sockaddr *) &clientInfo.cli_addr, &clilen);
    if (clientInfo.clientsockfd < 0)
        fatal_perror("ERROR on accept");

    std::clog<<"TTRTS: Client connected from "<<inet_ntoa(clientInfo.cli_addr.sin_addr)<<" socket "<<clientInfo.clientsockfd<< std::endl;

    // Handshake with client
    PerformServerHandshake(clientInfo, game);
    return clientInfo;
}


int SetUpServerListeningSocket(const sockaddr_in &serv_addr)
{
    int sockfd;

    std::clog<<"TTRTS: Opening socket"<< std::endl;
    // Create a new socket
    // AF_INET is general internet socket domain
    // SOCK_STREAM as messages will be read in on this socket, SOCK_DGRAM would be for packets
    // 0 is for default protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal_perror("ERROR opening socket");

    // bind our socket to this server address
    TryBindSocket(sockfd, serv_addr);

    // Listen on the socket for messages
    // Second param is length of backlog queue, the maximum number of connections
    // that can be waiting while the process is handling a single connection
    // max is usually set to 5
    listen(sockfd,5);
    return sockfd;
}


sockaddr_in GetLocalServerAddress()
{
    sockaddr_in serv_addr;  // Server address

    // empty the server address
    memset(&serv_addr,0, sizeof(serv_addr));
    // Set the server address family to AF_INET
    serv_addr.sin_family = AF_INET;
    // htons swaps from host byte order to network byte order
    serv_addr.sin_port = htons(TTRTS_PORT);
    // The host for this address is this current machine's IP, INADDR_ANY fetches this
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    return serv_addr;
}


int ConnectToHostServer(const std::string &hostname, sockaddr_in &serv_addr)
{
    // Create a new socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal_perror("ERROR opening socket");

    // Get the hostent information for the host by name
    hostent *server = gethostbyname(hostname.c_str());
    if (server == NULL)
        fatal_error("ERROR, no such host");

    // copy the server address into our server_addr struct
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Attempt to connect to the server using the socket and server address info
    if (connect(sockfd, (const sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        fatal_perror("ERROR connecting");

    return sockfd;
}

std::string WaitForGamestateMessage(int sockfd)
{
    std::string gamestate;
    char gamestateBuffer[1028];
    while( gamestate.find("END") == std::string::npos )
    {
        memset(gamestateBuffer,0,sizeof(gamestateBuffer));

        // Receive gamestate
        if (read(sockfd,gamestateBuffer,sizeof(gamestateBuffer)-1) < 0)
            fatal_perror("ERROR reading from client");

        gamestate+=gamestateBuffer;
    }
    return gamestate;
}

int SendOrdersToServer(int sockfd, const std::string &orders)
{
    int n = write(sockfd,orders.c_str(),orders.length());
    if (0 < n)
        fatal_perror("ERROR writing to socket");
    return n;
}


int OutputGameEnd( CTTRTSGame& game )
{
    std::cout<<"TTRTS: Game Over!"<<std::endl;

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
