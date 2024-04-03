#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 256

void checkAnagrams(int client_socket, const char *str1, const char *str2) {
    char result[BUFFER_SIZE];

    if (strlen(str1) != strlen(str2)) {
        sprintf(result, "Not anagrams: Lengths are different");
    } else {
        int freq[256] = {0};

        for (int i = 0; i < strlen(str1); i++) {
            freq[str1[i]]++;
            freq[str2[i]]--;
        }

        int isAnagram = 1;
        for (int i = 0; i < 256; i++) {
            if (freq[i] != 0) {
                isAnagram = 0;
                break;
            }
        }

        if (isAnagram) {
            sprintf(result, "Anagrams");
        } else {
            sprintf(result, "Not anagrams");
        }
    }

    send(client_socket, result, strlen(result), 0);
}

void *handleClient(void *arg) {
    int client_socket = *((int *)arg);
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    char address[INET_ADDRSTRLEN];

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    getpeername(client_socket, (struct sockaddr *)&client_addr, &addr_len);
    inet_ntop(AF_INET, &(client_addr.sin_addr), address, INET_ADDRSTRLEN);

    time_t t;
    time(&t);
    printf("Connection from %s at %s\n", address, ctime(&t));

    // Prompt the client to enter two strings
    send(client_socket, "Enter string 1: ", strlen("Enter string 1: "), 0);
    recv(client_socket, buffer1, BUFFER_SIZE, 0);

    send(client_socket, "Enter string 2: ", strlen("Enter string 2: "), 0);
    recv(client_socket, buffer2, BUFFER_SIZE, 0);

    // Check if the entered strings are anagrams
    checkAnagrams(client_socket, buffer1, buffer2);

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);

        if (client_socket < 0) {
            perror("Accept failed");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        pthread_t thread;
        pthread_create(&thread, NULL, handleClient, (void *)&client_socket);
        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
}

