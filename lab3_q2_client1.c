#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define BUFFER_SIZE 256

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Receive and display prompt for string 1
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // Get string 1 from the user
    fgets(buffer, BUFFER_SIZE, stdin);
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive and display prompt for string 2
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    // Get string 2 from the user
    fgets(buffer, BUFFER_SIZE, stdin);
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive and display the result from the server
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("Result from server: %s\n", buffer);

    close(client_socket);
    return 0;
}

