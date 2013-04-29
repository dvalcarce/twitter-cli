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

	yyparse();

	fclose(yyin);

	return 0;
}
