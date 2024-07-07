SERVER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
  int server, newSock, k = 5, m = 1, p;
  char buffer[1024];
  struct sockaddr_in servAddr, store;
  socklen_t addrSize = sizeof(servAddr);
  server = socket(PF_INET, SOCK_STREAM, 0);
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(5600);
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(servAddr.sin_zero, '\0', sizeof(servAddr.sin_zero));
  bind(server, (struct sockaddr * ) & servAddr, sizeof(servAddr));
  if (listen(server, 5) == 0) {
    printf("Listening\n");
  } else {
    printf("Error\n");
    exit(1);
  }
  newSock = accept(server, (struct sockaddr * ) & store, & addrSize);
  if (newSock == -1) {
    printf("Error in creating socket\n");
    exit(1);
  }
  while (k != 0) {
    int y = recv(newSock, buffer, sizeof(buffer), 0);
    if (y == -1) {
      printf("Error in receiving\n");
      exit(1);
    }
    if (strncmp(buffer, "frame", 5) == 0) {
      printf("Received %d successfully\n", m);
    } else {
      printf("Frame %d not received\n", m);
    }
    if (m % 2 == 0) {
      strcpy(buffer, "ack");
    } else {
      strcpy(buffer, "kca");
      printf("Ack lost\n");
      for (p = 1; p <= 3; p++) {
        printf("Waiting for %d seconds\n", p);
        sleep(1);
      }
      printf("Retransmitting Ack\n");
      strcpy(buffer, "ack");
    }
    printf("Sending Ack\n");
    int z = send(newSock, buffer, strlen(buffer), 0);
    if (z == -1) {
      printf("Error in sending\n");
      exit(1);
    }
    k--;
    m++;
  }
  close(newSock);
  close(server);
  return 0;
}

CLIENT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
  int client, y, x, k = 5, m = 1, p;
  char buffer[1024];
  struct sockaddr_in servAddr;
  socklen_t addrSize = sizeof(servAddr);
  client = socket(PF_INET, SOCK_STREAM, 0);
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(5600);
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(servAddr.sin_zero, '\0', sizeof(servAddr.sin_zero));
  y = connect(client, (struct sockaddr * ) & servAddr, addrSize);
  if (y == -1) {
    printf("Error in connection\n");
    exit(1);
  }
  while (k != 0) {
    if (m <= 5) {
      printf("Sending %d\n", m);
    }
    if (m % 2 == 0) {
      strcpy(buffer, "frame");
    } else {
      strcpy(buffer, "error");
      printf("Packet Loss\n");
      for (p = 1; p <= 3; p++) {
        printf("Waiting for %d seconds\n", p);
        sleep(1);
      }
      printf("Retransmitting...\n");
      strcpy(buffer, "frame");
    }
    int x = send(client, buffer, strlen(buffer), 0);
    if (x == -1) {
      printf("Error in sending\n");
      exit(1);
    } else {
      printf("Sent %d\n", m);
    }
    int z = recv(client, buffer, sizeof(buffer), 0);
    if (z == -1) {
      printf("Error in receiving data\n");
      exit(1);
    }
    k--;
    m++;
    if (strncmp(buffer, "ack", 3) == 0) {
      printf("Ack received for %d\n", m - 1);
    } else {
      printf("Ack not received\n");
    }
  }
  close(client);
  return 0;
}