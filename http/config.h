#ifndef CONFIG_H
#define CONFIG_H

#include <netinet/in.h>

#define PORT 6767
//shortcut typedefs
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

// Server configuration structure
typedef struct {
    char *directory;
} ServerConfig;

extern ServerConfig config;

#endif
