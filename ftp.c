SERVER

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void main() {
  int sockfd, newsockfd, n, len, i;
  char buff[1024];
  struct sockaddr_in servaddr;
  FILE *fp;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(6264);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    printf("Error in binding\n");
    exit(1);
  }
  printf("BINDING SUCESSFULL\n");
  if (listen(sockfd, 5) < 0) {
    printf("Error in listening\n");
    exit(1);
  }
  printf("LISTENING\n");

  socklen_t addrsize = sizeof(servaddr);
  newsockfd = accept(sockfd, (struct sockaddr *)&servaddr, &addrsize);

  if (newsockfd < 0) {
    printf("Error in accepting\n");
    exit(1);
  }
  printf("ACCEPTED\n");
  int charno = recv(newsockfd, buff, 1024, 0);
  if (charno < 0) {
    printf("Error in receiving\n");
    exit(1);
  }
  buff[charno] = '\0';
  printf("RECEIVED\n");
  printf("FIlename=%s", buff);
  fp = fopen(buff, "r");
  if (fp == NULL) {
    printf("File not found\n");
    exit(1);
  }
  char content[1024];
  while (fgets(content, 1024, fp) != NULL) {
    if (send(newsockfd, content, strlen(content), 0) < 0) {
      printf("Error in sending\n");
      exit(1);
    }

    printf("LINE sent\n");
  }
  fclose(fp);
  close(newsockfd);
  close(sockfd);
}

CLIENT

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void main() {
  int sockfd;
  char cliMsg[1024], servMsg[1024], newfname[1024];
  FILE *fp;
  struct sockaddr_in servaddr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(6264);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    printf("Error in connecting\n");
    exit(1);
  }
  printf("Enter name of file to be requested\n");
  scanf("%s", cliMsg);
  printf("Enter new name of file \n");
  scanf("%s", newfname);
  fp = fopen(newfname, "w");
  send(sockfd, cliMsg, strlen(cliMsg), 0);

  int charno;

  while ((charno = recv(sockfd, servMsg, sizeof(servMsg), 0)) > 0) {
    servMsg[charno] = '\0';
    fputs(servMsg, fp);
  }
  fclose(fp);
  close(sockfd);
}