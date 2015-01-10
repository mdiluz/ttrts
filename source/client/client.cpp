#include <iostream>

#include "net.h"
#include "game.h"
#include "error.h"
#include "filesystem.h"

int main(int argc, char* argv[])
{
    // must provide information
    if (argc < 2)
        fatal_error("Usage: ttrts-client HOST");

    std::string hostname = argv[1];

    sockaddr_in serv_addr; // Server address
    memset(&serv_addr,0, sizeof(serv_addr));

    // Set the server to AF_INET
    serv_addr.sin_family = AF_INET;
    // Set our server address port to the port number provided
    serv_addr.sin_port = htons(TTRTS_PORT);

    std::cout<<"TTRTS: Connecting to "<<hostname<<std::endl;
    int sockfd = ConnectToHostServer(hostname, serv_addr);

    unsigned int player;
    std::string gameNameString;

    // Handshake with server to fetch player and gamestring
    PerformClientHandshake(sockfd, player, gameNameString);

    // output our information
    player_t myPlayer = (player_t)player;
    std::cout<<"TTRTS: I am player "<<std::to_string((int)myPlayer)<<std::endl;
    std::cout<<"TTRTS: Game is "<<gameNameString<<std::endl;

    // Clean out the games dir
    CreateAndCleanGameDir(gameNameString);

    // Buffer for messages
    char buffer[1028];
    memset(buffer,0,sizeof(buffer));

    int n = 0;		// return value for read and write calls
    while ( n >= 0 )
    {
        std::cout<<"TTRTS: Waiting for gamestate"<<std::endl;
        std::string gamestate = WaitForGamestateMessage(sockfd);

        // Output the gamestate file for this game
        CTTRTSGame thisGame = GetGameFromString(gamestate);
        OutputGameStateFile(thisGame);

        // If game over, exit with out winning player and message
        if(thisGame.GameOver())
            exit( OutputGameEnd(thisGame) );

        // Get the order file for this turn`
        std::string orders = GetOrdersFromPlayerFile(thisGame,myPlayer);

        std::cout<<"TTRTS: Sending orders"<<std::endl;
        std::cout<<orders<<std::endl;
        // Write to the socket with the buffer
        n = SendOrdersToServer(sockfd, orders);
    }

    return 0;
}