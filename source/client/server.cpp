#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include "net.h"

int runServer(int argc, char* argv[])
{
    int sockfd; 	// socket File descriptor
    int newsockfd; 	// new socket File descriptor

    int portno;		// Port number
    socklen_t clilen;		// length of client address
    clilen = sizeof(sockaddr_in);

    int n = 0;			// return value for read and write calls

    char buffer[1028]; // buffer for socked read
    memset(buffer,0,sizeof(buffer));

    struct sockaddr_in serv_addr; // Server address
    struct sockaddr_in cli_addr;  // Client address

    // Create a new socket
    // AF_INET is general internetsocked domain
    // SOCK_STREAM as messages will be read in on this socket, SOCK_DGRAM would be for packets
    // 0 is for default protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // empty the server address
    memset(&serv_addr,0, sizeof(serv_addr));

    // Grab the port number
    portno = 11715;

    // Set the server address family to AF_INET
    serv_addr.sin_family = AF_INET;

    // Set the port number
    // htons swaps from host byte order to network byte order
    serv_addr.sin_port = htons(portno);

    // The host for this address is this current machines's IP
    // INADDR_ANY fetches this
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // bind out socket to this server address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // Listen on the socket for messages
    // Second param is length of backlog queue, the maximum number of connections
    // that can be waiting while the process is handling a single connection
    // max is usually set to 5
    listen(sockfd,5);

    // accept waits for a connection from a client
    // it returns a new socket file descriptor for this connection
    // client information will be stored in cli_addr
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    // loop
    while( n >= 0 )
    {
        // empty the buffer
        memset(buffer,0,sizeof(buffer));

        // Read in the new socket
        // read will block until the client has called write
        // up to the full size of the buffer
        n = read(newsockfd,buffer,sizeof(buffer)-1);
        if (n < 0)
            error("ERROR reading from socket");

        // print the message recieved
        printf("%s",buffer);
    }

    // Return
    return 0;
}