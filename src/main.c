#include "../include/socket/socket.h"
#include "../include/parser/parser.h"
#include <stdio.h>
#include <string.h>

int main() {
  char input_buffer[1024];
  SocketEntity* socket = connect_server();

  while (1) {
    printf("\nC: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
      input_buffer[strcspn(input_buffer, "\n")] = '\0';
      send_command(input_buffer, socket);
    }
  }

  return 0;
}
