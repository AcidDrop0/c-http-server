#ifndef HANDLERS_H
#define HANDLERS_H

void* handle_Client(void* client_fd); // The function passed to pthread_create to handle each client connection
void handle_get_request(char* response, char* buffer);
void handle_post_request(char* response, char* buffer);
void handle_file_request(char* response, char* url); // Handles GET file requests,. Used in handle_get_request

#endif
