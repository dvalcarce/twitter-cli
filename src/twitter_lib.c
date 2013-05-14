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

struct res_buffer {
	size_t reserved_space;
	size_t consumed_space;
	char *data;
};

static char *get_api_url(char *url, char **params)
{
	static char buffer[1000];
	char url_params[1000];
	int i;
	url_params[0] = 0;

	if(params == NULL)
		return url;

	strcat(url_params, params[0]);

	for(i = 1; params[i] != NULL; i++) {
		strcat(strcat(url_params, "&"), params[i]);
	}

	sprintf(buffer, "%s?%s", url, url_params);
	return buffer;
}





char *oauth_get_header(char *key, char *value)
{
	static char buffer[100];

	buffer[0] = 0;

	strcpy(&(buffer[0]), "oauth_");
	strcat(&(buffer[0]), key);
	strcat(&(buffer[0]), "=");
	strcat(&(buffer[0]), value);
	return &(buffer[0]);
}

static size_t get_body(char *ptr, size_t size, size_t nmemb, void *udata)
{
	int totalsize = size * nmemb;
	struct res_buffer *userdata = (struct res_buffer *) udata;

	if(totalsize > userdata->reserved_space - userdata->consumed_space) {
		register int newspace = userdata->reserved_space + totalsize * 1.5;
		userdata->data = realloc(userdata->data, newspace);
		userdata->reserved_space = newspace;
	}

	if(totalsize > 1)
		strncpy((char*)userdata->data, (char*) ptr, totalsize);
	userdata->data[totalsize] = 0;

	userdata->consumed_space += totalsize;

	return totalsize;
}

int twitter_request_post(config_t *config, char *url, char **postdata, char **v_ptr, size_t *recv_len)
{
	struct curl_slist *header_list; // Will hold the OAuth header for libcurl
	char **response_params = NULL; // Will get us the OAuth header from liboauth
	char *post_params = NULL;
	char *post_oauth_params = NULL; // Will give us the OAuth header rom liboauth
	char oauth_header[200]; // Buffer for mangling the OAuth header
	char oauth_url[400];

	int i, len, postdatasize;
	long status;
	CURLcode res;

	struct res_buffer buffer; // Holds the response
	buffer.reserved_space = 0;
	buffer.consumed_space = 0;
	buffer.data = NULL;

	strcpy(oauth_url, url);

	for(postdatasize = 0; postdata != NULL && postdata[postdatasize] != NULL; postdatasize++) {
		strcat(strcat(oauth_url, "?"), postdata[postdatasize]);
	}

	url = oauth_sign_url2(oauth_url, &post_params, OA_HMAC, "POST", TWITTER_OAUTH_CK, TWITTER_OAUTH_CS, NULL, NULL);

		for(i = 0; post_params[i] != 0; i++) {
			if(post_params[i] == '&') {
				post_params[i] = ',';
			}
		}

		strcpy(oauth_header, "Authorization: OAuth ");
		strcat(oauth_header, post_params);

		fprintf(stderr, "URL:    %s\nHEADER: %s\n", url, oauth_header);

		// Check OAuth
		CURL *curl;
		curl = curl_easy_init( );

		header_list = curl_slist_append((struct curl_slist *) NULL, oauth_header);

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata != NULL ? oauth_serialize_url_parameters(postdatasize, postdata) : NULL);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(oauth_serialize_url_parameters(postdatasize, postdata)));
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_body);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

		res = curl_easy_perform(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

		if(status == 401) {

			#if DEBUG_VERBOSE
			printf("[E] Twitter: Error 401: Unauthorized.\n");
			printf("[E] Twitter: Response body follows:\n");
			printf("%s", buffer.data);
			printf("\n[E] Twitter: END OF BODY.\n");
			exit(EXIT_FAILURE);
			#else
			return 0;
			#endif
		}

		len = oauth_split_url_parameters(buffer.data, &response_params);

		for(i = 0; i < len; i++) {
			char *key, *value;
			key = strtok(response_params[i], "=");
			value = strtok(NULL, "=");

			config_save_key(config, key, value);
		}

		curl_easy_cleanup(curl);
		curl_slist_free_all(header_list);

	return 1;
}


int twitter_request_token(config_t *cfg)
{
	return twitter_request_post(cfg, TWITTER_API_REQUEST_TOKEN, NULL, NULL, NULL);
}

int twitter_access_token(config_t *cfg, char *oauth_token, char *pin)
{
	int r;
	char *params[3];
	params[0] = strdup(oauth_get_header("verifier", pin));
	params[1] = strdup(oauth_get_header("token", oauth_token));
	params[2] = NULL;
	r = twitter_request_post(cfg, TWITTER_API_ACCESS_TOKEN, params, NULL, NULL);
	free(params[0]);
	free(params[1]);
	return r;
}

