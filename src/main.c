#include "../include/socket/socket.h"
#include "../libs/linenoise/linenoise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  char ip[INET_ADDRSTRLEN] = "127.0.0.1";
  int port = REDIS_DEFAULT_PORT;
  parse_arguments(argc, argv, ip, &port);
  
  SocketEntity* socket = connect_server(ip, port);
  char* line;

  while((line = linenoise("C: ")) != NULL) {
    send_command(line, socket);
    linenoiseHistoryAdd(line);
    free(line);
}
  return 0;
}
