#define _GNU_SOURCE
#include "../../include/socket/socket.h"
#include "../../include/parser/parser.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
