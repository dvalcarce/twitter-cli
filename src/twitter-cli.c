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
#include "http_lib.h"

extern char *http_server;
extern int http_port;
extern char *http_proxy_server;
extern int http_proxy_port;

extern int yyparse(void);
extern FILE *yyin;

int main(int argc, char *argv[])
{
	char *data = NULL;
	char *filename = NULL;
	char typebuf[100];
	int length;
	int http_retcode;

	if (argc < 2) {
		fprintf(stderr, "No url!\n");
		exit(EXIT_FAILURE);
	}

	http_parse_url(argv[1], &filename);
	http_retcode = http_get(filename, &data, &length, typebuf);

	printf ("http_retcode = %d, typebuf = '%s', length = %d\n", http_retcode, typebuf, length);

	switch(http_retcode) {
		case ERRHOST:
			fprintf(stderr, "No such host.\n");
			break;
		case ERRSOCK:
			fprintf(stderr, "Can't create socket.\n");
			break;
		case ERRCONN:
			fprintf(stderr, "Can't connect to host.\n");
			break;
		case ERRWRHD:
			fprintf(stderr, "Write error on socket while writing header.\n");
			break;
		case ERRWRDT:
			fprintf(stderr, "Write error on socket while writing data.\n");
			break;
		case ERRRDHD:
			fprintf(stderr, "Read error on socket while reading result.\n");
			break;
		case ERRPAHD:
			fprintf(stderr, "Invalid answer from data server.\n");
			break;
		case ERRNULL:
			fprintf(stderr, "Null data pointer.\n");
			break;
		case ERRNOLG:
			fprintf(stderr, "No/Bad length in header.\n");
			break;
		case ERRMEM:
			fprintf(stderr, "Can't allocate memory.\n");
			break;
		case ERR400:
			fprintf(stderr, "400: Invalid query.\n");
			break;
		case ERR403:
			fprintf(stderr, "403: Forbidden.\n");
			break;
		case ERR408:
			fprintf(stderr, "408: Request timeout.\n");
			break;
		case ERR500:
			fprintf(stderr, "500: Server error.\n");
			break;
		case ERR501:
			fprintf(stderr, "501: Not implemented.\n");
			break;
		case ERR503:
			fprintf(stderr, "Service overloaded.\n");
			break;
		case OK0:
			fprintf(stderr, "Successfull parse.\n");
			break;
		case OK201:
			fprintf(stderr, "Ressource succesfully created.\n");
			break;
		case OK200:
			fprintf(stderr, "Ressource succesfully read.\n");
			break;
	}

	printf("data: %s\n", data);

/*
	if ((yyin = fopen(argv[1], "r")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	yyparse();

	fclose(yyin);
*/

	return 0;

}
