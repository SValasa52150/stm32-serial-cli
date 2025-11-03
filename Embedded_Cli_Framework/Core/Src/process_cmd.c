/**
 * @file    process_cmd.c
 * @brief   This file contains the implementation of functions responsible for
 *          parsing, validating, and executing commands received via the CLI interface.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "em_cli_main.h"

base_type_t process_command(const char * const command_input, char *write_buffer, size_t write_buffer_len)
{
	const char *registered_command_string = NULL;
	base_type_t is_processed = PASS;
	base_type_t command_string_length;

	#ifdef ARRAY_BASED_COMMAND_REGISTER
	static int command_index = 0;

	if (command_index == 0)
	{
		for (command_index = 0; command_index < command_count; command_index++)
		{
			registered_command_string = commands_array[ command_index ].command;
			command_string_length = strlen( registered_command_string) ;

			/* To ensure the string lengths match exactly, so as not to pick up
			a sub-string of a longer command, check the byte after the expected
			end of the string is either the end of the string or a space before
			a parameter. */
			if ((command_input[ command_string_length ] == ' ') || ( command_input [ command_string_length ] == 0x00 ) )
			{
				if (strncmp (command_input, commands_array[ command_index].command, strlen(commands_array[ command_index].command)) == 0)
				{
					char *msg = output_string(registered_command_string,"\n SATHVIK : Command received - \n");
					UART_Transmit(msg);

					/* The command has been found.  Check it has the expected
					number of parameters.  If expected_number_of_parameters is -1,
					then there could be a variable number of parameters and no
					check is made. */
					if(commands_array[command_index].expected_number_of_params >= 0)
					{
						if (get_number_of_parameters( command_input) != commands_array[command_index].expected_number_of_params)
						{
							is_processed = FALSE;
						}
						else
						{
							char *exp_param_num;
							asprintf(&exp_param_num, "%d", (commands_array[command_index].expected_number_of_params));
							char *msg = output_string(exp_param_num,"SATHVIK : number of parameters are - \n");
							UART_Transmit(msg);
						}

					}

					break;

				}

			}

		}

	}

	if (is_processed == FALSE)
	{
		/* The command was found, but the number of parameters with the command was incorrect. */
		strncpy (write_buffer, " \n Incorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n\r\n", write_buffer_len);
		UART_Transmit_1(write_buffer);
	}
	else if ((is_processed == PASS) && (command_index < command_count))
	{
		char *cmnd_index;
		char *prefix = "\n command_index is ";
		asprintf(&cmnd_index, "%d", command_index);
		char *msg = output_string( cmnd_index,"\n command index is - ");
		UART_Transmit(msg);

		/* Call the callback function that is registered to this command. */
		is_processed = commands_array[command_index].command_interpreter (write_buffer, write_buffer_len, command_input);

		/* If is_processed is FALSE, then no further strings will be returned after this one */
		if (is_processed == FALSE)
		{
			command_index = 0;
		}

	}
	else
	{
		strncpy(write_buffer, "\n  command not recognized. Enter 'help' to view a list of available commands.\r\n\r\n", write_buffer_len);
		is_processed = FALSE;
		//printf("%s\n",write_buffer);
		UART_Transmit_1(write_buffer);
	}

	#else /* Linked list based command registry logic from here */

	static const cli_Definition_List_Item *curr_command = NULL;

	if( curr_command == NULL )
	{
		/* Search for the command string in the list of registered commands. */
		for( curr_command = &registered_commands; curr_command != NULL; curr_command = curr_command->pxNext )
		{
			registered_command_string = curr_command->command_line_definition->command;
			command_string_length = strlen( registered_command_string );

			/* To ensure the string lengths match exactly, so as not to pick up
			a sub-string of a longer command, check the byte after the expected
			end of the string is either the end of the string or a space before
			a parameter. */
			if ((command_input[command_string_length] == ' ') || (command_input[command_string_length] == 0x00))
			{
				if (strncmp(command_input, registered_command_string, command_string_length) == 0)
				{
					printf("VAMSI : Command received - %s\n",registered_command_string);
					/* The command has been found.  Check it has the expected
					number of parameters.  If expected_number_of_parameters is -1,
					then there could be a variable number of parameters and no
					check is made. */
					if ((curr_command->command_line_definition->expected_number_of_parameters) >= 0)
					{
						if (prvGetNumberOfParameters(command_input) != (curr_command->command_line_definition->expected_number_of_parameters))
						{
							is_processed = FALSE;
						}
						else
						{
							printf("VAMSI : Number of parameters are - %d\n",curr_command->command_line_definition->expected_number_of_parameters);
						}

					}

					break;
				}

			}

		}

	}

	if ((curr_command != NULL) && (is_processed == FALSE))
	{
		/* The command was found, but the number of parameters with the command was incorrect. */
		strncpy( write_buffer, "Incorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n\r\n", write_buffer_len );
		curr_command = NULL;
		printf("%s\n",write_buffer);
	}
	else if (curr_command != NULL)
	{
		/* Call the callback function that is registered to this command. */
		is_processed = curr_command->command_line_definition->command_interpreter( write_buffer, write_buffer_len, command_input );

		/* If is_processed is FALSE, then no further strings will be returned
		after this one, and	curr_command can be reset to NULL ready to search
		for the next entered command. */
		if( is_processed == FALSE )
		{
			curr_command = NULL;
		}
	}
	else
	{
		/* curr_command was NULL, the command was not found. */
		strncpy( write_buffer, "Command not recognised.  Enter 'help' to view a list of available commands.\r\n\r\n", write_buffer_len );
		is_processed = FALSE;
		printf("%s\n",write_buffer);
	}

	#endif

return is_processed;

}
