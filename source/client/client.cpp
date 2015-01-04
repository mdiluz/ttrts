#include "client.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>

#include "net.h"
#include "game.h"
#include "filesystem.h"

int runClient(int argc, char* argv[])
{
    player_t myPlayer = player_t::NUM_INVALID; // My player

    int sockfd; 	// socket File descriptor
    int portno;		// Port number
    int n = 0;		// return value for read and write calls

    struct sockaddr_in serv_addr; // Server address

    struct hostent *server; // pointer to host information

    char buffer[1028]; // buffer for socket read
    memset(buffer,0,sizeof(buffer));

    // must provide information
    if (argc < 2)
    {
        fprintf(stderr,"usage %s hostname\n", argv[0]);
        exit(0);
    }

    // Get port number
    portno = TTRTS_PORT;

    // Create a new socket
    // AF_INET is general internetsocket domain
    // SOCK_STREAM as messages will be read in on this socket, SOCK_DGRAM would be for packets
    // 0 is for default protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal_perror("ERROR opening socket");

    // Get the hostent information for the host by name
    server = gethostbyname(argv[1]);
    if (server == NULL)
        fatal_error("ERROR, no such host");

    std::cout<<"TTRTS: Connecting to "<<argv[1]<<std::endl;

    // Empty the server address struct
    memset(&serv_addr,0, sizeof(serv_addr));

    // Set the server to AF_INET
    serv_addr.sin_family = AF_INET;

    // copy the server address into our server_addr struct
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Set our server address port to the port number provided
    serv_addr.sin_port = htons(portno);

    // Attempt to connect to the server using the socket and server address info
    if (connect(sockfd, (const sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        fatal_perror("ERROR connecting");

    unsigned int player;
    std::string gameNameString;

    PerformClientHandshake(sockfd, player, gameNameString);

    myPlayer = (player_t)player;
    std::cout<<"TTRTS: I am player "<<std::to_string((int)myPlayer)<<std::endl;
    std::cout<<"TTRTS: Game is "<<gameNameString<<std::endl;

    // Clean out the games dir
    CreateAndCleanGameDir(gameNameString);

    while ( n >= 0 )
    {
        std::cout<<"TTRTS: Waiting for gamestate"<<std::endl;

        std::string gamestate;
        while( gamestate.find("END") == std::string::npos )
        {
            // Receive gamestate
            memset(buffer,0,sizeof(buffer));
            if (read(sockfd,buffer,sizeof(buffer)-1) < 0)
                fatal_perror("ERROR reading from client");

            gamestate+=buffer;
        }

        // Output the gamestate file for this game
        CTTRTSGame thisGame = GetGameFromString(gamestate);
        OutputGameStateFile(thisGame);

        // Get the order file for this turn
        std::string orders = GetOrdersFromPlayerFile(thisGame,myPlayer);

        std::cout<<"TTRTS: Sending orders"<<std::endl;
        std::cout<<orders<<std::endl;
        // Write to the socket with the buffer
        n = write(sockfd,orders.c_str(),orders.length());
        if (0 < n)
            fatal_perror("ERROR writing to socket");
    }

    return 0;
}