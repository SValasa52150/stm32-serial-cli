/*
  * @file           : register_cmd.c
  * @brief          : Command registration module for Embedded CLI Framework
 */

#include "em_cli_main.h"
extern base_type command_count;

base_type register_command(const cli_command_definition * const command_to_register) {
	base_type is_registered = FALSE;

	if (command_count < CUSTOM_CLI_MAX_COMMANDS){
		commands_array[command_count] = *command_to_register;
		command_count++;
		is_registered = PASS;
	}

	return is_registered;
}
