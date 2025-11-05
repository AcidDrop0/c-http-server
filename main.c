#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "config.h"
#include "handlers.h"
#include "http_utils.h"

ServerConfig config; // Global server configuration. This determines if the directory mode is enabled.

int main(int argc, char *argv[]) {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);	
	printf("Server logs will appear here!\n");

	if (argc > 2 && strcmp(argv[1], "--directory") == 0) {
        config.directory = argv[2]; 
        printf("Directory mode enabled: %s\n", config.directory);
    } else {
        config.directory = NULL;
    }
	
	int server_fd, client_addr_len;
	server_fd = socket(AF_INET, SOCK_STREAM, 0); // Creating a socket specifying the IPv4 and TCP protocols
	check(server_fd, "Socket creation failed");
	
	// This ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	check(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)), "SO_REUSEADDR failed");
	
	SA_IN serv_addr =  { .sin_family = AF_INET , // IPv4
						 .sin_port = htons(PORT), // port number
						 .sin_addr = { htonl(INADDR_ANY) }, // listen on any address
						};

	check(bind(server_fd, (SA *)&serv_addr, sizeof(serv_addr)), "Bind failed");
	
	int connection_backlog = 5; // Number of allowed pending connections
	check(listen(server_fd, connection_backlog), "Listen failed");
	
	printf("Server is listening on port %d...\n", PORT);
	
	while(1){
		SA_IN client_addr; // Structure to hold client address information
		client_addr_len = sizeof(client_addr); // Length of the client address structure

		int client_fd = accept(server_fd, (SA *) &client_addr, &client_addr_len);
		printf("\nClient connected\n");

		int *pclient_fd = malloc(sizeof(int));
		*pclient_fd = client_fd;

		pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_Client, pclient_fd);
        pthread_detach(thread_id);
	}
	
	close(server_fd);

	return 0;
}