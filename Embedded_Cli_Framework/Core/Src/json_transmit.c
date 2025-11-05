/*
 * json_transmit.c
 *
 *  Created on: Nov 3, 2025
 *      Author: DELL
 */


#include "hal_abstraction.h"
#include "em_cli_main.h"
#include "jsmn.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern char dest_arr[64];
extern int i;

/*compare token string */
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void json_str_transmit(const char *JSON_STRING, jsmntok_t *t, int r)
{
	for (i = 1; i < r; i++)
	{
	  if (jsoneq(JSON_STRING, &t[i], "user") == 0)
	  {
		strncpy(dest_arr, (JSON_STRING + t[i + 1].start), (t[i + 1].end - t[i + 1].start));
		dest_arr[(t[i + 1].end - t[i + 1].start)] = '\0';
		char *msg = output_string(dest_arr,"User: \n");
		UART_Transmit(msg);
		i++;
	  }
	  else if (jsoneq(JSON_STRING, &t[i], "admin") == 0)
	  {
		strncpy(dest_arr, (JSON_STRING + t[i + 1].start), (t[i + 1].end - t[i + 1].start));
		dest_arr[(t[i + 1].end - t[i + 1].start)] = '\0';
		char *msg = output_string(dest_arr,"\n- Admin: ");
		UART_Transmit(msg);
		i++;
	  }
	  else if (jsoneq(JSON_STRING, &t[i], "uid") == 0)
	  {
		strncpy(dest_arr, (JSON_STRING + t[i+1].start), (t[i+1].end - t[i+1].start));
		dest_arr[(t[i+1].end - t[i+1].start)] = '\0';
		char *msg = output_string(dest_arr,"\n- UID: ");
		UART_Transmit(msg);
		i++;
	  }
	  else if (jsoneq(JSON_STRING, &t[i], "groups") == 0)
	  {
		int j;

		UART_Transmit_1("- Groups: \n");
		if (t[i + 1].type != JSMN_ARRAY)
		{
		  continue;
		}

		for (j = 0; j < t[i + 1].size; j++)
		{
		  jsmntok_t *g = &t[i + j + 2];

		  strncpy(dest_arr, (JSON_STRING + g->start), (g->end - g->start));
		  dest_arr[(g->end - g->start)] = '\0';
		  char *msg = output_string(dest_arr,"\n * ");
		  UART_Transmit(msg);
		}

		i += t[i + 1].size + 1;
	  }
	  else
	  {
		strncpy(dest_arr, (JSON_STRING + t[i].start), (t[i].end - t[i].start));
		dest_arr[(t[i].end - t[i].start)] = '\0';
		char *msg = output_string(dest_arr,"Unexpected key: \n");
		UART_Transmit(msg);
	  }

	}
}
