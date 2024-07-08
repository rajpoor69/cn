CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in server_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int len, n;

    // Get user input
    printf("Enter an integer: ");
    fgets(buffer, MAXLINE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline character

    // Send message to server
    sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &server_addr, sizeof(server_addr));
    printf("Message sent to server: %s\n", buffer);

    // Receive server's response
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &server_addr, &len);
    buffer[n] = '\0';
    printf("Server responded with square: %s\n", buffer);

    close(sockfd);
    return 0;
}


SERVER


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in server_addr, client_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;
    len = sizeof(client_addr);

    // Receive message from client
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &client_addr, &len);
    buffer[n] = '\0';
    printf("Client sent: %s\n", buffer);

    // Convert received string to integer
    int num = atoi(buffer);

    // Calculate the square of the number
    int square = num * num;
    snprintf(buffer, MAXLINE, "%d", square);

    // Send square back to client
    sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &client_addr, len);
    printf("Square sent to client: %s\n", buffer);

    close(sockfd);
    return 0;
}





