#ifndef FILE_IO_H
#define FILE_IO_H

//Takes the requested path, finds it on the disk, and sends it to the browser
void serve_file(int client_socket, const char *path);

#endif
