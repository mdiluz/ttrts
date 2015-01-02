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

int runClient(int argc, char* argv[])
{
    int sockfd; 	// socket File descriptor
    int portno;		// Port number
    int n = 0;		// return value for read and write calls

    struct sockaddr_in serv_addr; // Server address

    struct hostent *server; // pointer to host information

    char buffer[1028]; // buffer for socked read
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
    // AF_INET is general internetsocked domain
    // SOCK_STREAM as messages will be read in on this socket, SOCK_DGRAM would be for packets
    // 0 is for default protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Get the hostent information for the host by name
    server = gethostbyname(argv[1]);
    if (server == NULL)
        error("ERROR, no such host");

    std::cout<<"Connecting to "<<argv[1]<<std::endl;

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
        error("ERROR connecting");

    while ( n >= 0 )
    {
        // Get the message to send
        printf("Please enter the message: ");
        memset(buffer,0,sizeof(buffer));
        fgets(buffer,sizeof(buffer)-1,stdin);

        // Place a test message into the buffer
        strncpy(buffer,buffer,sizeof(buffer));

        // Write to the socket with the buffer
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");
    }

    return 0;
}