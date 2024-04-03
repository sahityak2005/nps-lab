#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080

void permute(char *str, int start, int end) {
    if (start == end) {
        printf("%s\n", str);
    } else {
        for (int i = start; i <= end; i++) {
            // Swap characters at start and i position
            char temp = str[start];
            str[start] = str[i];
            str[i] = temp;

            // Recursively find permutations
            permute(str, start + 1, end);

            // Undo the swap to backtrack
            temp = str[start];
            str[start] = str[i];
            str[i] = temp;
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;
    len = sizeof(cliaddr);

    n = recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';

    printf("Client: %s\n", buffer);
    printf("All permutations of the string:\n");
    permute(buffer, 0, strlen(buffer) - 1);

    close(sockfd);
    return 0;
}
