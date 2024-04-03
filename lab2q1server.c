#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<string.h>

#define myport 1234

int main() {
    int acptfd = 0, size;
    char buffer[30];
    char reply[30];

    // Socket creation
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Setting up server address
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(myport);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    // Binding
    bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    // Listening
    listen(sockfd, 10);

    printf("The server is ready for chatting.\n");

    size = sizeof(struct sockaddr);

    // Connection accepted but data yet to be received
    acptfd = accept(sockfd, (struct sockaddr *) &client_addr, &size);

    int pid = fork();

    while(1) {
        if(pid == 0) {
            // If child, receive from the client and display it
            recv(acptfd, buffer, 30, 0);
            printf("\nClient's message = %s\n", buffer);
        } else {
            // If parent, read from the user and send it to the client
            printf("\nEnter your reply: ");
            fgets(reply, 30, stdin);
            if (strncmp(reply, "BYE", 3) == 0)
                break;
            send(acptfd, reply, 30, 0);
        }
    }

    close(sockfd);
    printf("\nClosing the socket\n\n");

    return 0;
}


