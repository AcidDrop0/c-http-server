#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

char* extract_URL(char *request, int flag); // flag: 0 for GET, else for POST
char* find_User_Agent(char *request);
char* get_Word_Between(const char* fullString, const char* startDelimiter, const char* endDelimiter);
void check(int ret, const char* msg);  // Checks the return value of the socket functions and exits on error

#endif
