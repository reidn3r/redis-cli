#ifndef SOCKET_H
#define SOCKET_H

#define REDIS_DEFAULT_PORT 6379
#define SOCKET_PORT 2703
#define REDIS_RESPONSE_MAX_SIZE 512 * 1024
#define REQUEST_BUFFER_SIZE 1024

typedef struct SocketEntity {
  int socket_fd;
} SocketEntity;

int tcp_handshake(SocketEntity* entity);
int redis_handshake(SocketEntity* entity);
int send_command(char* args, SocketEntity* entity);
SocketEntity* connect_server();

#endif