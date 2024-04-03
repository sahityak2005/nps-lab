#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345
#define MAX_CLIENTS 2
#define BUFFER_SIZE 1024

void processClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in clientAddr;
    socklen_t addrSize = sizeof(clientAddr);

    // Get the socket address of the connected client
    if (getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrSize) == 0) {
        printf("Client connected from %s:%d\n",
               inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    }

    // Receive message from the client
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Received message from client: %s\n", buffer);

    // Open the file and append the received messages
    FILE *file = fopen("Manipal.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s", buffer);

    // Close the file
    fclose(file);

    // Close the client socket
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    int clientCount = 0;

    while (1) {
        // Accept a connection from a client
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
        if (clientSocket == -1) {
            perror("Error accepting connection");
            continue;
        }

        // Handle client in a separate process or thread
        if (fork() == 0) {
            // Child process
            close(serverSocket);
            processClient(clientSocket);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(clientSocket);
            clientCount++;

            if (clientCount >= MAX_CLIENTS) {
                printf("Number of clients exceeded. Terminating sessions.\n");
                // Notify all clients to terminate
                // (Note: In a real-world scenario, you would use proper communication here)
                break;
            }
        }
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
