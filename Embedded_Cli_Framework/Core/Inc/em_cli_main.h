/*
  * @file           : em_cli_main.h
  * @brief          : Header file for em_cli.c
  *                   This file contains common definitions, data structures,
  *                   and function prototypes used by the Embedded CLI Framework.
 */

#ifndef INC_EM_CLI_MAIN_H_
#define INC_EM_CLI_MAIN_H_

#include <stddef.h>

/* Base type definitions */
typedef int base_type_t;
typedef signed char int8_t;

#define FALSE ((base_type_t)0)
#define PASS  ((base_type_t)1)
#define CUSTOM_CLI_MAX_COMMANDS 10 // Maximum number of commands

#define ARRAY_BASED_COMMAND_REGISTER ((base_type_t)1)
//#undef ARRAY_BASED_COMMAND_REGISTER
#undef DEBUG

/* Command definition structure */
typedef struct command_line_input {
	const char *command;
	const char *help_string;
	base_type_t (*command_interpreter)(char *write_buffer, size_t write_buffer_len, const char *command_string);
	int8_t expected_number_of_params;
} cli_command_definition;

#ifndef ARRAY_BASED_COMMAND_REGISTER
/* List Of Commands */
typedef struct command_input_list {
    const CLI_Command_Definition *command_line_definition;
    struct command_input_list *next;
} cli_Definition_List_Item;
#endif

extern cli_command_definition commands_array[CUSTOM_CLI_MAX_COMMANDS];
extern base_type_t command_count;

/* Function prototypes */
base_type_t register_command(const cli_command_definition * const command_to_register);
base_type_t process_command(const char * const command_input, char *write_buffer, size_t write_buffer_len);

base_type_t get_number_of_parameters(const char *command_string);
const char *cli_get_parameter(const char *command_string, base_type_t wanted_parameter, base_type_t *parameter_string_length);

/* Help, Set, Get Command Function Prototypes */
base_type_t help_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string);
base_type_t set_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string);
base_type_t get_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string);

#endif /* INC_EM_CLI_MAIN_H_ */
