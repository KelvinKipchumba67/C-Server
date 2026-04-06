#ifndef NETWORK_H
#define NETWORK_H

#include <netinet/in.h>

//holds server's configuration
typedef struct {
    int port;
    int server_fd;
} ServerConfig;

//initializes the socket, binds it, and starts listening.
//returns the server file descriptor, or -1 on error.
int initialize_server(ServerConfig *config);

//accepts an incoming connection from the queue.
//returns the new client socket file descriptor.
int accept_connection(int server_fd);

#endif
