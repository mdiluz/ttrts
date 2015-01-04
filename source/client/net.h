#ifndef _TTRTS_NET_H_
#define _TTRTS_NET_H_

#include <vector>
#include <mutex>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <game.h>
#include <formatters.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define TTRTS_HANDSHAKE_FORMAT "player %u name %s"

// Struct for net client info
struct ClientInfo
{
    sockaddr_in cli_addr;
    int clientsockfd;
    player_t player;
};

// Get the address of a local server
sockaddr_in GetLocalServerAddress();

// Set up a new listening socket for the server
int SetUpServerListeningSocket(const sockaddr_in &serv_addr);

// Wait for client connection on listening socket sockfd
// Will fill clientInfo with client information
ClientInfo &WaitForClientConnection(int sockfd, const std::string &game, ClientInfo &clientInfo);

// Wait for orders from a client, will not return until client has send valid orders
// Will automatically add orders to the game
void WaitForOrdersFromClient(const ClientInfo info, CTTRTSGame &game, std::mutex &mut);

// Iterates through a list of clients calling WaitForOrdersFromClient
void WaitForOrdersFromClients(std::vector<ClientInfo> &myClients, CTTRTSGame &game, std::mutex &gameMutex);

// Sends current gamestate to each client
void SendGamestateToClients(std::vector<ClientInfo> &myClients, const CTTRTSGame &game, std::mutex &gameMutex);

// Tries to bind to a socket, will attempt 10 times with longer waits between
void TryBindSocket(int sockfd, const sockaddr_in &serv_addr);

// Perform the server side handshake with a client
void PerformServerHandshake(const ClientInfo &client, const std::string &game);

// Perform the client side handshake with the server
void PerformClientHandshake(int sockfd, unsigned int &player, std::string &gameNameString);

// For local fatal errors
inline void fatal_error(const char *msg)
{
    std::cerr<<msg<<std::endl;
    exit(1);
}

// For system fatal errors (ie. functions that set errno)
inline void fatal_perror(const char *msg)
{
    perror(msg);
    exit(1);
}

#endif