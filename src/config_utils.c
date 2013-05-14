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
#include "config_utils.h"
#include "json_utils.h"
#include <sys/stat.h>

#define YYPARSE_PARAM param
#include "twitter_parser.h"
extern FILE * yyin;


#define CONFIG_PATH "twitter-cli/config.json"
#define DEFAULT_CONFIG "/.config/"


/**
 * We parse the JSON file in CONFIG_PATH
 */
int config_init(config_t *cfg, char *path) {
	FILE * f;
	FILE * old_yyin;
	int free_path = 1;
	int res;

	if (path == NULL) {
		path = getenv("XDG_CONFIG_DIRS");

		if (path == NULL) {
			if(getenv("HOME") == NULL) {
				perror("Need a working environment. No HOME, really?");
				exit(EXIT_FAILURE);
			}
			free_path = 0;
			path = malloc(strlen(getenv("HOME")) +
						strlen(DEFAULT_CONFIG) + strlen(CONFIG_PATH) + 1);
			path = strcat(
						strcat(
							strcpy(path, getenv("HOME")),
							DEFAULT_CONFIG),
						CONFIG_PATH);
		}
	}

	if((f = fopen(path, "a+")) == NULL) {
		perror("Could not open config file");
		exit(EXIT_FAILURE);
	}

	old_yyin = yyin;
	cfg->file = f;
	yyin = f;
	res = yyparse((void *) &(cfg->config_values));
	yyin = old_yyin;

	if(res == 1) {
		cfg->config_values = create_object(NULL);
	} else {
		printf("res = %d\n", res);
	}

	if(!free_path) // If path isn't free, set it free
		free(path);

	return 0;
}

int config_save(config_t *cfg) {
	rewind(cfg->file);
	ftruncate(fileno(cfg->file), 0);
	pretty_print4(cfg->config_values, 0, "\n", cfg->file);
	fflush(cfg->file);
	return 0;
}

int config_save_key(config_t *cfg, char *key, char *element)
{
	json_t value_j;
	value_j.type = JSON_STRING;
	strcpy(value_j.value.s, element);
	return json_set_object(&(cfg->config_values), key, &value_j);
}

char *config_get_key(config_t *cfg, char *key)
{
	json_t *next; json_context_t ctx;
	char *cur_key;

	ctx = json_context_init();

	while((next = for_object_r(&cfg->config_values, &ctx, &cur_key)) != NULL)
	{
		if (!strcmp(cur_key, key)) {
			return json_as_string(next);
		}
	}
	return NULL;
}

