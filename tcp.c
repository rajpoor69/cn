SERVER

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
void main() {
  int server, newSock;
  char buffer[1024];
  struct sockaddr_in servAddr;
  struct sockaddr_storage store;
  socklen_t addrSize;
  server = socket(AF_INET, SOCK_STREAM, 0);
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(6265);
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr));
  if (listen(server, 5) == 0)
    printf("Listening\n");
  else
    printf("Error\n");
  newSock = accept(server, (struct sockaddr *)&store, &addrSize);
  recv(newSock, buffer, 1024, 0);
  printf("2.Data Received: %s", buffer);
  strcpy(buffer, "Hi This is Server\n");
  printf("3.Sending Data to client...\n");
  send(newSock, buffer, 19, 0);
  close(newSock);
  close(server);
}

CLIENT

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
void main() {
  int client;
  char buffer[1024];
  struct sockaddr_in servAddr;
  socklen_t addrSize;
  client = socket(AF_INET, SOCK_STREAM, 0);
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(6265);
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  connect(client, (struct sockaddr *)&servAddr, sizeof(servAddr));
  printf("1. Sending data to server...\n");
  // Additional code for data exchange
  strcpy(buffer, "Hi This is client\n");
  send(client, buffer, 19, 0);
  recv(client, buffer, 1024, 0);
  printf("4. Data received: %s", buffer);
  close(client);
}
