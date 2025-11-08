/**
  ******************************************************************************
  * @file   command_interpreter.c
  * @brief  Command interpreter implementations: help, set, get.
  ******************************************************************************
  */

#include <string.h>
#include <stdio.h>
#include "em_cli_main.h"

extern char write_buffer[512];
extern cli_Definition_List_Item registered_commands;

/* Help Command Implementation */
base_type_t help_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string)
{
	const char *registered_command_string = NULL;
	base_type_t command_string_length;
	memset(write_buffer, 0, write_buffer_len);

	#ifdef ARRAY_BASED_COMMAND_REGISTER
	base_type_t command_index = 0;
	for (command_index = 0; command_index < command_count; command_index++)
	{
		if(command_index == 0) continue;
		registered_command_string = commands_array[command_index].command;
		command_string_length = strlen( registered_command_string );

		if(command_index == 1) {
		   strncpy(write_buffer, registered_command_string, command_string_length);
		} else {
			strcat(write_buffer,"\r\n");
			strcat(write_buffer,registered_command_string);
		}
	}
	char *msg = output_string( write_buffer,"\n SATHVIK : List of commands - \n");
	UART_Transmit(msg);

	#else
	const cli_Definition_List_Item *command = &registered_commands;
	command = command->next->next;
	for( command ; command != NULL; command = command->next )
	{
		registered_command_string = command->command_line_definition->command;
		command_string_length = strlen( registered_command_string );

		if(strlen(write_buffer) == 0) {
		strncpy(write_buffer, registered_command_string, command_string_length);
		} else {
			strcat(write_buffer,"\r\n");
			strcat(write_buffer, registered_command_string);
		}
	}
	 char *msg = output_string( write_buffer,"\n SATHVIK : List of commands - \n");
	 UART_Transmit(msg);
	#endif

	// Return FALSE if no more strings are to be returned
	return (FALSE);
}

/* Set Command Implementation */
base_type_t set_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string)
{
    char * parameter;
    base_type_t string_length = 0;

    /* Start with an empty string. */
    memset(write_buffer, 0, write_buffer_len);
    parameter = (char *) cli_get_parameter(command_string, 1, &string_length);
    strncpy(write_buffer, parameter, string_length);
    char *msg1 = output_string( write_buffer,"\n SATHVIK : Param 1- ");
    UART_Transmit(msg1);

    /* Start with an empty string. */
    memset(write_buffer, 0, write_buffer_len);
    parameter = (char *) cli_get_parameter(command_string, 2, &string_length);
    strncpy(write_buffer, parameter, string_length);
    char *msg2 = output_string( write_buffer," \n SATHVIK : Param 2- ");
    UART_Transmit(msg2);
	// Return FALSE if no more strings are to be returned
	return (FALSE);
}

/* Get Command Implementation */
base_type_t get_command_interpreter(char *write_buffer, size_t write_buffer_len, const char *command_string)
{

}


base_type_t get_number_of_parameters(const char *command_string)
{
	base_type_t parameter_count = 0;
	base_type_t is_last_character_space = FALSE;

	/* Count the number of space delimited words in command_string. */
	while (*command_string != 0x00)
	{
		if ((*command_string) == ' ')
		{
			if (is_last_character_space != PASS)
			{
				parameter_count++;
				is_last_character_space = PASS;
			}

		}
		else
		{
			is_last_character_space = FALSE;
		}

		command_string++;
	}

	/* If the command string ended with spaces, then there will have been too many parameters counted. */

	if( is_last_character_space == PASS )
	{
		parameter_count--;
	}

	/* The value returned is one less than the number of space delimited words, as the first word should be the command itself. */
	return parameter_count;
}


const char *cli_get_parameter(const char *command_string, base_type_t wanted_parameter, base_type_t *parameter_string_length)
{
	base_type_t parameters_found = 0;
	const char *return_param = NULL;
	*parameter_string_length = 0;

	while (parameters_found < wanted_parameter)
	{
		/* Index the character pointer past the current word.  If this is the start
		of the command string then the first word is the command itself. */
		while (((*command_string) != 0x00) && ((*command_string) != ' ' ))
		{
		    command_string++;
		}

		/* Find the start of the next string. */
		while (((*command_string) != 0x00) && ((*command_string) == ' '))
		{
		    command_string++;
		}

		/* Was a string found? */
		if (*command_string != 0x00)
		{
			/* Is this the start of the required parameter? */
			parameters_found++;

			if (parameters_found == wanted_parameter)
			{
				/* How long is the parameter? */
				return_param = command_string;

				while (((*command_string) != 0x00) && ((*command_string) != ' '))
				{
					(*parameter_string_length)++;
					command_string++;
				}

				if (*parameter_string_length == 0)
				{
					return_param = NULL;
				}

				break;
			}
		}
		else
		{
			break;
		}
	}

	return return_param;
}
