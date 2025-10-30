/*
  * @file           : em_cli_main.h
  * @brief          : Header file for em_cli.c
  *                   This file contains common definitions, data structures,
  *                   and function prototypes used by the Embedded CLI Framework.
 */

#ifndef INC_EM_CLI_MAIN_H_
#define INC_EM_CLI_MAIN_H_

#include <stddef.h>
#include <stddef.h>

/* Base type definitions*/
typedef int base_type;
typedef signed char int8_t;
#define FALSE ((base_type)0)
#define PASS  ((base_type)1)
#define CUSTOM_CLI_MAX_COMMANDS 10 // Maximum number of commands


/* Command definition structure */
typedef struct command_line_input {
	const char *command;
	const char *help_string;
	base_type (*command_interpreter)(char *write_buffer, size_t write_buffer_len, const char *command_string);
	int8_t expected_number_of_params;
} cli_command_definition;

extern cli_command_definition commands_array[CUSTOM_CLI_MAX_COMMANDS];
extern base_type command_count;

/* Function Prototypes */
base_type register_command(const cli_command_definition * const command_to_register);
base_type process_command(const char * const command_input, char *write_buffer, size_t write_buffer_len);

base_type get_number_of_parameters( const char *command_string );
const char *cli_get_parameter(const char *command_string, base_type wanted_parameter, base_type *parameter_string_length);

/* Help, Set, Get Command Function Prototypes */
base_type help_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string);
base_type set_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string);
base_type get_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string);

#endif /* INC_EM_CLI_MAIN_H_ */
