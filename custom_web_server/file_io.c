#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "file_io.h"

//ensures the browser knows the file being sent
const char *get_mime_type(const char *path) {
    if (strstr(path, ".css")) return "text/css";
    if (strstr(path, ".js"))  return "application/javascript";
    if (strstr(path, ".png")) return "image/png";
    if (strstr(path, ".jpg")) return "image/jpeg";
    return "text/html"; //Default to HTML
}

void serve_file(int client_socket, const char *path) {
    //Prepend a ./www folder so the 
    // browser can't accidentally read rivate OS files
    char file_path[512] = "./www"; 
    strcat(file_path, path);       

    struct stat file_stat;
    
    //checks if the file exists
    if (stat(file_path, &file_stat) < 0) {
        char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404: File Not Found";
        write(client_socket, not_found, strlen(not_found));
        printf("[FILE IO] 404 Not Found: %s\n", file_path);
        return;
    }

    //Opens the file in binary read mode rb
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        char *error = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        write(client_socket, error, strlen(error));
        return;
    }

    //Sends the HTTP Headers first
    char header[512];
    sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", 
            get_mime_type(file_path), file_stat.st_size);
    write(client_socket, header, strlen(header));

    //Reads the file from the hard drive in 4KB chunks and write to the socket
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        write(client_socket, buffer, bytes_read);
    }

    //clean up
    fclose(file);
    printf("[FILE IO] Successfully served: %s\n", file_path);
}
