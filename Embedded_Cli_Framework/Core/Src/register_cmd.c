/*
  * @file           : register_cmd.c
  * @brief          : Command registration module for Embedded CLI Framework
 */

#include "em_cli_main.h"
extern base_type_t command_count;

#ifndef ARRAY_BASED_COMMAND_REGISTER

extern const cli_command_definition help_command;

cli_Definition_List_Item registered_commands =
{
	&help_command,	/* The first command in the list is always the help command, defined in this file. */
	NULL			/* The next pointer is initialised to NULL, as there are no other registered commands yet. */
};
#endif

base_type_t register_command(const cli_command_definition * const command_to_register) {
	base_type_t is_registered = FALSE;

	#ifndef ARRAY_BASED_COMMAND_REGISTER
    static cli_Definition_List_Item *last_command_in_list = &registered_commands;
    cli_Definition_List_Item *new_list_item;
    /* Create a new list item that will reference the command being registered. */
    new_list_item = ( cli_Definition_List_Item * ) malloc( sizeof( cli_Definition_List_Item ) );
    #endif

    if (command_count < CUSTOM_CLI_MAX_COMMANDS) {
	#ifndef ARRAY_BASED_COMMAND_REGISTER
	if( new_list_item != NULL )
	{
		/* Reference the command being registered from the newly created
		list item. */
		new_list_item->command_line_definition = command_to_register;

		/* The new list item will get added to the end of the list, so
		pxNext has nowhere to point. */
		new_list_item->next = NULL;

		/* Add the newly created list item to the end of the already existing
		list. */
		last_command_in_list->next = new_list_item;

		/* Set the end of list marker to the new list item. */
		last_command_in_list = new_list_item;

	}
	#else
	commands_array[command_count] = *command_to_register;
	#endif
	command_count++;
	is_registered = PASS;
	}

	return is_registered;
}
