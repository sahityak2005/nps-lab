#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/dns_server.sock"
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
   
    // Create socket file descriptor
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
   
    // Set socket address parameters
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
   
    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
   
    printf("Connected to DNS Server\n");
   
    char domain[100];
    printf("Enter domain name: ");
    scanf("%s", domain);
   
    // Send domain name to server
    if (send(sockfd, domain, strlen(domain), 0) == -1) {
        perror("send failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
   
    // Receive response from server
    memset(buffer, 0, BUFFER_SIZE);
    if (recv(sockfd, buffer, BUFFER_SIZE, 0) == -1) {
        perror("recv failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
   
    printf("DNS Server Response: %s\n", buffer);
   
    close(sockfd);
    return 0;
}


