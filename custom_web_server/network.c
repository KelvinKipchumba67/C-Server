#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "network.h" //blueprint

//setup function
int initialize_server(ServerConfig *config) {
    int opt = 1;
    struct sockaddr_in address;

    //Creates the socket
    if ((config->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return -1;
    }

    //Allows port reuse
    if (setsockopt(config->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        return -1;
    }

    //Configures the network structures
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(config->port);

    //Binds to the port
    if (bind(config->server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }

    //Starts listening
    if (listen(config->server_fd, 10) < 0) {
        perror("Listen failed");
        return -1;
    }

    printf("[NETWORK] Server successfully bound to port %d\n", config->port);
    return config->server_fd;
}

//accept function
int accept_connection(int server_fd) {
    struct sockaddr_in client_address;
    int addrlen = sizeof(client_address);
    int new_socket;

    //Waits for a browser to connect
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        return -1;
    }

    return new_socket;
}
