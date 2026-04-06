#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "thread_pool.h"

int main() {
    ServerConfig config = { .port = 8080 };

    if (initialize_server(&config) < 0) {
        printf("Failed to start server.\n");
        exit(EXIT_FAILURE);
    }

    //boots up 10 worker threads
    thread_pool_t *pool = thread_pool_init(10);

    printf("\n[MAIN] Server is running and waiting for traffic...\n");

    while (1) {
        //Waits for a browser connection
        int client_socket = accept_connection(config.server_fd);
        if (client_socket < 0) continue;

        //Tosses the socket into the Queue. main.c
        thread_pool_add_job(pool, client_socket);
    }

    return 0;
}
