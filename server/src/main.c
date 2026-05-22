#include <netinet/in.h>
#include <stdio.h>
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
        char buffer[2048] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);
        printf("Recieved this\n");
        printf("%s\n", buffer);
        printf("\n--- --- --- --- ---\n");
        char path[256] = {0};
        char method[16] = {0};
        sscanf(buffer, "%15s %255s", method, path);
        if (strncmp(path, "/api/auth", 8) == 0) {
            printf("Path is AUTH\n");
        }
        if (strncmp(buffer, "OPTIONS", 7) == 0) {
            char *options_response = "HTTP/1.1 204 No Content\r\n"
                                     "Access-Control-Allow-Origin: *\r\n"
                                     "Access-Control-Allow-Methods: GET, POST, OPTIONS, PUT, DELETE\r\n"
                                     // Tell the browser these headers are allowed!
                                     "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
                                     "Access-Control-Max-Age: 86400\r\n" // Cache this preflight for 24 hours
                                     "Connection: close\r\n"
                                     "\r\n";
            send(client_fd, options_response, strlen(options_response), 0);
            printf("Options message sent to client. \n");
        } else {
            char *http_response = "HTTP/1.1 200 OK\r\n"
                                  "Access-Control-Allow-Origin: *\r\n"
                                  "Access-Control-Allow-Methods: GET, POST, OPTIONS, PUT, DELETE\r\n"
                                  // Tell the browser these headers are allowed!
                                  "Access-Control-Allow-Headers: Content-Type, Authorization\r\n"
                                  "Access-Control-Max-Age: 86400\r\n" // Cache this preflight for 24 hours
                                  "Content-Type: text/plain\r\n"
                                  "Content-Length: 5\r\n"
                                  "Connection: close\r\n"
                                  "\r\n"
                                  "hello";

            send(client_fd, http_response, strlen(http_response), 0);
            printf("Handled actual request data.\n");
        }
        shutdown(client_fd, SHUT_WR);
        close(client_fd);
        continue;
    }
}
