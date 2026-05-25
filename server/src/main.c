#include "../inc/cJSON.h"
#include <netinet/in.h>
#include <router.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
char *http_response = "HTTP/1.1 200 OK\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "Content-Type: text/plain\r\n"
                      "Content-Length: 5\r\n"
                      "Connection: close\r\n"
                      "\r\n"
                      "hello";
int main() {
    struct sockaddr_in socket_address = {};
    int addrlen = sizeof(socket_address);
    int server_fd, client_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("failed to create socket\n");
        return 1;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR failed");
        close(server_fd);
        return 1;
    }
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&socket_address, addrlen) < 0) {
        printf("Failed to Bind Socket\n");
        close(server_fd);
        return 1;
    }
    if (listen(server_fd, 3) < 0) {
        printf("Failed to start listen\n");
        close(server_fd);
        return 1;
    }
    printf("Server should be listening at port %d\n", PORT);
    while (1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&socket_address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            close(server_fd);
            continue;
        }
        http_info info = {};
        char buffer[2048] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);
        printf("Recieved this\n");
        printf("%s\n", buffer);
        // Gets the path and method from the buffer via sscanf
        sscanf(buffer, "%15s %255s %15s", info.method, info.path, info.http_rev);
        if (strncmp(info.http_rev, "http", 4)) {
            printf("Recieved a HTTP request\n");
            handle_http(&info);
        }
        shutdown(client_fd, SHUT_WR);
        close(client_fd);
        continue;
    }
}
