SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 7000
#define BUFFER_SIZE 2000

int main() {
    int server_fd, client_fd;
    struct sockaddr_in servAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t clientLen = sizeof(clientAddr);
    ssize_t recv_len, send_len;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept an incoming connection
    if ((client_fd = accept(server_fd, (struct sockaddr*)&clientAddr, &clientLen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Receive a message from client
    if ((recv_len = recv(client_fd, buffer, BUFFER_SIZE, 0)) < 0) {
        perror("Receive failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    buffer[recv_len] = '\0';

    printf("Message from client: %s\n", buffer);

    // Send the same message back to client
    if ((send_len = send(client_fd, buffer, recv_len, 0)) != recv_len) {
        perror("Send failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Message sent back to client.\n");

    close(client_fd);
    close(server_fd);
    return 0;
}

CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7000
#define BUFFER_SIZE 2000
#define SERVER_IP "127.0.0.1"

int main() {
    int client_fd;
    struct sockaddr_in servAddr;
    char buffer[BUFFER_SIZE];
    ssize_t send_len, recv_len;

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &servAddr.sin_addr);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Connect failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Get message to send to server
    printf("Enter message to server: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    // Send message to server
    if ((send_len = send(client_fd, buffer, strlen(buffer), 0)) < 0) {
        perror("Message sending failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Receive message from server
    if ((recv_len = recv(client_fd, buffer, BUFFER_SIZE, 0)) < 0) {
        perror("Receive failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    buffer[recv_len] = '\0';
    printf("Server response: %s\n", buffer);

    close(client_fd);
    return 0;
}




























STRING REVERSE SERVER ONLY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 7000
#define BUFFER_SIZE 2000

void reverse_string(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in servAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t clientLen = sizeof(clientAddr);
    ssize_t recv_len, send_len;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept an incoming connection
    if ((client_fd = accept(server_fd, (struct sockaddr*)&clientAddr, &clientLen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Receive a message from client
        if ((recv_len = recv(client_fd, buffer, BUFFER_SIZE, 0)) < 0) {
            perror("Receive failed");
            close(client_fd);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        if (recv_len == 0) { // Client closed the connection
            printf("Client disconnected.\n");
            break;
        }

        buffer[recv_len] = '\0'; // Null-terminate the received string

        printf("Message from client: %s\n", buffer);

        // Reverse the string
        reverse_string(buffer);

        // Send the reversed string back to client
        if ((send_len = send(client_fd, buffer, recv_len, 0)) != recv_len) {
            perror("Send failed");
            close(client_fd);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Reversed message sent back to client: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}


















PALINDROME CHECK SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 7000
#define BUFFER_SIZE 2000

int is_palindrome(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        if (str[i] != str[n - i - 1]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in servAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t clientLen = sizeof(clientAddr);
    ssize_t recv_len, send_len;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept an incoming connection
    if ((client_fd = accept(server_fd, (struct sockaddr*)&clientAddr, &clientLen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Receive a message from client
        if ((recv_len = recv(client_fd, buffer, BUFFER_SIZE, 0)) < 0) {
            perror("Receive failed");
            close(client_fd);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        if (recv_len == 0) { // Client closed the connection
            printf("Client disconnected.\n");
            break;
        }

        buffer[recv_len] = '\0'; // Null-terminate the received string

        printf("Message from client: %s\n", buffer);

        // Check if the string is a palindrome
        char response[BUFFER_SIZE];
        if (is_palindrome(buffer)) {
            snprintf(response, BUFFER_SIZE, "'%s' is a palindrome.", buffer);
        } else {
            snprintf(response, BUFFER_SIZE, "'%s' is not a palindrome.", buffer);
        }

        // Send the response back to client
        if ((send_len = send(client_fd, response, strlen(response), 0)) < 0) {
            perror("Send failed");
            close(client_fd);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Response sent back to client: %s\n", response);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
