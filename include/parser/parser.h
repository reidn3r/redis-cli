#ifndef PARSER_H
#define PARSER_H

char* parse_request(char** arguments, int size);
char* __parse_single_string(char* arg);
char** string_to_array(char* string_params, int* length);

#endif