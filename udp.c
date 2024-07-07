SERVER

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
int main() {
  int server;
  char servMsg[2000], cliMsg[2000];
  struct sockaddr_in servAddr, client_addr;
  int client_struct_length = sizeof(client_addr);
  server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (server < 0) {
    printf("Error while creating socket\n");
    exit(1);
  }
  printf("Socket created successfully\n");
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(2002);
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    printf("Couldn't bind to the port\n");
    exit(1);
  }
  printf("Binding done\n");
  printf("Listening...\n");
  if (recvfrom(server, cliMsg, sizeof(cliMsg), 0,
               (struct sockaddr *)&client_addr, &client_struct_length) < 0) {
    printf("Couldn't receive\n");
    exit(1);
  }
  printf("Msg from client: %s\n", cliMsg);
  strcpy(servMsg, cliMsg);
  if (sendto(server, servMsg, strlen(servMsg), 0,
             (struct sockaddr *)&client_addr, client_struct_length) < 0) {
    printf("Can't send\n");
    exit(1);
  }
  close(server);
  return 0;
}

CLIENT

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
void main() {
  int client;
  struct sockaddr_in servAddr;
  char servMsg[2000], cliMsg[2000];
  int server_struct_length = sizeof(servAddr);
  client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (client < 0) {
    printf("Error while creating socket\n");
    exit(1);
  }
  printf("Socket created successfully\n");
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(2002);
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  printf("Enter message to server: ");
  gets(cliMsg);
  if (sendto(client, cliMsg, strlen(cliMsg), 0, (struct sockaddr *)&servAddr,
             sizeof(servAddr)) < 0) {
    printf("Unable to send message\n");
    exit(1);
  }
  //(socklen_t *)&server_struct_length
  if (recvfrom(client, servMsg, sizeof(servMsg), 0,
               (struct sockaddr *)&servAddr, &server_struct_length) < 0) {
    printf("Error while receiving server's msg\n");
    exit(1);
  }
  close(client);
}

