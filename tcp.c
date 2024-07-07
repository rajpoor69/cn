SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8081

void chat(int connfd) {
    char buff[MAX];
    while (1) {
        memset(buff, 0, MAX);
        read(connfd, buff, sizeof(buff));
        printf("From Client: %sTo Client: ", buff);
        memset(buff, 0, MAX);
        fgets(buff, MAX, stdin);
        write(connfd, buff, strlen(buff) + 1);
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created\n");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Socket bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully binded\n");

    if (listen(sockfd, 5) != 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening\n");

    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    if (connfd < 0) {
        perror("Server accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server accepted the client\n");

    chat(connfd);

    close(sockfd);
    return 0;
}

CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8081

void chat(int sockfd) {
    char buff[MAX];
    while (1) {
        memset(buff, 0, MAX);
        printf("To Server: ");
        fgets(buff, MAX, stdin);
        write(sockfd, buff, strlen(buff) + 1);
        memset(buff, 0, MAX);
        read(sockfd, buff, sizeof(buff));
        printf("From Server: %s", buff);
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created\n");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection with the server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server\n");

    chat(sockfd);
    close(sockfd);
    return 0;
}
