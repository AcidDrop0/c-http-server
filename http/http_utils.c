#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "http_utils.h"


char* get_Word_Between(const char* fullString, const char* startDelimiter, const char* endDelimiter) {
    char* startPtr;
    char* endPtr;
    int wordLength;
    char* extractedWord = NULL;

    // Find the start delimiter
    startPtr = strstr(fullString, startDelimiter);
    if (startPtr == NULL) {
        return NULL; // Start delimiter not found
    }

    // Move past the start delimiter
    startPtr += strlen(startDelimiter);

    // Find the end delimiter from the new start point
    endPtr = strstr(startPtr, endDelimiter);
    if (endPtr == NULL) {
        return NULL; // End delimiter not found
    }

    // Calculate the length of the word
    wordLength = endPtr - startPtr;

    // Allocate memory for the extracted word (+1 for null terminator)
    extractedWord = (char*)malloc(sizeof(char) * (wordLength + 1));
    if (extractedWord == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Copy the word and null-terminate it
    strncpy(extractedWord, startPtr, wordLength);
    extractedWord[wordLength] = '\0';

    return extractedWord;
}

char* extract_URL(char *request, int flag){
	if(flag == 0) return get_Word_Between(request, "GET ", " HTTP/");
	else return get_Word_Between(request, "POST ", " HTTP/");
}

char* find_User_Agent(char *request){
	return get_Word_Between(request, "User-Agent: ", "\r\n");
}

void check(int ret, const char* msg){
	if(ret < 0){
		perror(msg);
		exit(EXIT_FAILURE);
	}
}