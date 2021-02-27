#include "tecnicofs-client-api.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>

#define MAX_NAME_SIZE 50

struct sockaddr_un serv_addr, client_addr;
socklen_t servlen, clilen;

int sockfd;

void tfs_send(char buffer[MAX_INPUT_SIZE])
{
  if (sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&serv_addr, servlen) < 0)
  {
    printf("Error: %d\n", errno);
    puts(serv_addr.sun_path);
    puts(client_addr.sun_path);
    perror("tfsCreate: client: sendto error\n");
    exit(TECNICOFS_ERROR_CONNECTION_ERROR);
  }
}

void tfs_recv(char buffer[MAX_INPUT_SIZE])
{
  if (recvfrom(sockfd, buffer, strlen(buffer), 0, 0, 0) < 0)
  {
    fprintf(stderr, "tfsCreate: client: recvfrom error\n");
    exit(EXIT_FAILURE);
  }
}

int tfsCreate(char *filename, char nodeType)
{
  char buffer[MAX_INPUT_SIZE];

  sprintf(buffer, "c %s %c", filename, nodeType);

  tfs_send(buffer);

  tfs_recv(buffer);

  /* We typecast the char we sent to int, then return that */
  return atoi(buffer);
}

int tfsDelete(char *path)
{
  char buffer[MAX_INPUT_SIZE];

  sprintf(buffer, "d %s", path);

  tfs_send(buffer);

  tfs_recv(buffer);

  /* We typecast the char we sent to int, then return that */
  return atoi(buffer);
}

int tfsMove(char *from, char *to)
{
  char buffer[MAX_INPUT_SIZE];

  sprintf(buffer, "m %s %s", from, to);

  tfs_send(buffer);

  tfs_recv(buffer);

  /* We typecast the char we sent to int, then return that */
  return atoi(buffer);
}

int tfsLookup(char *path)
{
  char buffer[MAX_INPUT_SIZE];

  sprintf(buffer, "l %s", path);

  tfs_send(buffer);

  tfs_recv(buffer);

  /* We typecast the char we sent to int, then return that */
  return atoi(buffer);
}

int tfsPrint(char *path)
{
  char buffer[MAX_INPUT_SIZE];

  sprintf(buffer, "p %s", path);

  tfs_send(buffer);

  tfs_recv(buffer);

  /* We typecast the char we sent to int, then return that */
  return atoi(buffer);
}

int setSockAddrUn(char *path, struct sockaddr_un *addr)
{

  if (addr == NULL || path == NULL)
  {
    return -1;
  }

  bzero((char *)addr, sizeof(struct sockaddr_un));
  addr->sun_family = AF_UNIX;
  strcpy(addr->sun_path, path);

  return SUN_LEN(addr);
}

int tfsMount(char *sockPath)
{
  char client_socket[MAX_NAME_SIZE];

  /* Create client name */
  sprintf(client_socket, "/tmp/sock-%d", getpid());
  puts(client_socket);

  /* Create socket */
  if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
  {
    printf("tfsMount: client: error creating client socket.\n");
    return TECNICOFS_ERROR_OTHER;
  }

  /* Associate socket name */

  unlink(client_socket);

  clilen = setSockAddrUn(client_socket, &client_addr);

  if (bind(sockfd, (struct sockaddr *)&client_addr, clilen) < 0)
  {
    perror("tfsMount: client: bind error\n");
    exit(EXIT_FAILURE);
  }

  servlen = setSockAddrUn(sockPath, &serv_addr);

  return EXIT_SUCCESS;
}

int tfsUnmount()
{
  if (close(sockfd) < 0)
  {
    perror("tfsUnmount: client: failed to close\n");
    exit(EXIT_FAILURE);
  }

  if (unlink(client_addr.sun_path) < 0)
  {
    perror("tfsUnmount: client: failed to unlink\n");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
