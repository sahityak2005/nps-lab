#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

void sortNumbers(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (str[i] > str[j]) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

void sortChars(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (str[i] < str[j]) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

int main() {
    int sockfd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(server_addr);
    char buffer[1024] = {0};
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process sorts numbers in ascending order
        printf("Child Process - PID: %d\n", getpid());

        recv(new_socket, buffer, sizeof(buffer), 0);
        printf("Received from client: %s\n", buffer);

        sortNumbers(buffer);
        send(new_socket, buffer, strlen(buffer), 0);

        close(new_socket);
    } else {
        // Parent process sorts characters in descending order
        printf("Parent Process - PID: %d\n", getpid());

        recv(new_socket, buffer, sizeof(buffer), 0);
        printf("Received from client: %s\n", buffer);

        sortChars(buffer);
        send(new_socket, buffer, strlen(buffer), 0);

        close(new_socket);
    }

    close(sockfd);
    return 0;
}
