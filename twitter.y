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

%{
	#include "headers.h"

%}

%union{
	long long num;
	char *str;
	double real;
	unsigned flag;
}

%error-verbose

%token <str> SOMETHING

%start S

%%

S: SOMETHING
	{
		printf("%s\n", $1);	
	}

%%

int
main(int argc, char *argv[])
{
	return 0;
}
