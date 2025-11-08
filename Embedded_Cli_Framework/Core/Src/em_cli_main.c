/**
  ******************************************************************************
  * @file           : em_cli_main.c
  * @brief          : Main application file for embedded cli framework
  ******************************************************************************
  * @project        : Embedded CLI Framework
  * @version        : 1.0.0
  * @author         : Sathvik Valasa
  * @date           : 28-Oct-2025
 *
  * @details
  * This project implements a lightweight, UART-based Command Line Interface (CLI)
  * for STM32 microcontrollers. It enables users to register, parse, and execute
  * commands dynamically through a serial console.
  ******************************************************************************
  */

#include "hal_abstraction.h"
#include "em_cli_main.h"
#include "jsmn.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define WAITTIME 100

base_type_t command_count = 0;
char write_buffer[512];

int i;
int r;
char dest_arr[64];
jsmn_parser p;
jsmntok_t t[128]; /* Expect no more than 128 tokens */

cli_command_definition commands_array[CUSTOM_CLI_MAX_COMMANDS];

/* initialize commands */
const cli_command_definition help_command = {
	"help",
	"\r\nhelp : \r\nLists all the registered commands\r\nCommands : \r\n",
	help_command_interpreter,
	-1
};

const cli_command_definition get_command = {
	"get",
	"\r\nget : \r\n Gets the value from the system \r\n",
	get_command_interpreter,
	1
};

const cli_command_definition set_command = {
	"set",
	"\r\nset: \n\rSets the passed value in the system\r\n",
	set_command_interpreter,
	2
};


static const char *JSON_STRING =
    "{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
    "\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

/**
  * @brief
  * Entry point for the Embedded CLI Framework running on STM32.
  * Initializes hardware peripherals (UART, GPIO, etc.), registers CLI commands,
  * and demonstrates command execution through the serial interface.
  */


int main(void)
{
  Init_lib();
  SystemClock_Config();

  /* Initialize all configured peripherals */
  GPIO_Init();
  USART2_UART_Init();

  jsmn_init(&p);
  r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
				   sizeof(t) / sizeof(t[0]));

	if (r < 0)
	{
		strcpy(dest_arr, r);
		char *msg = output_string(dest_arr,"Failed to parse JSON: \n");
		UART_Transmit(msg);

		 return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT)
	{
		UART_Transmit_1("\nObject expected \n");
		return 1;
	}

	/* Loop over all keys of the root object */
	json_str_transmit(JSON_STRING, t, r);

  /*register help command*/
  register_command(&help_command);
  register_command(&set_command);
  register_command(&get_command);

  /* Process Example Command */
  process_command("set temp 25", write_buffer, sizeof(write_buffer));
  process_command("help", write_buffer, sizeof(write_buffer));
  process_command("copy file1 file2", write_buffer, sizeof(write_buffer));
}


