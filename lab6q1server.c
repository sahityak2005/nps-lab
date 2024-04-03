#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

struct Student {
    char reg_number[20];
    char name[100];
    char address[100];
    char dept[50];
    char semester[20];
    char section;
    char courses[100];
    char subject_code[10];
    int marks;
};

void handleRegistrationNumber(int client_sock) {
    struct Student student;
    char buffer[BUFFER_SIZE];
   
    // Simulate fetching student details based on registration number
    strcpy(student.name, "John Doe");
    strcpy(student.address, "123 Main St, Anytown, USA");
   
    // Send student details to client
    sprintf(buffer, "Name: %s\nAddress: %s\nChild Process PID: %d\n", student.name, student.address, getpid());
    send(client_sock, buffer, strlen(buffer), 0);
}

void handleStudentName(int client_sock) {
    struct Student student;
    char buffer[BUFFER_SIZE];
   
    // Simulate fetching student enrollment details based on name
    strcpy(student.dept, "Computer Science");
    strcpy(student.semester, "3rd");
    student.section = 'A';
    strcpy(student.courses, "CSE101, CSE102, MAT101");
   
    // Send student enrollment details to client
    sprintf(buffer, "Department: %s\nSemester: %s\nSection: %c\nCourses: %s\nChild Process PID: %d\n", student.dept, student.semester, student.section, student.courses, getpid());
    send(client_sock, buffer, strlen(buffer), 0);
}

void handleSubjectCode(int client_sock) {
    struct Student student;
    char buffer[BUFFER_SIZE];
   
    // Simulate fetching marks based on subject code
    student.marks = 85; // Simulated marks for subject code
   
    // Send marks to client
    sprintf(buffer, "Marks Obtained: %d\nChild Process PID: %d\n", student.marks, getpid());
    send(client_sock, buffer, strlen(buffer), 0);
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
   
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
   
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
   
    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
   
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
   
    printf("Server is listening on port %d\n", PORT);
   
    while(1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
       
        printf("Client connected\n");
       
        int choice;
        // Receive client's choice
        recv(new_socket, &choice, sizeof(int), 0);
       
        switch(choice) {
            case 1:
                handleRegistrationNumber(new_socket);
                break;
            case 2:
                handleStudentName(new_socket);
                break;
            case 3:
                handleSubjectCode(new_socket);
                break;
            default:
                printf("Invalid option\n");
        }
       
        close(new_socket);
    }
   
    close(server_fd);
    return 0;
}

