#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void displayMenu() {
    printf("Menu:\n");
    printf("1. Registration Number\n");
    printf("2. Name of the Student\n");
    printf("3. Subject Code\n");
    printf("Enter your choice: ");
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
   
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
   
    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
   
    printf("Connected to server\n");
   
    int choice;
    char buffer[BUFFER_SIZE];
    while(1) {
        displayMenu();
        scanf("%d", &choice);
        send(sockfd, &choice, sizeof(int), 0);
       
        switch(choice) {
            case 1:
            case 2:
            case 3:
                // Receive response from server
                memset(buffer, 0, BUFFER_SIZE);
                recv(sockfd, buffer, BUFFER_SIZE, 0);
                printf("Server Response:\n%s\n", buffer);
                break;
            default:
                printf("Invalid option\n");
        }
    }
   
    close(sockfd);
    return 0;
}


