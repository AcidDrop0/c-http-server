#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "http_utils.h"
#include "handlers.h"

#define BUF_1MB (1024 * 1024) // 1 Megabyte buffer size for reading requests and forming responses

void* handle_Client(void* arg){
	int client_fd = (*(int*)arg);
	free(arg);

	char* buffer = (char*)malloc(BUF_1MB*sizeof(char));
	int bytes_read = read(client_fd, buffer, BUF_1MB-1);
	buffer[bytes_read] = '\0';
	printf("Received request:\n%s\n", buffer);

	char* response = (char*)malloc(BUF_1MB*sizeof(char));

	if(strncmp(buffer, "GET", 3) == 0){
		handle_get_request(response, buffer);
	}
	else if(strncmp(buffer, "POST", 4) == 0){
		handle_post_request(response, buffer);
	}
	else{
		printf("Error while processing the HTTP request\n");
		return NULL;
	}

	send(client_fd, response, strlen(response), 0);

	close(client_fd);
	free(buffer);
	free(response);
	
	return NULL;
}

void handle_post_request(char* response, char* buffer){
	char* url = extract_URL(buffer, 1);

	if(config.directory != NULL && strncmp(url, "/files/", 7) == 0){
		char* cl_str = get_Word_Between(buffer, "Content-Length: ", "\r\n");
		int content_length = atoi(cl_str);
		free(cl_str);

		char* data = strstr(buffer, "\r\n\r\n") + 4; // body starts after the double CRLF
		char *file = url + 7; // skip "/files/"

		char path[512];
    	snprintf(path, sizeof(path), "%s%s", config.directory, file);
		printf("FilePath: %s\n", path);

		FILE *fp = fopen(path, "wb");
		if (!fp) {
			perror("fopen");
			free(url);
			return;
		}

		strcpy(response, "HTTP/1.1 201 Created\r\n\r\n");
		fwrite(data, 1, content_length, fp);
		fclose(fp);
	}
	else{
		strcpy(response, "HTTP/1.1 400 Bad Request\r\n\r\n");
	}

	free(url);
	return;
}

void handle_get_request(char* response, char* buffer){
	char* url = extract_URL(buffer, 0);
	printf("Extracted URL: %s\n", url);

	if(config.directory != NULL && strncmp(url, "/files/", 7) == 0){
		handle_file_request(response, url);
		free(url);
		return;
	}

	if(strcmp(url, "/") == 0){
		strcpy(response, "HTTP/1.1 200 OK\r\n\r\n");
	}
	else if (strncmp(url, "/echo/", 6) == 0){
		char *message = url + 6; // Skip "/echo/"
		int message_length = strlen(message);
		snprintf(response, BUF_1MB*2, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s", message_length, message);		
	}
	else if(strcmp(url, "/user-agent") == 0){
		char* userAgent = find_User_Agent(buffer);
		snprintf(response, BUF_1MB*2,"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s", (int)strlen(userAgent), userAgent);
		free(userAgent);
	}
	else strcpy(response, "HTTP/1.1 404 Not Found\r\n\r\n");

	free(url);

	return;
}

void handle_file_request(char* response, char* url){
	char file_path[256];
	char *file = url + 7; // Skip "/files/"
	sprintf(file_path, "%s%s", config.directory, file);
	printf("File path: %s\n", file_path);

	FILE *fp = fopen(file_path, "r");
	if (fp == NULL) strcpy(response, "HTTP/1.1 404 Not Found\r\n\r\n");
	else{
		fseek(fp, 0, SEEK_END);
		long file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char *file_content = (char *)malloc(file_size + 1);
		fread(file_content, 1, file_size, fp);
		fclose(fp);

		snprintf(response, BUF_1MB*2, "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: %ld\r\n\r\n%s", file_size, file_content);
		free(file_content);
	}
	
	return;
}