#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "../../include/socket/socket.h"
#include "../../include/parser/parser.h"

SocketEntity* connect_server() {
  printf("C: Starting Connection.\n");
  SocketEntity* entity = malloc(sizeof(SocketEntity*));
  
  tcp_handshake(entity);
  redis_handshake(entity);
  
  return entity;
}

int tcp_handshake(SocketEntity* entity){
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd == -1) {
    printf("C: Create socket failed.\n");
    return -1;
  }
  
  struct sockaddr_in redis_address;
  redis_address.sin_port = htons(REDIS_DEFAULT_PORT);
  redis_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  redis_address.sin_family = AF_INET;
  memset(&(redis_address.sin_zero), 0, sizeof(redis_address.sin_zero));
  
  int handshake = connect(socket_fd, (struct sockaddr*)&redis_address, sizeof(redis_address));
  if(handshake == -1) {
    printf("C: Failed to connect to Redis Server.\n");
    return -1;
  }

  printf("C: TCP Handshake - OK.\n");
  
  entity->socket_fd = socket_fd;
  return socket_fd;
}

int redis_handshake(SocketEntity* entity) {
  char buffer[REDIS_RESPONSE_MAX_SIZE];
  const char* hello = "*2\r\n$5\r\nHELLO\r\n$1\r\n3\r\n";

  ssize_t sent_buffer = send(entity->socket_fd, hello, strlen(hello), 0);
  if(sent_buffer == -1) {
    perror("C: send failed");
    return -1;
  }

  printf("C: Redis Handshake Initialized.\n");  
  ssize_t response_bytes = recv(entity->socket_fd, (void*)buffer, REDIS_RESPONSE_MAX_SIZE, 0);
  if (response_bytes == -1) {
    perror("C: recv failed");
    return -1;
  }

  buffer[response_bytes] = '\0';

  printf("C: Redis Handshake Received %zd bytes.\n", response_bytes);
  printf("C: Redis Handshake: Ok.\n");  

  return 0;
}

int send_command(char* args, SocketEntity* entity) {
  struct timeval stop, start;
  char buffer[REDIS_RESPONSE_MAX_SIZE];
  int array_length;
  double time_taken;

  char** params_array = string_to_array(args, &array_length);  
  char* request = parse_request(params_array, array_length);

  gettimeofday(&start, NULL);
  ssize_t sent_buffer = send(entity->socket_fd, request, strlen(request), 0);
  if(sent_buffer == -1){
    perror("C: send failed");
    return -1;
  }
  
  ssize_t response_bytes = recv(entity->socket_fd, (void*)buffer, REDIS_RESPONSE_MAX_SIZE, 0);
  gettimeofday(&stop, NULL);
  if (response_bytes == -1) {
    perror("C: recv failed");
   return -1;
  }

  time_taken = (stop.tv_sec - start.tv_sec) * 1000.0;      // segundos para ms
  time_taken += (stop.tv_usec - start.tv_usec) / 1000.0; 

  buffer[response_bytes] = '\0';
  char* output = parse_response(buffer);

  printf("\nS: %s\n", output);
  printf("\%.3f ms\n\n", time_taken);

  free(request);
  free(output);
  free(params_array);    
  return 0;
}