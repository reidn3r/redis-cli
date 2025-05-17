#include "../include/socket/socket.h"
#include "../libs/linenoise/linenoise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  SocketEntity* socket = connect_server();
  char* line;

  while((line = linenoise("C: ")) != NULL) {
    send_command(line, socket);
    linenoiseHistoryAdd(line);
    free(line);
}
  return 0;
}
