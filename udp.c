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
    int server;
    struct sockaddr_in servAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t clientLen = sizeof(clientAddr);
    ssize_t recv_len, send_len;

    if ((server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);

    if (bind(server, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(server);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    if ((recv_len = recvfrom(server, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &clientAddr, &clientLen)) < 0) {
        perror("Receive failed");
        close(server);
        exit(EXIT_FAILURE);
    }
    buffer[recv_len] = '\0';

    printf("Message from client: %s\n", buffer);

    if ((send_len = sendto(server, buffer, recv_len, 0, (struct sockaddr *) &clientAddr, clientLen)) != recv_len) {
        perror("Send failed");
        close(server);
        exit(EXIT_FAILURE);
    }

    printf("Message sent back to client.\n");

    close(server);
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
    int client;
    struct sockaddr_in servAddr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(servAddr);

    if ((client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &servAddr.sin_addr);

    printf("Enter message to server: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    if (sendto(client, buffer, strlen(buffer), 0, (struct sockaddr*)&servAddr, addr_len) < 0) {
        perror("Message sending failed");
        exit(EXIT_FAILURE);
    }

    recvfrom(client, buffer, BUFFER_SIZE, 0, NULL, NULL);
    printf("Server response: %s\n", buffer);

    close(client);
    return 0;
}
