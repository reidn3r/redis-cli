#ifndef SOCKET_H
#define SOCKET_H

#define REDIS_DEFAULT_PORT 6379
#define SOCKET_PORT 2703
#define REDIS_RESPONSE_MAX_SIZE 512 * 1024
#define REQUEST_BUFFER_SIZE 1024

#include <arpa/inet.h>

typedef struct SocketEntity {
  int socket_fd;
  char ip[INET6_ADDRSTRLEN];
  int port;
} SocketEntity;

int tcp_handshake(SocketEntity* entity);
int redis_handshake(SocketEntity* entity);
int send_command(char* args, SocketEntity* entity);
void parse_arguments(int argc, char* argv[], char* ip, int* port);
SocketEntity* connect_server(char* ip, int port);

#endif