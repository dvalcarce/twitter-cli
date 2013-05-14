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
#include "curl/curl.h"
#include "oauth.h"
#include "json_utils.h"
#include "config_utils.h"
#include "twitter_lib.h"

#define YYPARSE_PARAM param
#include "twitter_parser.h"
extern FILE * yyin;


int exit_flag = 0;

config_t config;

int check_config()
{
	char *oauth_token;
	char pin[16];
	config_init(&config, "./config.json");
	oauth_token = config_get_key(&config, "oauth_token");

	if(oauth_token == NULL) {
		for(;;) {
			printf("Please authenticate yourself against Twitter to use the app.\n");

			twitter_request_token(&config);
			if(!!strcmp("true", json_as_string(json_object_get_value(&config.config_values, "oauth_callback_confirmed")))) {
				fprintf(stderr, "Problem with OAuth. Run the app again, please.\n");
				exit(EXIT_FAILURE);
			}
			oauth_token = config_get_key(&config, "oauth_token");

			printf("Please, head to \nhttps://api.twitter.com/oauth/authorize?oauth_token=%s to complete the login process\n", oauth_token);

			printf("You'll then be provided a PIN number.\n");
			printf("Please, enter the PIN: ");
			fflush(stdout);
			fgets(pin, 16, stdin);
			pin[strlen(pin) - 1] = 0; // drop \n

			if(!twitter_access_token(&config, oauth_token, pin)) {
				printf("Unauthorized. Maybe you put a wrong pin?\n");
				printf("\n\n\n");
				continue;
			} else {
				break;
			}
		}
		config_save(&config);

	} else {

		// Check that oauth token authenticates against something
	}

	pretty_print(config.config_values);

}


void menu_ask_login()
{

}
void menu_main()
{
	exit_flag = 1;
}


int main(int argc, char *argv[])
{
	if(!check_config()) {
		menu_ask_login();
	}

	while(!exit_flag) {
		menu_main();
	}

	return 0;

}
