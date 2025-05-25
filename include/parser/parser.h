#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>

char* parse_request(char** arguments, int size);
char** string_to_array(char* string_params, int* length);
char* __parse_single_string(char* arg);

char* parse_response(char* response);
char* __parse_substring(char delimiter, char* string, int8_t* index, int8_t string_length);
char* __parse_array(char* string, int8_t string_length, int8_t* index);
char* __parse_string(char* string, int8_t string_length, int8_t* index);
char* __parse_message(char* string, int8_t string_length, int8_t* index);
char* __parse_null(int8_t* index);
char* __parse_boolean(char* string, int8_t* index);
char* __parse_double(char* string, int8_t string_length, int8_t* index);
char* __parse_bulk_errors(char* string, int8_t string_length, int8_t* index);
char* __parse_verbatim_strings(char* string, int8_t string_length, int8_t* index);
char* __parse_map(char* string, int8_t string_length, int8_t* index);
char* __parse_set(char* string, int8_t string_length, int8_t* index);
char* __parse_attributes(char* string, int8_t string_length, int8_t* index);
char* __parse_push(char* string, int8_t string_length, int8_t* index);

#endif