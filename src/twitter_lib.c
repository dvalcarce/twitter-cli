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


// We dont currently support more things..
enum _http_method { METHOD_GET, METHOD_POST };

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
		strncpy((char*)(userdata->data + userdata->consumed_space), (char*) ptr, totalsize);
	userdata->data[userdata->consumed_space + totalsize] = 0;

	userdata->consumed_space += totalsize;

	return totalsize;
}

char *
twitter_request_get(char *url, oauth_t oauth, char **get_params, long *status, size_t *recv_len)
{
	struct curl_slist *header_list; // Will hold the OAuth header for libcurl
	char *post_params = NULL;
	char oauth_header[400]; // Buffer for mangling the OAuth header
	char oauth_url[400];

	int i, get_params_len;
	long _status;
	CURLcode res;

	static struct res_buffer buffer; // Holds the response
	buffer.reserved_space = 0;
	buffer.consumed_space = 0;
	buffer.data = NULL;

	strcpy(oauth_url, url);

	for(get_params_len = 1; get_params != NULL && get_params[get_params_len] != NULL; get_params_len++) {
		strcat(strcat(oauth_url, "?"), get_params[get_params_len]);
	}

	url = oauth_sign_url2(oauth_url, &post_params, OA_HMAC, "GET", TWITTER_OAUTH_CK, TWITTER_OAUTH_CS, oauth.token, oauth.token_secret);

		for(i = 0; post_params[i] != '\0'; i++) {
			if(post_params[i] == '&') {
				post_params[i] = ',';
			}
		}

		strcpy(oauth_header, "Authorization: OAuth ");
		strcat(oauth_header, post_params);

		// fprintf(stderr, "URL:    %s\nHEADER: %s\n", url, oauth_header);

		// Check OAuth
		CURL *curl;
		curl = curl_easy_init( );

		header_list = curl_slist_append((struct curl_slist *) NULL, oauth_header);

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_body);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

		res = curl_easy_perform(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &_status);

		if(status != NULL)
			*status = _status;

		if(recv_len != NULL)
			*recv_len = buffer.consumed_space;

		curl_easy_cleanup(curl);
		curl_slist_free_all(header_list);

		if(_status == 401) {

			#if DEBUG_VERBOSE
			printf("[E] Twitter: Error 401: Unauthorized.\n");
			printf("[E] Twitter: Response body follows:\n");
			printf("%s", buffer.data);
			printf("\n[E] Twitter: END OF BODY.\n");
			exit(EXIT_FAILURE);
			#else
			return NULL;
			#endif
		}

	return buffer.data;
}

char *
twitter_perform_request(char *url, enum _http_method method, char *postdata, long postdatasize, struct curl_slist *header_list, long *status, size_t *body_len)
{
	long _status;
	CURLcode res;

	static struct res_buffer buffer; // Holds the response
	buffer.reserved_space = 0;
	buffer.consumed_space = 0;
	buffer.data = NULL;

	CURL *curl;
	curl = curl_easy_init( );

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);

	if(method == METHOD_GET) {
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	} else if(method == METHOD_POST) {
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (postdata == NULL ? 0 : strlen(postdata)));
	}

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_body);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

	res = curl_easy_perform(curl);

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &_status);

	if(status != NULL)
		*status = _status;

	if(body_len != NULL)
		*body_len = buffer.consumed_space;

	curl_easy_cleanup(curl);

	if(_status == 401) {

		#if DEBUG_VERBOSE
		printf("[E] Twitter: Error 401: Unauthorized.\n");
		printf("[E] Twitter: Response body follows:\n");
		printf("%s", buffer.data);
		printf("\n[E] Twitter: END OF BODY.\n");
		#endif
	}

	return buffer.data;
}


char *
twitter_request_post(char *url, oauth_t oauth, char **_postparams, long *status, size_t *recv_len)
{
	// List of headers to give libcurl
	struct curl_slist *header_list;
	int i, param_len;
	char *body, *req_header, *req_url;
	char *postdata = NULL;
	char **postparams;
	for(param_len = 1; _postparams != NULL && _postparams[param_len - 1] != NULL; param_len++);

	postparams = malloc((2 + param_len) * sizeof(void*));
	postparams[0] = strdup(url);

	for(i = 0; i < param_len - 1; i++) {
		postparams[i + 1] = strdup(_postparams[i]);
	}
	postparams[i + 1] = NULL;

	// printf("Params %d:\n\tURL\t%s\n\tParameter#0\t%s\n\tParameter#1\t%s\n", param_len, url, postparams[0], postparams[1]);

	oauth_sign_array2_process(&param_len, &postparams, &postdata, OA_HMAC, "POST", TWITTER_OAUTH_CK, TWITTER_OAUTH_CS, oauth.token, oauth.token_secret);

	req_url = oauth_serialize_url_sep(param_len, 0, postparams, "&", 1);
	req_header = oauth_serialize_url_sep(param_len, 1, postparams, ", ", 6);

	char oauth_header[strlen(req_header) + 55];

	sprintf(oauth_header, "Authorization: OAuth %s", req_header);

	header_list = curl_slist_append((struct curl_slist *) NULL, oauth_header);

	// printf("Making request. URL = %s\nHeader = %s\n", req_url, oauth_header);
	// printf("POST DATA: %s\n", postdata);

	body = twitter_perform_request(req_url, METHOD_POST, postdata, param_len, header_list, status, recv_len);

	curl_slist_free_all(header_list);
	oauth_free_array(&param_len, &postparams);

	return body;
}

/*
char *
twitter_request_post_legacy(char *url, oauth_t oauth, char **postdata, long *status, size_t *recv_len)
{
	struct curl_slist *header_list; // Will hold the OAuth header for libcurl
	char *post_params = NULL;
	char *oauth_header_data;
	char oauth_header[200]; // Buffer for mangling the OAuth header
	char oauth_url[400];

	int i, postdatasize;
	long _status;
	CURLcode res;

	static struct res_buffer buffer; // Holds the response
	buffer.reserved_space = 0;
	buffer.consumed_space = 0;
	buffer.data = NULL;

	strcpy(oauth_url, url);

	for(postdatasize = 0; postdata != NULL && postdata[postdatasize] != NULL; postdatasize++) {
		strcat(strcat(oauth_url, "?"), postdata[postdatasize]);
	}

	url = oauth_sign_url2(oauth_url, &post_params, OA_HMAC, "POST", TWITTER_OAUTH_CK, TWITTER_OAUTH_CS, oauth.token, oauth.token_secret);


	char **oauth_params;
	int oauth_params_len;
	oauth_params_len = oauth_split_url_parameters(oauth_url,, &oauth_params);

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

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &_status);

	if(status != NULL)
		*status = _status;

	curl_easy_cleanup(curl);
	curl_slist_free_all(header_list);

	if(_status == 401) {

		#if DEBUG_VERBOSE
		printf("[E] Twitter: Error 401: Unauthorized.\n");
		printf("[E] Twitter: Response body follows:\n");
		printf("%s", buffer.data);
		printf("\n[E] Twitter: END OF BODY.\n");
		exit(EXIT_FAILURE);
		#else
		return NULL;
		#endif
	}

	return buffer.data;
}
*/

int twitter_request_token(config_t *cfg)
{
	int i, len;
	char *buffer, **response_params = NULL; // Will get us the OAuth header from liboauth

	buffer = twitter_request_post(TWITTER_API_REQUEST_TOKEN, oauth_null(), NULL, NULL, NULL);

	len = oauth_split_url_parameters(buffer, &response_params);

	for(i = 0; i < len; i++) {
		char *key, *value;
		key = strtok(response_params[i], "=");
		value = strtok(NULL, "=");

		config_save_key(cfg, key, value);
	}
	return 1;
}

int twitter_access_token(config_t *cfg, char *oauth_token, char *pin)
{
	int i, len;
	oauth_t auth;
	auth.token = oauth_token;
	auth.token_secret = NULL;

	char *buffer, **response_params = NULL;
	char *params[2];
	params[0] = strdup(oauth_get_header("verifier", pin));
	params[1] = NULL;
	buffer = twitter_request_post(TWITTER_API_ACCESS_TOKEN, auth, params, NULL, NULL);
	free(params[0]);

	len = oauth_split_url_parameters(buffer, &response_params);

	for(i = 0; i < len; i++) {
		char *key, *value;
		key = strtok(response_params[i], "=");
		value = strtok(NULL, "=");

		config_save_key(cfg, key, value);
	}
	return buffer != NULL;
}


json_t *twitter_status_home_timeline(const oauth_t oauth)
{
	static json_t result;
	char *body;
	long status;
	size_t body_len;

	body = twitter_request_get(TWITTER_API_STATUS_HOME_TIMELINE, oauth, NULL, &status, &body_len);

	if(status == 200) {
		yy_scan_string(body);
		yyparse((void*) &result);

		return &result;
	} else {
		fprintf(stderr, "[E] Twitter: Status code: %ld\n", status);
		fprintf(stderr, "[E] Twitter: Body =  %s\n", body);
	}

	return NULL;
}


int twitter_account_verify_credentials(const oauth_t oauth)
{
	long status = 0;

	twitter_request_get(TWITTER_API_ACCOUNT_VERIFY_CREDENTIALS, oauth, NULL, &status, NULL);

	if(status == 200)
		return 1;
	return 0;
}


json_t *twitter_status_update(char *buffer, const oauth_t oauth)
{
	long status;
	size_t body_len;
	static json_t json_value;
	char *params[2], *body;
	char status_buffer[282];

	params[0] = strcat(strcpy(status_buffer, "status="), buffer);
	params[1] = NULL;

	body = twitter_request_post(TWITTER_API_STATUS_UPDATE, oauth, params, &status, &body_len);

	if(status == 401 || body == NULL) {
		fprintf(stderr, "[E] Twitter: Status code: %ld\n", status);
		fprintf(stderr, "[E] Twitter: Body = %s\n", body);
		return NULL;
	}

	yy_scan_string(body);
	yyparse((void*) &json_value);

	return &json_value;
}
