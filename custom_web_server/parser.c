#include <stdio.h>
#include <string.h>
#include "parser.h"

int parse_http_request(const char *raw_request, HttpRequest *parsed_request) {
    //Clears the struct memory to avoid leftover garbage data
    memset(parsed_request, 0, sizeof(HttpRequest));

    //We only care about the very first line of the HTTP request.
    //sscanf reads formatted input from a string.
    //%15s means read a string up to 15 chars (this prevents Buffer Overflows)
    int items_parsed = sscanf(raw_request, "%15s %255s %15s",
                              parsed_request->method,
                              parsed_request->path,
                              parsed_request->version);

    //If we didn't find all 3 parts, the browser sent garbage
    if (items_parsed != 3) {
        printf("[PARSER] Malformed request received.\n");
        return -1;
    }

    //Routing Convenience: If the user just types "localhost:8080/", 
    //assumes they want the home page
    if (strcmp(parsed_request->path, "/") == 0) {
        strcpy(parsed_request->path, "/index.html");
    }

    printf("[PARSER] Successfully parsed: %s %s\n", parsed_request->method, parsed_request->path);
    return 0;
}
