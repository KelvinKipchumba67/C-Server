#ifndef PARSER_H
#define PARSER_H

//clean box to hold exactly what the browser wants
typedef struct {
    char method[16];   // e.g., GET, POST
    char path[256];    // e.g., index.html /images/logo.png
    char version[16];  // e.g., HTTP/1.1
} HttpRequest;

//Takes raw text from the socket and fills HttpRequest struct
//Returns 0 on success, -1 if the request is malformed
int parse_http_request(const char *raw_request, HttpRequest *parsed_request);

#endif
