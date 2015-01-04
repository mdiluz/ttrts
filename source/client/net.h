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

void WaitForOrdersFromClient(const ClientInfo info, std::mutex &mut, CTTRTSGame &game);

void GetOrdersFromClients(std::vector<ClientInfo> &myClients, CTTRTSGame &game, std::mutex &gameMutex);

void SendGameInfoToClients(std::vector<ClientInfo> &myClients, const CTTRTSGame &game, std::mutex &gameMutex);

void TryBindSocket(int sockfd, sockaddr_in &serv_addr);

void PerformServerHandshakeWithClient(const ClientInfo &client, const CTTRTSGame &game);

void PerformClientHandshake(int sockfd, unsigned int &player, std::string &gameNameString);

inline void fatal_error(const char *msg)
{
    std::cerr<<msg<<std::endl;
    exit(1);
}

inline void fatal_perror(const char *msg)
{
    perror(msg);
    exit(1);
}

#endif