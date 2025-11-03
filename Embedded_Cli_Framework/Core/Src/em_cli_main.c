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

#include <C:\Users\DELL\OneDrive\Desktop\Embedded\stm32_workspace\Embedded_Cli_Framework\Core\Inc\hal_abstraction.h>
#include "C:\Users\DELL\OneDrive\Desktop\Embedded\stm32_workspace\Embedded_Cli_Framework\Core\Inc\em_cli_main.h"
#include "C:\Users\DELL\OneDrive\Desktop\Embedded\stm32_workspace\Embedded_Cli_Framework\Core\Inc\jsmn.h"

#define WAITTIME 100

base_type command_count = 0;
char write_buffer[512];

int i;
int r;
char dest_arr[64];
jsmn_parser p;
jsmntok_t t[128]; /* We expect no more than 128 tokens */

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


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

/**
  * @brief
  * Entry point for the Embedded CLI Framework running on STM32.
  * Initializes hardware peripherals (UART, GPIO, etc.), registers CLI commands,
  * and demonstrates command execution through the serial interface.
  */


int main(void)
{

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  Init_lib();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  GPIO_Init();
  USART2_UART_Init();

  jsmn_init(&p);
  r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
				   sizeof(t) / sizeof(t[0]));

	if (r < 0) {
	//printf("Failed to parse JSON: %d\n", r);
	strcpy(dest_arr, r);
	char *msg = output_string(dest_arr,"Failed to parse JSON: \n");
	UART_Transmit(msg);

	 return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
	UART_Transmit_1("\nObject expected \n");
	return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
	  if (jsoneq(JSON_STRING, &t[i], "user") == 0) {

		strncpy(dest_arr, (JSON_STRING + t[i + 1].start), (t[i + 1].end - t[i + 1].start));
		dest_arr[(t[i + 1].end - t[i + 1].start)] = '\0';
		char *msg = output_string(dest_arr,"User: \n");
		UART_Transmit(msg);

		i++;
	  } else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
		/* We may additionally check if the value is either "true" or "false" */

		strncpy(dest_arr, (JSON_STRING + t[i + 1].start), (t[i + 1].end - t[i + 1].start));
		dest_arr[(t[i + 1].end - t[i + 1].start)] = '\0';
		char *msg = output_string(dest_arr,"\n- Admin: ");
		UART_Transmit(msg);

		i++;
	  } else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
		/* We may want to do strtol() here to get numeric value */

		strncpy(dest_arr, (JSON_STRING + t[i+1].start), (t[i+1].end - t[i+1].start));
		dest_arr[(t[i+1].end - t[i+1].start)] = '\0';
		char *msg = output_string(dest_arr,"\n- UID: ");
		UART_Transmit(msg);

		i++;
	  } else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
		int j;

		UART_Transmit_1("- Groups: \n");
		if (t[i + 1].type != JSMN_ARRAY) {
		  continue; /* We expect groups to be an array of strings */
		}
		for (j = 0; j < t[i + 1].size; j++) {
		  jsmntok_t *g = &t[i + j + 2];

		  strncpy(dest_arr, (JSON_STRING + g->start), (g->end - g->start));
		  dest_arr[(g->end - g->start)] = '\0';
		  char *msg = output_string(dest_arr,"\n * ");
		  UART_Transmit(msg);

		}
		i += t[i + 1].size + 1;
	  } else {

		strncpy(dest_arr, (JSON_STRING + t[i].start), (t[i].end - t[i].start));
		dest_arr[(t[i].end - t[i].start)] = '\0';
		char *msg = output_string(dest_arr,"Unexpected key: \n");
		UART_Transmit(msg);

	  }
	}

  /*register help command*/
  register_command(&help_command);
  register_command(&set_command);
  register_command(&get_command);

  /* Process Example Command */
  process_command("set temp 25", write_buffer, sizeof(write_buffer));
  process_command("help", write_buffer, sizeof(write_buffer));
  process_command("copy file1 file2", write_buffer, sizeof(write_buffer));
}


