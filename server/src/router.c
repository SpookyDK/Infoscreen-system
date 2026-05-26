#include "cJSON.h"
#include "router.h"
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
// Function to handle Http request, should end with the / path
int handle_http(http_info *info) {

    // Handling is added here based on path and method,
    // uses if statements, could propably be a hashed switch
    if (strncmp(info->path, "/api/auth", 8) == 0) {

        printf("Path is AUTH\n");
        if (strncmp(info->method, "POST", 4) == 0) {
            char *body_start = strstr(info->buffer, "\r\n\r\n");
            char *screenID = NULL;
            char *Passwd = NULL;
            if (body_start != NULL) {
                // Advance past the 4 characters of "\r\n\r\n" to reach the actual payload
                body_start += 4;
                printf("Body found: %s\n", body_start);
                cJSON *json = cJSON_Parse(body_start);
                if (json == NULL) {
                    printf("Error parsing JSON data.\n");
                }
                cJSON *screenID_obj = cJSON_GetObjectItemCaseSensitive(json, "screenID");
                cJSON *password_obj = cJSON_GetObjectItemCaseSensitive(json, "password");
                if (cJSON_IsString(screenID_obj) && (screenID_obj->valuestring != NULL)) {
                    printf("Screen ID: %s\n", screenID_obj->valuestring);
                    screenID = malloc(strlen(screenID_obj->valuestring) + 1);
                    if (screenID != NULL) {
                        strcpy(screenID, screenID_obj->valuestring);
                    }
                }

                if (cJSON_IsString(password_obj) && (password_obj->valuestring != NULL)) {
                    printf("Password: %s\n", password_obj->valuestring);
                    Passwd = malloc(strlen(password_obj->valuestring) + 1);
                    if (Passwd != NULL) {
                        strcpy(Passwd, password_obj->valuestring);
                    }
                }

                // 4. CRITICAL: Free the memory allocated by cJSON
                printf("ID: %s, PSWD: %s\n", screenID, Passwd);
                cJSON_Delete(json);
                // Rember to free passwd, and screenID
                free(screenID);
                free(Passwd);
            }
        }
        close(info->client_fd);
        return 0;
    }
    if (strncmp(info->path, "/imageHandler.js", 16) == 0) {
        printf("Javascript requested\n");
        FILE *js_file = fopen("../client-display/imageHandler.js", "rb");
        if (js_file == NULL) {
            printf("JS file not avaiable\n");
        }
        fseek(js_file, 0, SEEK_END);
        size_t js_size = ftell(js_file);
        fseek(js_file, 0, SEEK_SET);
        printf("JS file open, size = %ld\n", js_size);
        char response_header[256];
        snprintf(response_header, sizeof(response_header),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/javascript\r\n"
                 "Content-Length: %ld\r\n"
                 "\r\n",
                 js_size);
        send(info->client_fd, response_header, strlen(response_header), 0);
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), js_file)) > 0) {
            send(info->client_fd, buffer, bytes_read, 0);
        }
        fclose(js_file);
        close(info->client_fd);
        return 0;
    }
    if (strncmp(info->path, "/api/testimage", 14) == 0) {
        FILE *img_file = fopen("images/test.png", "rb");
        if (img_file == NULL) {
            printf("Image file not avaiable\n");
        }
        fseek(img_file, 0, SEEK_END);
        size_t img_size = ftell(img_file);
        fseek(img_file, 0, SEEK_SET);
        printf("NEED TO SEND IMAGE\n");
        char response_header[256];
        snprintf(response_header, sizeof(response_header),

                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: image/png\r\n"
                 "Content-Length: %ld\r\n"
                 "\r\n",
                 img_size);
        send(info->client_fd, response_header, strlen(response_header), 0);
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), img_file)) > 0) {
            send(info->client_fd, buffer, bytes_read, 0);
        }
        fclose(img_file);
        close(info->client_fd);
        return 0;
    }
    // Case for getting the images associated with the user.
    if (strncmp(info->path, "/api/images", 14) == 0) {
        FILE *img_file = fopen("images/test.png", "rb");
        if (img_file == NULL) {
            printf("Image file not avaiable\n");
        }
        fseek(img_file, 0, SEEK_END);
        size_t img_size = ftell(img_file);
        fseek(img_file, 0, SEEK_SET);
        printf("NEED TO SEND IMAGE\n");
        char response_header[256];
        snprintf(response_header, sizeof(response_header),

                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: image/png\r\n"
                 "Content-Length: %ld\r\n"
                 "\r\n",
                 img_size);
        send(info->client_fd, response_header, strlen(response_header), 0);
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), img_file)) > 0) {
            send(info->client_fd, buffer, bytes_read, 0);
        }
        fclose(img_file);
        close(info->client_fd);
        return 0;
    }
    // Should be last case
    if (strncmp(info->path, "/", 1) == 0) {
        printf("HTML page requested\n");
        FILE *html_file = fopen("../client-display/base.html", "rb");
        if (html_file == NULL) {
            printf("Html file not avaiable\n");
        }
        fseek(html_file, 0, SEEK_END);
        size_t html_size = ftell(html_file);
        fseek(html_file, 0, SEEK_SET);
        printf("html file open, size = %ld\n", html_size);
        char response_header[256];
        snprintf(response_header, sizeof(response_header),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Content-Length: %ld\r\n"
                 "\r\n",
                 html_size);
        send(info->client_fd, response_header, strlen(response_header), 0);
        char buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), html_file)) > 0) {
            send(info->client_fd, buffer, bytes_read, 0);
        }
        fclose(html_file);
        close(info->client_fd);
        return 0;
    }
};
