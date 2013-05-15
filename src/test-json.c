/***************************************************************************
 *   Twitter client for the CLI.                                           *
 *   Copyright (C) 2013 by Santiago Saavedra and Daniel Valcarce           *
 *   <santiagosaavedra@gmail.com>, <danivalsil@gmail.com>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
/***************************************************************************
 *   This is made as a final exercise for Compilers in the University of   *
 *   A Coruna.                                                             *
 *   Year: 2012/2013                                                       *
 **************************************************************************/

#include "headers.h"
#include "json_utils.h"

#define YYPARSE_PARAM param
#include "twitter_parser.h"
extern FILE * yyin;


int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Feed me a file.\n");
		exit(EXIT_FAILURE);
	}

	if ((yyin = fopen(argv[1], "r")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	json_t json_value;

	yyparse((void*) &json_value);

	printf("Testing... \n");

	json_context_t c = json_context_init();
	json_t *arr_val, *obj_val;
	char *key;

	/* Test twitter-like API messages */
	if (json_get_type(&json_value) == JSON_ARRAY) {
		while((arr_val = for_array_r(&json_value, &c))) {
			printf("ARRAY VALUE: ");
			if (json_get_type(arr_val) == JSON_OBJECT) {
				json_context_t c2 = json_context_init();
				while((obj_val = for_object_r(arr_val, &c2, &key))) {
					printf("OBJKEY : %s\n", key);
					printf("VALUE : ");
					pretty_print(*obj_val);
				}
			} else
			pretty_print(*arr_val);
		}
	}

	/* For any value */
	printf("Final value: "); pretty_print(json_value);

	fclose(yyin);

	return 0;
}
