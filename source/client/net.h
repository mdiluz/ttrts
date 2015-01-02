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

// Struct for net client info
struct ClientInfo
{
    sockaddr_in cli_addr;
    int clientsockfd;
    player_t player;
};

int WaitForOrdersFromClient(const ClientInfo info, std::mutex &mut, CTTRTSGame &game);

void GetOrdersFromClients(std::vector<ClientInfo> &myClients, CTTRTSGame &game, std::mutex &gameMutex);

void SendGameInfoToClients(std::vector<ClientInfo> &myClients, const CTTRTSGame &game, std::mutex &gameMutex);

inline void error(const char *msg)
{
    perror(msg);
    exit(1);
}

#endif