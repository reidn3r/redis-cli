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

char* __parse_message(char* string, int8_t string_length, int8_t* index) {
  int8_t current_idx = *index + 1;

  while (current_idx < string_length && string[current_idx] != '\r') {
    current_idx++;
  }

  int8_t buffer_len = current_idx - (*index + 1);
  char* buffer = malloc(buffer_len + 1);
  if (!buffer) return NULL;

  strncpy(buffer, string + *index + 1, buffer_len);
  buffer[buffer_len] = '\0';

  *index = current_idx + 2;
  return buffer;
}

char* __parse_null(int8_t* index) {
  *index += 3;
  char* buffer = malloc(5);
  if(!buffer) return NULL;
  strcpy(buffer, "NULL");
  return buffer;
}

char* __parse_boolean(char* string, int8_t* index) {
  int8_t current_idx = *index + 1;
  int8_t buffer_size = string[current_idx] == 't' ? 4 : 5;
  char* buffer_content = string[current_idx] == 't' ? "True" : "False";

  char* buffer = malloc(buffer_size);
  if(!buffer) return NULL;
  strcpy(buffer, buffer_content);

  *index += 4;
  return buffer;
}

char* __parse_double(char* string, int8_t string_length, int8_t* index) {
    int8_t current_idx = *index + 1;

    while (current_idx < string_length && string[current_idx] != '\r') {
        current_idx++;
    }

    int8_t buffer_len = current_idx - (*index + 1);

    char* buffer = malloc(buffer_len + 1);
    if (!buffer) return NULL;

    strncpy(buffer, string + *index + 1, buffer_len);
    buffer[buffer_len] = '\0';

    *index = current_idx + 2;
    return buffer;
}

char* __parse_bulk_errors(char* string, int8_t string_length, int8_t* index){
  int8_t current_idx = *index + 1;

  int8_t size_str_start = current_idx;
  while (current_idx < string_length && string[current_idx] != '\r') {
      current_idx++;
  }

  int8_t size_str_len = current_idx - size_str_start;
  char size_buffer[16];
  strncpy(size_buffer, string + size_str_start, size_str_len);
  size_buffer[size_str_len] = '\0';

  int error_length = atoi(size_buffer);
  current_idx += 2;

  char* buffer = malloc(error_length + 1);
  if (!buffer) return NULL;

  strncpy(buffer, string + current_idx, error_length);
  buffer[error_length] = '\0';

  *index = current_idx + error_length + 2;
  return buffer;
}

char* __parse_verbatim_strings(char* string, int8_t string_length, int8_t* index) {
    int8_t current_idx = *index + 1;

    int8_t size_str_start = current_idx;
    while (current_idx < string_length && string[current_idx] != '\r') {
        current_idx++;
    }

    int8_t size_str_len = current_idx - size_str_start;
    char size_buffer[16];
    strncpy(size_buffer, string + size_str_start, size_str_len);
    size_buffer[size_str_len] = '\0';

    int total_length = atoi(size_buffer);
    current_idx += 2;

    char* buffer = malloc(total_length - 4 + 1);
    if (!buffer) return NULL;

    strncpy(buffer, string + current_idx + 4, total_length - 4);
    buffer[total_length - 4] = '\0';

    *index = current_idx + total_length + 2;
    return buffer;
}

char* __parse_map(char* string, int8_t string_length, int8_t* index){
    int8_t current_idx = *index + 1;
    int8_t size_str_start = current_idx;

    while (current_idx < string_length && string[current_idx] != '\r') {
        current_idx++;
    }

    int8_t map_str_len = current_idx - size_str_start;
    char size_buffer[16];
    strncpy(size_buffer, string + size_str_start, map_str_len);
    size_buffer[map_str_len] = '\0';

    int map_size = atoi(size_buffer);
    *index = current_idx + 2;

    char* buffer = malloc(1024);
    if (!buffer) return NULL;

    buffer[0] = '{';
    buffer[1] = '\0';

    int8_t pair_count = 0;
    while (*index < string_length && pair_count < map_size) {
        char* key = __parse_substring(string[*index], string, index, string_length);
        char* value = __parse_substring(string[*index], string, index, string_length);
        if (key && value) {
            if (pair_count > 0) strcat(buffer, ", ");
            strcat(buffer, key);
            strcat(buffer, ": ");
            strcat(buffer, value);
            free(key);
            free(value);
            pair_count++;
        } else {
            if (key) free(key);
            if (value) free(value);
            (*index)++;
        }
    }

    strcat(buffer, "}");
    return buffer;
}

char* __parse_set(char* string, int8_t string_length, int8_t* index) {
    int8_t current_idx = *index + 1;
    int8_t size_str_start = current_idx;

    while (current_idx < string_length && string[current_idx] != '\r') {
        current_idx++;
    }

    int8_t set_str_len = current_idx - size_str_start;
    char size_buffer[16];
    strncpy(size_buffer, string + size_str_start, set_str_len);
    size_buffer[set_str_len] = '\0';

    int set_size = atoi(size_buffer);
    *index = current_idx + 2;

    char* buffer = malloc(1024);
    if (!buffer) return NULL;

    buffer[0] = '{';
    buffer[1] = '\0';

    int8_t element_count = 0;
    while (*index < string_length && element_count < set_size) {
        char* element = __parse_substring(string[*index], string, index, string_length);
        if (element) {
            if (element_count > 0) strcat(buffer, ", ");
            strcat(buffer, element);
            free(element);
            element_count++;
        } else {
            (*index)++;
        }
    }

    strcat(buffer, "}");
    return buffer;
}

char* __parse_attributes(char* string, int8_t string_length, int8_t* index) {
    int8_t current_idx = *index + 1;
    int8_t size_str_start = current_idx;

    while (current_idx < string_length && string[current_idx] != '\r') {
        current_idx++;
    }

    int8_t attr_str_len = current_idx - size_str_start;
    char size_buffer[16];
    strncpy(size_buffer, string + size_str_start, attr_str_len);
    size_buffer[attr_str_len] = '\0';

    int attr_size = atoi(size_buffer);
    *index = current_idx + 2;

    char* buffer = malloc(1024);
    if (!buffer) return NULL;

    buffer[0] = '{';
    buffer[1] = '\0';

    int8_t attr_count = 0;
    while (*index < string_length && attr_count < attr_size * 2) {
        char* element = __parse_substring(string[*index], string, index, string_length);
        if (element) {
            if (attr_count > 0) strcat(buffer, ", ");
            strcat(buffer, element);
            free(element);
            attr_count++;
        } else {
            (*index)++;
        }
    }

    strcat(buffer, "}");
    return buffer;
}

char* __parse_push(char* string, int8_t string_length, int8_t* index) {
    int8_t current_idx = *index + 1;
    int8_t size_str_start = current_idx;

    while (current_idx < string_length && string[current_idx] != '\r') {
        current_idx++;
    }

    int8_t push_str_len = current_idx - size_str_start;
    char size_buffer[16];
    strncpy(size_buffer, string + size_str_start, push_str_len);
    size_buffer[push_str_len] = '\0';

    int push_size = atoi(size_buffer);
    *index = current_idx + 2;

    char* buffer = malloc(1024);
    if (!buffer) return NULL;

    buffer[0] = '<';
    buffer[1] = '[';
    buffer[2] = '\0';

    int8_t element_count = 0;
    while (*index < string_length && element_count < push_size) {
        char* element = __parse_substring(string[*index], string, index, string_length);
        if (element) {
            if (element_count > 0) strcat(buffer, ", ");
            strcat(buffer, element);
            free(element);
            element_count++;
        } else {
            (*index)++;
        }
    }

    strcat(buffer, "]>");
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

    case '+':
      return __parse_message(string, string_length, index);

    case '-':
      return __parse_message(string, string_length, index);

    case '#':
      return __parse_boolean(string, index);

    case '_':
      return __parse_null(index);

    case ',':
      return __parse_double(string, string_length, index);

    case '(':
      return __parse_double(string, string_length, index);

    case '!':
      return __parse_bulk_errors(string, string_length, index);

    case '=':
      return __parse_verbatim_strings(string, string_length, index);
    
    case '%':
      return __parse_map(string, string_length, index);

    case '~':
      return __parse_set(string, string_length, index);
    
    case '|':
      return __parse_attributes(string, string_length, index);

    case '>':
      return __parse_push(string, string_length, index);

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
