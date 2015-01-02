#include "server.h"

#include <thread>
#include <vector>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "net.h"

std::vector<std::thread> gClientThreads;

int clientHandler( sockaddr_in cli_addr, socklen_t clilen, int clientsockfd)
{
    std::cout<<"Client connected from "<<inet_ntoa(cli_addr.sin_addr)<<std::endl;

    char buffer[1028]; // buffer for socked read
    memset(buffer,0,sizeof(buffer));

    // loop
    int n = 0;			// return value for read and write calls
    while( n >= 0 )
    {
        // empty the buffer
        memset(buffer,0,sizeof(buffer));

        // Read in the new socket
        // read will block until the client has called write
        // up to the full size of the buffer
        n = read(clientsockfd,buffer,sizeof(buffer)-1);
        if (n < 0)
            error("ERROR reading from socket");

        // print the message recieved
        printf("%s",buffer);
    }

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

    std::cout<<"Waiting for clients"<<std::endl;

    // Loop while we're connecting the clients
    bool connectingClients = true;
    while ( connectingClients )
    {

        // information for each client
        sockaddr_in cli_addr;   // Client address
        socklen_t clilen;		// length of client address
        int clientsockfd; 	    // new socket File descriptor

        clilen = sizeof(sockaddr_in);


        // accept waits for a connection from a client
        // it returns a new socket file descriptor for this connection
        // client information will be stored in cli_addr
        clientsockfd = accept(sockfd, (sockaddr *) &cli_addr, &clilen);
        if (clientsockfd < 0)
            error("ERROR on accept");

        std::cout<<"Client recieved"<<std::endl;
        std::thread clientThread = std::thread(clientHandler,cli_addr,clilen,clientsockfd);

        gClientThreads.push_back(std::move(clientThread));
    }

    // Return
    return 0;
}