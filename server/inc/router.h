#include <stdlib.h>

typedef struct {
    int client_fd;
    char buffer[2048];
    char path[256];
    char method[16];
    char http_rev[16];
} http_info;

int handle_http(http_info *info);
