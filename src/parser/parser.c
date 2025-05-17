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

char* __parse_integer(char* string, int8_t string_length, int8_t* index) {
  int8_t current_idx = *index + 1;
  
  while(current_idx < string_length && string[current_idx] != '\r'){
    current_idx++;
  }
  
  int8_t buffer_len = current_idx - (*index + 1);

  char* buffer = malloc(buffer_len + 1);
  if(!buffer) return NULL;

  strncpy(buffer, string + *index + 1, buffer_len);
  buffer[buffer_len] = '\0';
  
  *index = current_idx + 2; //ignora \n
  return buffer;
}

char* __parse_string(char* string, int8_t string_length, int8_t* index) {
    int8_t current_idx = *index + 1;

    int8_t size_str_start = current_idx;
    while (current_idx < string_length && string[current_idx] != '\r') {
        current_idx++;
    }

    int8_t size_str_len = current_idx - size_str_start;
    char size_buffer[16];

    strncpy(size_buffer, string + size_str_start, size_str_len);
    size_buffer[size_str_len] = '\0';

    int string_size = atoi(size_buffer);

    current_idx += 2;
    int8_t string_start_index = current_idx;

    char* buffer = malloc(string_size + 1);
    if (!buffer) return NULL;

    strncpy(buffer, string + string_start_index, string_size);
    buffer[string_size] = '\0';

    *index = string_start_index + string_size;
    return buffer;
}

char* __parse_array(char* string, int8_t string_length, int8_t* index) {
  int8_t current_idx = *index + 1;
  int8_t size_str_start = current_idx;
  
  while (current_idx < string_length && string[current_idx] != '\r') {
      current_idx++;
  }

  int8_t array_str_len = current_idx - size_str_start;
  char size_buffer[16];
  strncpy(size_buffer, string + size_str_start, array_str_len);
  size_buffer[array_str_len] = '\0';

  int array_size = atoi(size_buffer);
  *index = current_idx + 2;

  char* buffer = malloc(1024);
  if (!buffer) return NULL;
  
  buffer[0] = '[';
  buffer[1] = '\0';

  int8_t members_count=0;
  while(*index < string_length && members_count < array_size){
    char* substring = __parse_substring(string[*index], string, index, string_length);
    if (substring) {
      if(members_count > 0) strcat(buffer, ", ");
      strcat(buffer, substring);
      free(substring);
      members_count++;
    }
    else{
      (*index)++;
    }
  }

  strcat(buffer, "]");
  return buffer;
}

char* __parse_substring(char delimiter, char* string, int8_t* index, int8_t string_length){
  switch (delimiter) {
    case ':':
      return __parse_integer(string, string_length, index);
      break;
    
    case '$':
      return __parse_string(string, string_length, index);
      break;
    
    case '*':
      return __parse_array(string, string_length, index);
  
    default:
      return NULL;
      break;
  }
}

char* parse_response(char* response) {
  int8_t current_idx=0, response_len = strlen(response);
  char* output = malloc(1024);
  if(!output) return NULL;
  output[0] = '\0';

  while(current_idx < response_len) {
    char* substring = __parse_substring(response[current_idx], response, &current_idx, response_len);
    if(substring){
        strcat(output, substring);
        free(substring);
        strcat(output, " ");
    } else {
        current_idx++; 
    }
  }
  return output;
}
