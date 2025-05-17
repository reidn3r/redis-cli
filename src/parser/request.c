#define _GNU_SOURCE
#include "../../include/socket/socket.h"
#include "../../include/parser/parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

char* __parse_single_string(char* arg) {
  const char* crlf = "\r\n";
  int size = strlen(arg);

  char *string_size;
  asprintf(&string_size, "%d", size);

  char* buffer = malloc((sizeof(char) * size) + 20);
  snprintf(buffer, size + 20, "$%d%s%s%s", size, crlf, arg, crlf);

  return buffer;
}

char** string_to_array(char* string_params, int* length) {
  int i=0, count = 1;
  
  for (int i = 0; string_params[i] != '\0'; i++) {
    if (string_params[i] == ' ') {
      count++;
    }
  }

  char** tokens = malloc(sizeof(char*) * count);

  char* token = strtok(string_params, " ");
  while (token != NULL) {
    tokens[i++] = token;
    token = strtok(NULL, " ");
  }
  *length = i;

  return tokens;
}

char* parse_request(char** arguments, int array_length){  
  char* request = malloc(sizeof(char) * REQUEST_BUFFER_SIZE);
  char *string_size;
  
  asprintf(&string_size, "%d", array_length);
  snprintf(request, strlen(string_size) + 11, "%s%d%s", "*", array_length, "\r\n");

  char* current_argument;
  for(int i=0; i<array_length; i++){
    current_argument = __parse_single_string(arguments[i]);
    strcat(request, current_argument);
  }
  return request;
}