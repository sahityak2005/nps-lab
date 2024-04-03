#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/dns_server.sock"
#define BUFFER_SIZE 1024

struct DNS_Record {
    char domain[100];
    char ip_address[20];
};

struct DNS_Record database[] = {
    {"www.example.com", "192.168.1.1"},
    {"www.google.com", "8.8.8.8"},
    {"www.yahoo.com", "98.139.183.24"}
};

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];
   
    // Create socket file descriptor
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
   
    // Set socket address parameters
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
    unlink(SOCKET_PATH); // Remove any existing socket file
   
    // Bind the socket to the specified address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
   
    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
   
    printf("DNS Server is listening on %s\n", SOCKET_PATH);
   
    socklen_t client_addr_len = sizeof(client_addr);
   
    while (1) {
        // Accept incoming connection
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
       
        printf("Client connected\n");
       
        // Receive domain name from client
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(client_fd, buffer, BUFFER_SIZE, 0) == -1) {
            perror("recv failed");
            close(client_fd);
            continue;
        }
       
        // Search for domain in the database
        int found = 0;
        for (int i = 0; i < sizeof(database) / sizeof(database[0]); ++i) {
            if (strcmp(database[i].domain, buffer) == 0) {
                // Send IP address to client
                if (send(client_fd, database[i].ip_address, strlen(database[i].ip_address), 0) == -1) {
                    perror("send failed");
                }
                found = 1;
                break;
            }
        }
       
        if (!found) {
            // Send error message to client if domain not found
            if (send(client_fd, "Domain not found", strlen("Domain not found"), 0) == -1) {
                perror("send failed");
            }
        }
       
        close(client_fd);
    }
   
    close(server_fd);
    return 0;
}
