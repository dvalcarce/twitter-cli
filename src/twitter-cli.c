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

#include <ctype.h>
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

int twitter_oauth_interactive()
{
	char pin[16];
	char *oauth_token;
	printf("Please authenticate yourself against Twitter to use the app.\n");

	twitter_request_token(&config);
	if(!!strcmp("true", json_as_string(json_object_get_value(&config.config_values, "oauth_callback_confirmed")))) {
		fprintf(stderr, "[E] OAuth: callback not confirmed.\n");
		return 0;
	}
	oauth_token = config_get_key(&config, "oauth_token");

	printf("Please, head to \nhttps://api.twitter.com/oauth/authorize?oauth_token=%s to complete the login process\n", oauth_token);

	printf("You'll then be provided a PIN number.\n");
	printf("Please, enter the PIN: ");
	fflush(stdout);
	fgets(pin, 16, stdin);
	pin[strlen(pin) - 1] = 0; // drop \n

	if(!twitter_access_token(&config, oauth_token, pin)) {
		return 0;
	} else {
		config_save(&config);
		return 1;
	}
}

int check_config(oauth_t *oauth)
{

	config_init(&config, "./config.json");
	*oauth = config_get_oauth(&config);

	if(oauth->token == NULL) {
		return twitter_oauth_interactive();
	}

	*oauth = config_get_oauth(&config);

	if(!twitter_account_verify_credentials(*oauth)) {
		fprintf(stderr, "[I] Twitter: credentials expired.\n");
		return twitter_oauth_interactive();
	} else {
		fprintf(stderr, "[N] Twitter: credentials OK.\n");
	}

	pretty_print(config.config_values);

	return 1;

}


void menu_new_tweet(const oauth_t auth)
{
	char buffer[282], *user, *tw_id_str;
	json_t *output;

	printf("New tweet: ");
	fflush(stdout);
	fgets(buffer, 282, stdin);

	buffer[strlen(buffer) - 1] = 0;

	output = twitter_status_update(buffer, auth);

	if(output != NULL) {
		printf("Tweet sent!\n");
		user = json_as_string(json_object_get_value(json_object_get_value(output, "user"), "screen_name"));

		tw_id_str = json_as_string(json_object_get_value(output, "id_str"));

		sprintf(buffer, "https://twitter.com/%s/status/%s", user, tw_id_str);
		printf("See it at: %s\n", buffer);
	}

}


void menu_show_timeline(const oauth_t auth)
{
	json_t *response;

	printf("Stuff..\n");
	response = twitter_status_home_timeline(auth);
	pretty_print(*response);

	printf("Stuff..\n");
}


void menu_main(oauth_t *auth)
{
	char buffer[4];

	printf("Welcome to twcli\n");
	printf("----------------\n");
	printf("\nOptions:\n");
	printf(" 1. New tweet.\n");
	printf(" 2. Show your timeline (last 20 tweets).\n");
	printf(" 3. Exit.\n");

	fgets(buffer, 4, stdin);
	fflush(stdin);

	switch(tolower(buffer[0]))
	{
		case '1':
			menu_new_tweet(*auth);
			return;
		case '2':
			menu_show_timeline(*auth);
			return;
		case '3':
			exit_flag = 1;
			return;
	}
}


int main(int argc, char *argv[])
{
	oauth_t user_oauth;

	while(!check_config(&user_oauth)) {
		fprintf(stderr, "Unauthorized. Maybe you put a wrong pin?\n");
		fprintf(stderr, "Please, try again or press Ctrl.+C to abort.\n");
		fprintf(stderr, "\n\n\n");
	}

	while(!exit_flag) {
		menu_main(&user_oauth);
	}

	return 0;

}
